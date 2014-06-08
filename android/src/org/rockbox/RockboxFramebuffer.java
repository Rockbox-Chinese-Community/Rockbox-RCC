/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2010 Thomas Martitz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

package org.rockbox;

import java.nio.ByteBuffer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewConfiguration;


public class RockboxFramebuffer extends SurfaceView 
                                 implements SurfaceHolder.Callback
{
    private final DisplayMetrics metrics;
    private final ViewConfiguration view_config;
    private Bitmap btm=null;
    private int srcWidth=0,srcHeight=0, desWidth, desHeight,paddingHeight=0,paddingWidth=0;
    private int fixedWidth, fixedHeight;
    private float scaleWidthFactor,scaleHeightFactor;
    private boolean statusBarOn = false; 
    private boolean fastScale =false;
    private boolean keepAspectRatio =false; 
    private Matrix myMatrix=null;
    private Paint myPaint=null;
    private boolean downscaled = false;
    /* first stage init; needs to run from a thread that has a Looper 
     * setup stuff that needs a Context */
    public RockboxFramebuffer(Context c)
    {
        super(c);
        metrics = c.getResources().getDisplayMetrics();
        view_config = ViewConfiguration.get(c);
        getHolder().addCallback(this);
        /* Needed so we can catch KeyEvents */
        setFocusable(true);
        setFocusableInTouchMode(true);
        setClickable(true);
        /* don't draw until native is ready (2nd stage) */
        setEnabled(false);
        desWidth = metrics.widthPixels;
        desHeight = metrics.heightPixels;
    }

    /* srcWidth, srcHeight: static value from Rockbox internal frambuffer */
    /* desWidth, desHeight: read from the current running android device */
    /* paddingWidth, paddingHeight: the size of black bar when keep scale aspect ratio.*/
    /* fixedWidth, fixedHeight the sum of srcWidth+paddingWidth, srcHeight+paddingHeight */

    private void initialize(int lcd_width, int lcd_height)
    {
        srcWidth = lcd_width;
        srcHeight = lcd_height;   
        scaleWidthFactor = ((float)desWidth) / srcWidth;
        scaleHeightFactor = ((float)desHeight) / srcHeight;
        
        checkDisplayConfig();
   
        getHolder().setFormat(4); /* RGB_565 */  
        btm = Bitmap.createBitmap(srcWidth, srcHeight, Bitmap.Config.RGB_565); 
       
        setEnabled(true);  
    }
   
    private void checkDisplayConfig()
    {
        downscaled = (scaleWidthFactor < 1 || scaleHeightFactor < 1)?true:false;
        
        fastScale = (!RockboxApp.getInstance().getRockboxFastScaleMode())?true:false;   
        
        statusBarOn = (!RockboxApp.getInstance().getTitlebarStatus())?true:false;  
        
        keepAspectRatio = (!RockboxApp.getInstance().getRockboxScaleKeepAspect())?true:false;
        

        if (fastScale == false && keepAspectRatio ==true)
        {
            if (scaleWidthFactor >  scaleHeightFactor && scaleWidthFactor >1)
                scaleWidthFactor =  scaleHeightFactor;
            else
                scaleHeightFactor = scaleWidthFactor;
        }

        if (keepAspectRatio == true)
        {
            if (scaleHeightFactor > scaleWidthFactor)
                paddingHeight = (int)Math.ceil(srcWidth * desHeight / desWidth) - srcHeight;

            if ( scaleWidthFactor > scaleHeightFactor)
                paddingWidth = (int)Math.ceil(srcHeight * desWidth / desHeight) - srcWidth;
        }

        fixedWidth = srcWidth+paddingWidth;
        fixedHeight = (statusBarOn == true)?srcHeight-(int)(getStatusBarHeight()/scaleHeightFactor)+paddingHeight 
                                            : srcHeight+paddingHeight;
 
        if (fastScale == true)  /* surface setFixedSize, emulate the screen resolution no matter what actual LCD resolution is */
        {
            getHolder().setFixedSize(fixedWidth,fixedHeight);
        }
        else
        {
            myMatrix = new Matrix();
            myMatrix.postScale(scaleWidthFactor,scaleHeightFactor);
            /* note. upscaling with Paint flags is very slow. */
            myPaint =(scaleWidthFactor > 1 || scaleHeightFactor > 1)?null: 
                                      new Paint(Paint.ANTI_ALIAS_FLAG| Paint.DITHER_FLAG| Paint.FILTER_BITMAP_FLAG);
        }
    }


    private void update(ByteBuffer framebuffer)
    {
        SurfaceHolder holder = getHolder(); 
        Canvas c;
        Rect dirty = new Rect();


        if (downscaled && fastScale == true) //downscale
        {
           /* (bug!)getHolder may returned a SurfaceHolder with wrong size, ignoring the previous setFixedSize call. (pre-Kitkat) */
           /* needs to menually setFixedSize again, and lock update area according to source width and height. */
           holder.setFixedSize(fixedWidth,fixedHeight);   
           dirty.set(0,0,fixedWidth,srcHeight+paddingHeight);
        }
        else
           dirty = null;
         
        c = holder.lockCanvas(dirty);

        if (c == null) 
            return;
        
        btm.copyPixelsFromBuffer(framebuffer);
        synchronized (holder)
        { /* draw */
            if (fastScale == true)
                c.drawBitmap(btm, 0.0f, 0.0f, null);
            else
                c.drawBitmap(btm,myMatrix,myPaint); 
        }
        holder.unlockCanvasAndPost(c);
    }
    
    private void update(ByteBuffer framebuffer, Rect dirty)
    { 
        SurfaceHolder holder = getHolder();
        Rect scaledDirty = new Rect();

        if (fastScale == true)
        {
            scaledDirty = dirty;
            if (downscaled) /* downscale */
                holder.setFixedSize(fixedWidth,fixedHeight);
        }
        else
            scaledDirty.set((int)(dirty.left * scaleWidthFactor), (int)(dirty.top * scaleHeightFactor),
                        (int)(dirty.right * scaleWidthFactor), (int)(dirty.bottom * scaleHeightFactor));    

        if (scaledDirty.isEmpty()) /*  (left >= right or top >= bottom) */
            scaledDirty.sort();
          
        Canvas c = holder.lockCanvas(scaledDirty);
        if (c == null) 
            return;

        /* can't copy a partial buffer, but it doesn't make a noticeable difference anyway */
        btm.copyPixelsFromBuffer(framebuffer);
        synchronized (holder)
        {   /* draw */
            if (fastScale == true)
                c.drawBitmap(btm, 0.0f, 0.0f, null);
            else
                c.drawBitmap(btm,myMatrix,myPaint);
        }
        holder.unlockCanvasAndPost(c);
    }

    public boolean onTouchEvent(MotionEvent me)
    {
        int x,y;
        if (statusBarOn)  /*need to fix the xy touched coordinates */
        {
           int[] coords = new int[2];
           this.getLocationInWindow(coords);
        
           x = (int)me.getRawX() - coords[0]; 
           y = (int)me.getRawY() - coords[1];   
        }
        else
        {  
            x = (int) me.getRawX();
            y = (int) me.getRawY();
        }
        /*convert */
        x =  (paddingWidth  > 0) ? (int)( x / scaleHeightFactor):(int)( x / scaleWidthFactor);
        y =  (paddingHeight > 0) ? (int)( y / scaleWidthFactor) :(int)( y / scaleHeightFactor);
       
        switch (me.getAction())
        {
        case MotionEvent.ACTION_CANCEL:
        case MotionEvent.ACTION_UP:
            touchHandler(false, x, y);
            return true;
        case MotionEvent.ACTION_MOVE:
        case MotionEvent.ACTION_DOWN:
            touchHandler(true, x, y);
            return true;
        }

        return false;
    }

    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
        return buttonHandler(keyCode, true);
    }

    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
        return buttonHandler(keyCode, false);
    }
 
    private int getDpi()
    {
        return metrics.densityDpi;
    }
    

    private int getScrollThreshold()
    {
        return view_config.getScaledTouchSlop();
    }

    private native void touchHandler(boolean down, int x, int y);
    public native static boolean buttonHandler(int keycode, boolean state);

    public native void surfaceCreated(SurfaceHolder holder);
    public native void surfaceDestroyed(SurfaceHolder holder);
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
    {
        
    }
    public int getStatusBarHeight() 
    {
        int result = 0;
        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
             result = getResources().getDimensionPixelSize(resourceId);
        }
        return result;
    }
}

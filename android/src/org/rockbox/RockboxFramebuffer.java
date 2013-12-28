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
    private Bitmap btm,tmp_bmp;
    private int srcWidth, srcHeight, desWidth, desHeight;
    private float scaleWidth,scaleHeight;
    private Matrix myMatrix;
    private Paint myPaint;
    private Rect scaledDirty;
    private boolean upscaled =false;
    private float stretch_ratio=0;

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


   
    /* second stage init; called from Rockbox with information about the 
     * display framebuffer */
    private void initialize(int lcd_width, int lcd_height)
    {
       srcWidth = lcd_width;
       srcHeight = lcd_height; 
       scaleWidth = ((float)desWidth) / srcWidth;
       scaleHeight = ((float)desHeight) / srcHeight;
       
       myMatrix = new Matrix();
       upscaled = (desWidth > lcd_width) ? true:false;

       if (upscaled) 
       {
           stretch_ratio = (scaleWidth < scaleHeight)? scaleWidth: scaleHeight;
           myMatrix.postScale((float)stretch_ratio,(float)stretch_ratio);
           myPaint = new Paint(Paint.DITHER_FLAG);
       }
       else
       {
           myMatrix.postScale(scaleWidth,scaleHeight);
           myPaint = new Paint(Paint.ANTI_ALIAS_FLAG| Paint.DITHER_FLAG| Paint.FILTER_BITMAP_FLAG);
       }

       scaledDirty = new Rect();
       btm = Bitmap.createBitmap(lcd_width, lcd_height, Bitmap.Config.RGB_565); 
       
       setEnabled(true);
    }
  

    private void update(ByteBuffer framebuffer)
    {
        SurfaceHolder holder = getHolder();                            
        Canvas c = holder.lockCanvas();
        if (c==null) 
            return;

        btm.copyPixelsFromBuffer(framebuffer);
      
        synchronized (holder)
        { /* draw */    
            if (upscaled){
                tmp_bmp =Bitmap.createScaledBitmap(btm,(int)(desWidth/stretch_ratio),(int)(desHeight/stretch_ratio),true);
                c.drawBitmap(tmp_bmp,myMatrix,myPaint);
                tmp_bmp.recycle();
            }else{
                c.drawBitmap(btm,myMatrix,myPaint); 
            }  
        }
        holder.unlockCanvasAndPost(c);
        
    }
    
    private void update(ByteBuffer framebuffer, Rect dirty)
    {
        SurfaceHolder holder = getHolder();         
        
        scaledDirty.set((int)(dirty.left * scaleWidth), (int)(dirty.top * scaleHeight),
                        (int)(dirty.right * scaleWidth), (int)(dirty.bottom * scaleHeight));
        Canvas c = holder.lockCanvas(scaledDirty);
         if (c==null) 
            return; 
         /* can't copy a partial buffer, but it doesn't make a noticeable difference anyway */       
        btm.copyPixelsFromBuffer(framebuffer);
        
        synchronized (holder)
        {    
            /* draw */          
            if (upscaled){
                tmp_bmp =Bitmap.createScaledBitmap(btm,(int)(desWidth/stretch_ratio),(int)(desHeight/stretch_ratio),true);
                c.drawBitmap(tmp_bmp,myMatrix,myPaint);
                tmp_bmp.recycle();
            }else{
                c.drawBitmap(btm,myMatrix,myPaint); 
            }                 
        }
        holder.unlockCanvasAndPost(c);
        
    }

    public boolean onTouchEvent(MotionEvent me)
    {
        

        int x = (int) me.getX();
        int y = (int) me.getY();
     
        x = (int)( x / scaleWidth);
        y = (int)( y / scaleHeight);
        
        
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
}

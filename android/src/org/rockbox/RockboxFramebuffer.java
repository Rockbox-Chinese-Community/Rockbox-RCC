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
import android.graphics.Paint;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewConfiguration;



public class RockboxFramebuffer extends SurfaceView 
                                 implements Runnable,SurfaceHolder.Callback
{
    private final DisplayMetrics metrics;
    private final ViewConfiguration view_config;
    private Bitmap btm,tmp_bmp;
    private int srcWidth, srcHeight, desWidth, desHeight;
    private float ScaleWidthFactor,scaleHeightFactor;
    private Paint myPaint;
    private Thread thread = null;
    private boolean firstrun = true;
    private Rect dirty=null;
    private boolean upscale = false;
    private boolean statusBarOn = false; 
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
       ScaleWidthFactor = ((float)desWidth) / srcWidth;
       scaleHeightFactor = ((float)desHeight) / srcHeight;
       /*Limited the upscaled ratio to <= 2 for better looks*/
       ScaleWidthFactor  = (ScaleWidthFactor > 2) ? 2: ScaleWidthFactor;
       scaleHeightFactor = (scaleHeightFactor > 2) ? 2 : scaleHeightFactor;

       if ( (desWidth >= srcWidth) || (desHeight >=  srcHeight) )
       {  
           upscale = true;
       }
    
       if (!RockboxApp.getInstance().getTitlebarStatus())  
           statusBarOn=true;
       
       /*surface setFixedSize, emulate the screen resolution no matter what actual LCD resolution is*/
       if (!statusBarOn)
          getHolder().setFixedSize(srcWidth,srcHeight);
       else
          getHolder().setFixedSize(srcWidth,srcHeight-(int)(getStatusBarHeight()/scaleHeightFactor));  
           
       if (ScaleWidthFactor == scaleHeightFactor && ScaleWidthFactor ==1) //no zoom
           myPaint = null;
       else
           myPaint = new Paint(Paint.ANTI_ALIAS_FLAG| Paint.DITHER_FLAG| Paint.FILTER_BITMAP_FLAG);

       btm = Bitmap.createBitmap(lcd_width, lcd_height, Bitmap.Config.RGB_565); 
       
       setEnabled(true);
       thread = new Thread(this);
       firstrun = false;
        
       thread.start(); 
    }
  
    private void update(ByteBuffer framebuffer)
    {
        
        synchronized(btm)
        {
            btm.copyPixelsFromBuffer(framebuffer);   
        }
        this.dirty = null; 

    }
    
    private void update(ByteBuffer framebuffer, Rect dirty)
    {  
        synchronized(btm)
        {
            try{
            btm.copyPixelsFromBuffer(framebuffer);
            }catch(Exception e){}  
        }
        this.dirty = new Rect();
        try{
            this.dirty.set(dirty);
        }catch(Exception e2){ 
             this.dirty = null;
        }      
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
        x = (int)( x / ScaleWidthFactor); 
        y = (int)( y / scaleHeightFactor);
       

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

/*      
 *  use a sepreate thread to handle the drawing - constant interval(5ms) 
 *  so no need to draw canvas as much as the old method.  
 *  
 */

    public void run()
    {
        Rect scaledDirty= new Rect();
        Canvas c;
        SurfaceHolder holder;
        while(firstrun == false)
        {    
            try{
                Thread.sleep(5);
            }catch (Exception e){}
            
            holder = getHolder();
            holder.setFormat(4); //RGB_565
            if (dirty == null)                            
                c = holder.lockCanvas();
            else
            {   
                scaledDirty.set(dirty.left ,dirty.top, dirty.right,dirty.bottom);  
                c = holder.lockCanvas(scaledDirty); 
            }   
            
            if (c==null) 
                continue;

            synchronized (holder)
            {     
                synchronized(btm)
                {    
                    c.drawBitmap(btm,0,0,myPaint);         
                }             
            }
            holder.unlockCanvasAndPost(c);
        }
        return;
    }

    public void stopUpdate()
    {
         firstrun = true; 
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

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
//added by zhkailing@163.com

package org.rockbox;


import org.rockbox.Helper.Logger;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;
import android.media.AudioManager;

public class RockboxApp extends Application {
    private static RockboxApp instance; //static For Audiorack ...
    private static final String VolLock_KEY = "VolLockKey";
    private static final String VolLock_KEY_STAT = "VolLockKeyStat";
    /* Initialize status */
    private boolean RockboxVolLockStatus = false; //初始化音量锁定状态
    private int vol=1; //初始化锁定音量
    
    public static RockboxApp getInstance() {
        return instance;
    }
    
    public int getVol(){
        return vol;
    }
    
    public boolean getRockboxVolLockStatus(){
        return RockboxVolLockStatus;
    }
    
    public void setRockboxVolLockStatus(boolean s){
        RockboxVolLockStatus = s;
    }
    
    public void setVol(int s){
        vol = s;
    }
    
    /*Set Volume Lock*/
    private void SetVolLock()
    {
    	if (RockboxVolLockStatus == true) {
        AudioManager audiomanager;
        int streamtype = AudioManager.STREAM_MUSIC;
        audiomanager =
                (AudioManager) this.getSystemService(Context.AUDIO_SERVICE);
        int maxstreamvolume = audiomanager.getStreamMaxVolume(streamtype);
        int VolSet;
        VolSet = maxstreamvolume*(vol-1)/99;
        try{
            audiomanager.setStreamVolume(streamtype, VolSet, 0);
            }catch (Exception e){
                Logger.d("setStreamVolume error!");
            }
        }
    }
    
    /*Save Volume Lock Setting, -1 means to only save VolLock_KEY_STAT*/
    public void SaveVolLock(int vollock)
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        if (vollock != -1)
        {
            editor.putInt(VolLock_KEY, vollock);
            Logger.d("Set the Volume-Lock:"+(vollock-100)+"db");
        }
        editor.putBoolean(VolLock_KEY_STAT, RockboxVolLockStatus);
        editor.commit();
    }
    
    /*Read Volume Lock Setting*/
    private void ReadVolLock()
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        vol = prefs.getInt(VolLock_KEY, 0);
        RockboxVolLockStatus = prefs.getBoolean(VolLock_KEY_STAT, false);
    }
    
    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();
        instance = this; //主要方便RockboxPCM获取Application
        ReadVolLock();
        SetVolLock(); //增加自定义Application，可以在桌面插件启动前锁定音量
    }
}



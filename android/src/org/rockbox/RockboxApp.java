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
import android.os.PowerManager;

public class RockboxApp extends Application {
    private static RockboxApp instance; //static For Audiotrack ...
    private static final String VolLock_KEY = "VolLockKey";
    private static final String VolLock_KEY_STAT = "VolLockKeyStat";
    private static final String WakeLock_KEY_STAT = "WakeLockKeyStat";
    private static final String RUN_KEY_STAT = "isFirstRun";
    /* Initialize status */
    private boolean RockboxVolLockStatus = false; //初始化音量锁定状态
    private boolean isFirstRunStatus = true;
    private int vol=1; //初始化锁定音量
    private PowerManager.WakeLock RockboxWakeLock = null;
    private boolean RockboxWakeLockStatus = false; //初始化Wakeock状态
    
    public static RockboxApp getInstance() {
        return instance;
    }
    
    public int getVol(){
        return vol;
    }
    
    public boolean getRockboxVolLockStatus(){
        return RockboxVolLockStatus;
    }
    
    public void setRockboxVolLockStatus(boolean b){
        RockboxVolLockStatus = b;
    }
    
    public boolean getRockboxWakeLockStatus(){
        return RockboxWakeLockStatus;
    }
    
    public void setRockboxWakeLockStatus(boolean b){
    	RockboxWakeLockStatus = b;
    }
    
    public void setVol(int s){
        vol = s;
    }
    
    public boolean getRunStatus(){
        return isFirstRunStatus;
    }
    
    /*Set Volume Lock*/
    private void EnableStreamVolumeSetting()
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
    
    /*Save Volume Lock Setting and Wake Lock Setting*/
    public void SaveSetting(int vollock, boolean vollockstat, boolean isvolseton, boolean wakelockstat, boolean iswakeseton)
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        if (isvolseton)
        {
            editor.putInt(VolLock_KEY, vollock);
            Logger.d("Set the Volume-Lock:"+(vollock-100)+"db");
            editor.putBoolean(VolLock_KEY_STAT, vollockstat);
        }
        if (iswakeseton)
        	editor.putBoolean(WakeLock_KEY_STAT, wakelockstat);
        editor.commit();
    }
    
    /*Read Setting*/
    private void ReadSetting()
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        vol = prefs.getInt(VolLock_KEY, 0);
        RockboxVolLockStatus = prefs.getBoolean(VolLock_KEY_STAT, false);
        RockboxWakeLockStatus = prefs.getBoolean(WakeLock_KEY_STAT, false);
        isFirstRunStatus = prefs.getBoolean(RUN_KEY_STAT, true);
    }
    
    /* Acquire WakeLock */
    public void acquireWakeLock()
    {
    	if (RockboxWakeLockStatus) {
            if (RockboxWakeLock == null) {
            Logger.d("Rockbox is acquiring wake lock");
            PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
            RockboxWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "RockboxService");
            RockboxWakeLock.acquire();
            }
    	}
    }
    
    /* Release WakeLock */
    public void releaseWakeLock()
    {
    	if (RockboxWakeLockStatus) {
            if (RockboxWakeLock != null && RockboxWakeLock.isHeld()) {
            Logger.d("Rockbox is releasing wake lock");
            RockboxWakeLock.release();
            RockboxWakeLock =null;
            }
    	}
    }
    
    private void firstrun()
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        editor.putBoolean(RUN_KEY_STAT, false);
        editor.commit();
    }
    
    @Override
    public void onCreate() {
        // TODO Auto-generated method stub
        super.onCreate();
        instance = this; //主要方便RockboxPCM获取Application
        ReadSetting();
        firstrun();
        EnableStreamVolumeSetting(); //增加自定义Application，可以在桌面插件启动前锁定音量
    }
}



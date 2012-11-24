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


import org.rockbox.Helper.Logger;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.media.AudioManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.os.ResultReceiver;
import android.text.InputType;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.EditText;
import android.widget.Toast;

import com.umeng.analytics.MobclickAgent;
import com.umeng.fb.NotificationType;
import com.umeng.fb.UMFeedbackService;

public class RockboxActivity extends Activity 
{
    private static final String VolLock_KEY = "VolLockKey";
    private static final String VolLock_KEY_STAT = "VolLockKeyStat";
    /* Initialize status */
    private PowerManager.WakeLock RockboxWakeLock = null;
    private boolean RockboxWakeLockStatus = false; //初始化Wakeock状态
    private boolean RockboxVolLockStatus = false; //初始化音量锁定状态
    private int vol=0; //初始化锁定音量
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        /* Volume Lock Func */
        ReadVolLock();
        SetVolLock();
        UMFeedbackService.enableNewReplyNotification(this, NotificationType.AlertDialog);
	Toast.makeText(this, "Rockbox中文社区精心定制", Toast.LENGTH_LONG).show();
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                             WindowManager.LayoutParams.FLAG_FULLSCREEN);
        Intent intent = new Intent(this, RockboxService.class);
        intent.setAction(Intent.ACTION_MAIN);
        intent.putExtra("callback", new ResultReceiver(new Handler(getMainLooper())) {
            private boolean unzip = false;
            private ProgressDialog loadingdialog;
            private void createProgressDialog()
            {
                loadingdialog = new ProgressDialog(RockboxActivity.this);
                loadingdialog.setMessage(getString(R.string.rockbox_extracting));
                loadingdialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
                loadingdialog.setIndeterminate(true);
                loadingdialog.setCancelable(false);
                loadingdialog.show();
            }

            @Override
            protected void onReceiveResult(final int resultCode, final Bundle resultData)
            {
                RockboxFramebuffer fb;
                switch (resultCode) {
                    case RockboxService.RESULT_INVOKING_MAIN:
                        if (loadingdialog != null)
                            loadingdialog.dismiss();
                        fb = new RockboxFramebuffer(RockboxActivity.this);
                        setContentView(fb);
                        fb.requestFocus();
                        break;
                    case RockboxService.RESULT_LIB_LOAD_PROGRESS:
                        if (loadingdialog == null)
                            createProgressDialog();
                        loadingdialog.setIndeterminate(false);
                        loadingdialog.setMax(resultData.getInt("max", 100));
                        loadingdialog.setProgress(resultData.getInt("value", 0));
                        break;
                    case RockboxService.RESULT_LIB_LOADED:
                        unzip = resultData.getBoolean("unzip");
                        break;
                    case RockboxService.RESULT_SERVICE_RUNNING:
                        if (!unzip) /* defer to RESULT_INVOKING_MAIN */
                        {
                            fb = new RockboxFramebuffer(RockboxActivity.this);
                            setContentView(fb);
                            fb.requestFocus();
                        }
                        setServiceActivity(true);
                        break;
                    case RockboxService.RESULT_ERROR_OCCURED:
                        Toast.makeText(RockboxActivity.this, resultData.getString("error"), Toast.LENGTH_LONG).show();
                        break;
                    case RockboxService.RESULT_ROCKBOX_EXIT:
                        finish();
                        break;
                }
            }
        });
        startService(intent);
    }
    
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        menu.clear();
        if (RockboxWakeLockStatus == false)
        menu.add(0, 2, 0, R.string.rockbox_wakelock_on);
        if (RockboxWakeLockStatus == true)
        menu.add(0, 3, 0, R.string.rockbox_wakelock_off);
        menu.add(0, 5, 0, R.string.rockbox_vollock_on);
        if (RockboxVolLockStatus == true)
        menu.add(0, 6, 0, R.string.rockbox_vollock_off);
        menu.add(0, 0, 0, R.string.UMFeedbackUmengTitle);
        menu.add(0, 4, 0, R.string.rockbox_about);
        menu.add(0, 1, 0, R.string.rockbox_exit);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        final EditText InputVol=new EditText(this);
        InputVol.setInputType(InputType.TYPE_CLASS_NUMBER);
        if (RockboxVolLockStatus == true)
        InputVol.setHint("当前值："+Integer.toString(vol));
        else
        InputVol.setHint("当前值："+Integer.toString(vol)+"（未启用）");
        switch (item.getItemId())
        {
            case 0:
                UMFeedbackService.setGoBackButtonVisible();
                UMFeedbackService.openUmengFeedbackSDK(this);
                break;
            case 1:
                android.os.Process.killProcess(android.os.Process.myPid());
                System.exit(0);
                break;
            case 2:
                RockboxWakeLockStatus = true;
                Toast.makeText(this, R.string.rockbox_wakelock_on_toast, Toast.LENGTH_LONG).show();
                break;
            case 3:
                RockboxWakeLockStatus = false;
                Toast.makeText(this, R.string.rockbox_wakelock_off_toast, Toast.LENGTH_LONG).show();
                break;
            case 4:
                new AlertDialog.Builder(this)
            	                .setTitle(R.string.rockbox_about_title)
            	                .setMessage(R.string.rockbox_about_message)
            	                .setPositiveButton(R.string.OK, null)
            	                .show();
                break;
            case 5:
                new AlertDialog.Builder(this)
            	                .setTitle(R.string.rockbox_vollock_info)
            	                .setView(InputVol)
            	                .setPositiveButton(R.string.OK, new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int whichButton) {
                                    	try{
                                            int voltmp=Integer.parseInt(InputVol.getText().toString());
                                            RockboxVolLockStatus = true;
                                            SaveVolLock(voltmp);
                                            vol=voltmp;
                                    	    }catch (Exception e){
                                    		    Logger.d("Volume-Lock input error!");
                                    	    }
                                        }
                                    })
                                .setNegativeButton(R.string.Cancel, null)
                                .show();
                break;
            case 6:
            	RockboxVolLockStatus = false;
            	SaveVolLock(-1);
            	Toast.makeText(this, R.string.rockbox_vollock_off_toast, Toast.LENGTH_LONG).show();
            	break;
         }
        return true;
     }
    
    /* Acquire WakeLock */
    private void acquireWakeLock()
    {
    	if (RockboxWakeLockStatus == true) {
            if (RockboxWakeLock == null) {
            Logger.d("Rockbox is acquiring wake lock");
            PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
            RockboxWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "RockboxService");
            RockboxWakeLock.acquire();
            }
    	}
    }
    
    /* Release WakeLock */
    private void releaseWakeLock()
    {
    	if (RockboxWakeLockStatus == true) {
            if (RockboxWakeLock != null && RockboxWakeLock.isHeld()) {
            Logger.d("Rockbox is releasing wake lock");
            RockboxWakeLock.release();
            RockboxWakeLock =null;
            }
    	}
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
        VolSet = maxstreamvolume*vol/15;
        audiomanager.setStreamVolume(streamtype, VolSet, 0);
        }
    }
    
    /*Save Volume Lock Setting, -1 means to only save VolLock_KEY_STAT*/
    private void SaveVolLock(int vollock)
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        if (vollock != -1)
        editor.putInt(VolLock_KEY, vollock);
        editor.putBoolean(VolLock_KEY_STAT, RockboxVolLockStatus);
        editor.commit();
        if (vollock != -1)
        Toast.makeText(this,R.string.rockbox_vollock_toast, Toast.LENGTH_LONG).show();
    }
    
    /*Read Volume Lock Setting*/
    private void ReadVolLock()
    {
        String prefName = "Rockbox";
        SharedPreferences prefs = getSharedPreferences(prefName, MODE_PRIVATE);
        vol = prefs.getInt(VolLock_KEY, 0);
        RockboxVolLockStatus = prefs.getBoolean(VolLock_KEY_STAT, false);
    }
    
    private void setServiceActivity(boolean set)
    {
        RockboxService s = RockboxService.getInstance();
        if (s != null)
            s.setActivity(set ? this : null);
    }

    public void onResume()
    {
        super.onResume();
        releaseWakeLock();
        MobclickAgent.onResume(this);
        setVisible(true);
    }
    
    /* this is also called when the backlight goes off,
     * which is nice 
     */
    @Override
    protected void onPause() 
    {
        super.onPause();
        acquireWakeLock();
        MobclickAgent.onPause(this);
        /* this will cause the framebuffer's Surface to be destroyed, enabling
         * us to disable drawing */
        setVisible(false);
    }
    
    @Override
    protected void onStop() 
    {
        super.onStop();
        setServiceActivity(false);
    }
    
    @Override
    protected void onDestroy() 
    {
        super.onDestroy();
        setServiceActivity(false);
    }
}

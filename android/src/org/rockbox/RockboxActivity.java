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
import org.rockbox.jni.RockboxNativeInterface;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.ResultReceiver;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Toast;

import com.umeng.analytics.MobclickAgent;
import com.umeng.fb.NotificationType;
import com.umeng.fb.UMFeedbackService;

import android.os.Environment;
import java.io.File;
import java.io.FileOutputStream;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.ByteArrayOutputStream;
import android.content.DialogInterface;

public class RockboxActivity extends Activity 
{
    private RockboxApp RockboxAppSetting = RockboxApp.getInstance();
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        UMFeedbackService.enableNewReplyNotification(this, NotificationType.AlertDialog);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        if (RockboxAppSetting.getTitlebarStatus())
        {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
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
        if (RockboxAppSetting.getRunStatus())
        {
            new AlertDialog.Builder(this)
                .setTitle(R.string.rockbox_firstrun_title)
                .setMessage(R.string.rockbox_firstrun_message)
                .setPositiveButton(R.string.OK, null)
                .show();
        }
    }
    
    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        menu.clear();
        menu.add(0, 0, 0, R.string.rockbox_simulatebutton);
        menu.add(0, 1, 0, R.string.rockbox_preference);
        menu.add(0, 2, 0, R.string.rockbox_about);
        menu.add(0, 4, 0, "Save EQ Preset");
        menu.add(0, 5, 0, "tinyCoverMaker"); 
        menu.add(0, 3, 0, R.string.rockbox_exit);       
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
            case 5:
              
                final CharSequence[] items = {" 64x64 "," 96x96 "};    
                new AlertDialog.Builder(this)
                         .setTitle("Select icon size")
                         .setSingleChoiceItems(items, -1, new DialogInterface.OnClickListener() {
                              public void onClick(DialogInterface dialog, int item) {
                              int iconSize,result;
                    
                              switch(item)
                              {
                                  case 0:
                                      iconSize=64;
                                      result=tinyCoverMaker(iconSize);
                                      popMessage(result);
                                  break;
                                  case 1:
                                      iconSize=96;
                                      result=tinyCoverMaker(iconSize);
                                      popMessage(result);
                                  break;
                        
                               }
                               dialog.dismiss();    
                              }
                          }) 
                         .show();  
               break; 
            case 4:
                 try {
                RockboxFramebuffer.buttonHandler(85, true); //press
                Thread.sleep(100);
                RockboxFramebuffer.buttonHandler(85, false); //release
                Thread.sleep(300);
                new AlertDialog.Builder(this)
                                .setTitle("Save EQ Preset")
            	                .setMessage("Save EQ OK")
            	                .setPositiveButton(R.string.OK, null)
            	                .show(); 
                saveEQ(); 
                
                RockboxFramebuffer.buttonHandler(85, true); //press
                Thread.sleep(100);
                RockboxFramebuffer.buttonHandler(85, false); //release

                } catch (InterruptedException e) {}
                break;
            case 3:
		        MobclickAgent.onKillProcess(this);
		        RockboxNativeInterface.powerOff();
		        new Thread("Power-Off"){
		            @Override
		            public void run(){
			            try {
                            Thread.sleep(3500); //确保退出
                        } catch (InterruptedException ignored) { }
		                android.os.Process.killProcess(android.os.Process.myPid());
                    }
		        }.start();
                break;
            case 2:
                new AlertDialog.Builder(this)
            	                .setTitle(R.string.rockbox_about_title)
            	                .setMessage(R.string.rockbox_about_message)
            	                .setPositiveButton(R.string.OK, null)
            	                .show();
                break;
            case 0:
                RockboxFramebuffer.buttonHandler(0, true); //press
                try {
                    Thread.sleep(400); //线程阻塞400ms，模拟长按菜单键（WPS_MENU），小于300ms即为WPS_CONTEXT。
                } catch (InterruptedException e) {}
                RockboxFramebuffer.buttonHandler(0, false); //release
                break;
            case 1:
                Intent intent = new Intent();  
                intent.setClass(RockboxActivity.this, RockboxPref.class);  
                 /* 调用一个新的Activity */ 
                startActivity(intent);  
            	break;
         }
        return true;
     }

    public void popMessage(int result)
    {
          final String[] ResultString = {"task completed.","no default Music folder.","cannot access default Music folder.",
                                              "missing sbs file","error, directory name should not contain round breaket."}; 
         new AlertDialog.Builder(this)
                                .setTitle("tinyCoverMaker")
            	                .setMessage(ResultString[result])
            	                .setPositiveButton(R.string.OK, null)
            	                .show();
    }
    private void saveEQ()
    {
    try {
        FileOutputStream fos = new FileOutputStream(new File(Environment.getExternalStorageDirectory(), "rockbox/eqs/eq_backup.cfg"));
        FileInputStream fis = new FileInputStream(new File(Environment.getExternalStorageDirectory(), "rockbox/config.cfg"));
        DataInputStream in = new DataInputStream(fis);
        BufferedReader br =
          new BufferedReader(new InputStreamReader(in));
        String strLine;
        while ((strLine = br.readLine()) != null) {
            if (strLine.startsWith("eq") || strLine.startsWith("compressor") 
                ||strLine.startsWith("replaygain"))
            {
                fos.write((strLine + "\n").toString().getBytes());
            }
        }
        in.close();
        fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    
    private int tinyCoverMaker(int iconSize)
    {
        File cover = null;
        FileOutputStream fos; 
        BufferedReader br;
        DataInputStream in;
        FileInputStream fis;
        String strLine=null,strLine2=null,sbsNameStr=null;
        int currentapiVersion = android.os.Build.VERSION.SDK_INT;
        try{
            //find which theme is calling tinyCoverMaker
            fis = new FileInputStream(new File(Environment.getExternalStorageDirectory(), "rockbox/config.cfg"));
            in = new DataInputStream(fis);
            br = new BufferedReader(new InputStreamReader(in));
        
            while ((strLine = br.readLine()) != null) 
            {
                if ( strLine.startsWith("sbs:") )
                {  
                    String temp = strLine.split("/")[3];
                    sbsNameStr =  temp.split("\\.")[0];
                }
            }
            in.close();

            String path = Environment.getExternalStorageDirectory().toString() +"/rockbox/wps/" + sbsNameStr; 
            File src =new File(path+".sbs" );
            fis = new FileInputStream(src);
            in = new DataInputStream(fis);
            br = new BufferedReader(new InputStreamReader(in));
            File txtFile =new File(path+".sbs_new" );
            if (!txtFile.exists())
            {
                txtFile.createNewFile();
            }
            //read from old file and copy to new
            fos = new FileOutputStream(txtFile);
            while ((strLine = br.readLine()) != null)
            {
                fos.write((strLine +"\n").toString().getBytes());
                if ( strLine.startsWith("##--Generated by tinyCoverMaker--##")) //Magic string to mark where we should append new scripts
                    break;
            }  
            if (strLine == null) //in case Magic string doesn't exist
            {
                fos.write("##--Generated by tinyCoverMaker--##\n".getBytes());  
            }
            in.close();
            fos.close();

      
            if (currentapiVersion >= android.os.Build.VERSION_CODES.KITKAT)
            {
            // can only access "MUSIC" reference folder because Android 4.4.2 
            // block user from access external SD card's absolute path!
                File storageDir =new File (Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC),"");
                if (!storageDir.exists())
                    storageDir.mkdirs();
  
                if(storageDir.isDirectory())
                {
                    File file[] = storageDir.listFiles();
                    int counter=0; 
                    for (File f : file) 
                    {
                    //ok let's find the cover art pic. 
                        if (f.isDirectory()) 
                        {  
                            String coverName=null;
                            String tname1 =  f.getName().toString()+".jpg";
                            String tname2 =  f.getName().toString()+".bmp";
                            String[] names = { "cover.jpg","cover.bmp",tname1,tname2,"folder.jpg","folder.bmp" };
                            if (tname1.contains("(") || tname1.contains(")") )
                                return 4; //error, directory name should not contain round breaket. eg ( or )
 
                            for (int i=0;i<6;i++)
                            {
                                cover = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC),"/"
                                                  +f.getName().toString()+"/"+ names[i]); 
                                coverName = names[i]; 
                                if (cover.exists() == true)
                                {  
                                    break;
                                }        
                            }
                            if (cover.exists() == false) // find the first picture in the folder and make it cover.
                            {
                                File subfile[] = f.listFiles();
                                for (File sf : subfile)
                                {
                                    if (sf.getName().toLowerCase().contains(".jpg") )
                                    {
                                        cover = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC),"/"
                                                  +f.getName().toString()+"/"+ names[0]);
                                        sf.renameTo(cover);
                                        coverName = names[0]; 
                                        break;     
                                    }
                                    else if (sf.getName().toLowerCase().contains(".bmp") )
                                    {
                                        cover = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC),"/"
                                                           +f.getName().toString()+"/"+ names[1]);
                                        sf.renameTo(cover);
                                        coverName = names[1];
                                        break;   
                                    }
                                } 
                            }
                            //if cover exist, decode the file to BMP and save
                            if (cover.exists() == true)
                            {     
                                Bitmap coverBmp = BitmapFactory.decodeFile(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC)+"/"
                                                                        +f.getName().toString()+"/"+ coverName);
                                BitmapEx bmpEx = new BitmapEx(coverBmp.createScaledBitmap(coverBmp,iconSize,iconSize,true));
                            
                                File file2 = new File(path +"/"+ tname2); 
                           
 			        bmpEx.saveAsBMP(new FileOutputStream(file2));
                                //make the script into txt file
                                //e.g %?if(%ss(0,18,%LT), =,SWING HOLIC VOL.07)<%x(c1,SWING HOLIC VOL.07.bmp,0,0)|>
    		                fos = new FileOutputStream(txtFile,true);
                                String script ="%?if(%ss(0,"+ tname2.length() +",%LT), =,"+tname2.split(".bmp")[0]
                                             +")<%x(c"+counter+","+tname2+",0,0)|>"; 
    	                        fos.write((script + "\n").toString().getBytes()); 
    	                        fos.close();
                                counter++;
                            }  
                        }
                    }
                    //lets overwrite the old sbs file here
                    src =new File(path+".sbs_new" );
                    fis = new FileInputStream(src);
                    in = new DataInputStream(fis);
                    br = new BufferedReader(new InputStreamReader(in));
                    txtFile =new File(path+".sbs" );
                    fos = new FileOutputStream(txtFile);
                    while ((strLine = br.readLine()) != null)
                    {
                        fos.write((strLine +"\n").toString().getBytes());
                    }  
                    in.close();
                    fos.close();
                }
                else
                {
                    return 2; //error 2, cannot access storageDir!
                }
            }
            else  //versions before Kitkat
            {
                 //step1. read default music folder 
                fis = new FileInputStream(new File(Environment.getExternalStorageDirectory(), "rockbox/config.cfg"));
                in = new DataInputStream(fis);
                br = new BufferedReader(new InputStreamReader(in));
        
                while ((strLine = br.readLine()) != null) 
                {
                    if ( strLine.startsWith("start directory: ") )
                    {
                        strLine2 = strLine.split(": ")[1];
                    }
                }
                in.close();
                if (strLine2 == null)
                    return 1; //error 1, no dafault music folder
                String defaultMusicFolder = strLine2;
                //step 2. get music folder list
                File storageDir = new File(defaultMusicFolder); 
        
                if(storageDir.isDirectory())
                {
                    File file[] = storageDir.listFiles();
                    int counter=0; 
                    for (File f : file) 
                    {
                        //ok let's find the cover art pic. 
                        if (f.isDirectory()) 
                        {  
                            String tname1 =  f.getAbsolutePath().toString().split(defaultMusicFolder)[1] +".jpg";
                            String tname2 =  f.getAbsolutePath().toString().split(defaultMusicFolder)[1] +".bmp";
                            String[] names = { "cover.jpg","cover.bmp",tname1,tname2,"folder.jpg","folder.bmp" };
                            if (tname1.contains("(") || tname1.contains(")") )
                                return 4; //error, directory name should not contain round breaket. eg ( or )
                            for (int i=0;i<6;i++)
                            {
                                cover = new File(f.getAbsolutePath().toString() +"/"+names[i]); 
                                if (cover.exists() == true)
                                    break;       
                            }
                            if (cover.exists() == false) // find the first picture in the folder and make it cover.
                            {
                                File subfile[] = f.listFiles();
                                for (File sf : subfile)
                                {
                                    if (sf.getName().toLowerCase().contains(".jpg") )
                                    {
                                        cover = new File(f.getAbsolutePath().toString() +"/"+names[0]);
                                        sf.renameTo(cover);  
                                        break; 
                                    }
                                    else if (sf.getName().toLowerCase().contains(".bmp") )
                                    {
                                        cover = new File(f.getAbsolutePath().toString() +"/"+names[1]);
                                        sf.renameTo(cover);
                                        break;  
                                    }
                                } 
                            }
                            //if cover exist, decode the file to BMP and save
                            if (cover.exists() == true)
                            {
                                Bitmap coverBmp = BitmapFactory.decodeFile(cover.getAbsolutePath());
                                BitmapEx bmpEx = new BitmapEx(coverBmp.createScaledBitmap(coverBmp,iconSize,iconSize,true));
                                File file2 = new File(path, tname2); 
 			        bmpEx.saveAsBMP(new FileOutputStream(file2));
                                //make the script into txt file
                                //e.g %?if(%ss(0,18,%LT), =,SWING HOLIC VOL.07)<%x(c1,SWING HOLIC VOL.07.bmp,0,0)|>
    		                fos = new FileOutputStream(txtFile,true);
                                String script ="%?if(%ss(0,"+ tname2.length() +",%LT), =,"+tname2.split(".bmp")[0]
                                                 +")<%x(c"+counter+","+tname2+",0,0)|>"; 
    	                        fos.write((script + "\n").toString().getBytes()); 
    	                        fos.close();
                                counter++;
                            }
                        }
                    }
                    //lets overwrite the old sbs file here
                    src =new File(path+".sbs_new" );
                    fis = new FileInputStream(src);
                    in = new DataInputStream(fis);
                    br = new BufferedReader(new InputStreamReader(in));
                    txtFile =new File(path+".sbs" );
                    fos = new FileOutputStream(txtFile);
                    while ((strLine = br.readLine()) != null)
                    {
                        fos.write((strLine +"\n").toString().getBytes());
                    }  
                    in.close();
                    fos.close();
                }
            }
        }catch (Exception e) {
                e.printStackTrace();
        }
        return 0;      
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
        RockboxAppSetting.releaseWakeLock();
        MobclickAgent.onResume(this);
        setServiceActivity(true);
        setVisible(true);
    }
    
    /* this is also called when the backlight goes off,
     * which is nice 
     */
    @Override
    protected void onPause() 
    {
        super.onPause();
        RockboxAppSetting.acquireWakeLock();
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

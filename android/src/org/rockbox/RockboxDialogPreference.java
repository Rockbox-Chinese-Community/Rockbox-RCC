package org.rockbox;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Environment;
import android.preference.DialogPreference;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.widget.LinearLayout;
import android.widget.Toast;

public class RockboxDialogPreference extends DialogPreference {
    private Context context;
    private LinearLayout layout = null;
    private String rockboxSdDirPath = Environment.getExternalStorageDirectory().getPath()+"/rockbox";
    private File path = new File(rockboxSdDirPath);
    private File filein = new File(rockboxSdDirPath+"/config.cfg"); //源文件
    private File fileout = new File(rockboxSdDirPath+"/config-backup.cfg"); //备份文件
    //初始化变量
    private InputStream configin = null;
    private OutputStream configout = null;
    private boolean isPositive = false;
    private boolean isCancel = false;
    private boolean isNegative = false;
    private boolean isunmount = false;
    private boolean isbackupfilenotfound = false;
        
    public RockboxDialogPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.context = context;
    }
 
        @SuppressWarnings("deprecation")
        @Override
        protected void onPrepareDialogBuilder(Builder builder) {
        //判断SD卡是否挂载
        if (!Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            isunmount = true;
        }
        if (!path.exists()) {
            path.mkdirs();
        }
        layout = new LinearLayout(context);
        layout.setLayoutParams(new LinearLayout.LayoutParams(LinearLayout.LayoutParams.FILL_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT));
        layout.setMinimumWidth(400);
        layout.setPadding(20, 20, 20, 20);  //上下左右的Padding
        builder.setView(layout)
               .setPositiveButton(R.string.pre_rockbox_backup_backup, new DialogInterface.OnClickListener() {
            	   @Override
            	   public void onClick(DialogInterface dialog, int id) {
                       if (!fileout.exists()) {
                           try {
                               fileout.createNewFile();
                           } catch (IOException e) {
                               e.printStackTrace();
                           }
                       }
                       try {
            	           configin = new FileInputStream(filein);
            	           configout = new FileOutputStream(fileout);
                       } catch (FileNotFoundException e) {
                           e.printStackTrace();
                       }
                       isPositive = true;
            	   }
               })
              .setNegativeButton(R.string.pre_rockbox_backup_restore, new DialogInterface.OnClickListener() {
            	   @Override
            	   public void onClick(DialogInterface dialog, int id) {
                       if (!fileout.exists()) {
                           isbackupfilenotfound = true;
                       }
            	       try {
            	           configin = new FileInputStream(fileout);
            	           configout = new FileOutputStream(filein);
                       } catch (FileNotFoundException e) {
            	           e.printStackTrace();
                       }
            	       isNegative = true;
            	   }
              })
              //限制返回键和搜索键
              .setOnKeyListener(new DialogInterface.OnKeyListener() {
                   @Override
                   public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
                       if (keyCode == KeyEvent.KEYCODE_SEARCH || keyCode == KeyEvent.KEYCODE_BACK)
                       {
            	           isCancel = true;
            	           dialog.cancel();
            	           return true;
                       }else{
            	           return false; //默认返回 false
                       }
                   }});
    }
           
    @Override
    protected void onDialogClosed(boolean positiveResult) {
    	/*异常情况直接返回*/
        if (isbackupfilenotfound || isunmount || isCancel)
        {
            if (isunmount)
                Toast.makeText(context, R.string.rockbox_sdcard_notfound, Toast.LENGTH_LONG).show();
            if (isbackupfilenotfound)
                Toast.makeText(context, R.string.rockbox_backupfile_notfound, Toast.LENGTH_LONG).show();
            isPositive = false;
            isCancel = false;
            isNegative = false;
            isunmount = false;
            isbackupfilenotfound = false;
            return;
        }
        /*文件操作*/
        try {
            byte[] buffer = new byte[1024];
            int length;
            while ((length = configin.read(buffer)) > 0) {
                configout.write(buffer, 0, length);
            }

            configout.flush();
            configout.close();
            configin.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
		
        if (isPositive)
        {
            Toast.makeText(context, R.string.rockbox_backup_success, Toast.LENGTH_LONG).show();
        }
    	
        if (isNegative)
        {
            android.os.Process.killProcess(android.os.Process.myPid());
            System.exit(0);
        }

        super.onDialogClosed(positiveResult);
        /*恢复初始值*/
        isPositive = false;
        isCancel = false;
        isNegative = false;
        isunmount = false;
        isbackupfilenotfound = false;
        }
    }


package org.rockbox;

import org.rockbox.Helper.Logger;

import android.os.Bundle;  
import android.preference.CheckBoxPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;

public class RockboxPref extends PreferenceActivity implements OnPreferenceChangeListener {
	
	private String volumelockKey;
	private String wakelockKey;
	private String drivebywireKey;
	private String drivebywireexchangeKey;
	private CheckBoxPreference volumelockCheckPref;
	private CheckBoxPreference wakelockCheckPref;
	private CheckBoxPreference drivebywireCheckPref;
	private CheckBoxPreference drivebywireexchangeCheckPref;
    
    private RockboxApp RockboxAppSetting = RockboxApp.getInstance();
    
	@SuppressWarnings("deprecation")
	@Override  
    public void onCreate(Bundle savedInstanceState) {  
     super.onCreate(savedInstanceState);  
     addPreferencesFromResource(R.xml.preference);
     
     volumelockKey = "volume_lock";
     volumelockCheckPref = (CheckBoxPreference)findPreference(volumelockKey);
     wakelockKey = "wake_lock";
     wakelockCheckPref = (CheckBoxPreference)findPreference(wakelockKey);
     drivebywireKey = "drive_by_wire";
     drivebywireCheckPref = (CheckBoxPreference)findPreference(drivebywireKey);
     drivebywireexchangeKey = "drive_by_wire_exchange";
     drivebywireexchangeCheckPref = (CheckBoxPreference)findPreference(drivebywireexchangeKey);
     //为各个Preference注册监听接口    
     volumelockCheckPref.setOnPreferenceChangeListener(this);
     wakelockCheckPref.setOnPreferenceChangeListener(this);
     drivebywireCheckPref.setOnPreferenceChangeListener(this);
     drivebywireexchangeCheckPref.setOnPreferenceChangeListener(this);
    }

	@Override
	public boolean onPreferenceChange(Preference preference, Object newValue) {    
		Logger.i(preference.getKey()+" Changed");
		if(preference.getKey().equals(volumelockKey))
			RockboxAppSetting.changeRockboxVolLockStatus();
		else if(preference.getKey().equals(wakelockKey))
			RockboxAppSetting.changeRockboxWakeLockStatus();
		else if(preference.getKey().equals(drivebywireKey))
			return true;
		else if(preference.getKey().equals(drivebywireexchangeKey))
			return true;
		else
			return false; 
		return true;
	}  
}

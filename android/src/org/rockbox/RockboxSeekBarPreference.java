package org.rockbox;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.TypedArray;
import android.preference.Preference;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.SeekBar.OnSeekBarChangeListener;

public class RockboxSeekBarPreference extends Preference implements

    OnSeekBarChangeListener {

    public static int maximum = 100; // 最大音量100
    public static int interval = 1; // 按照1递增
    private float oldValue = 1; //默认1
    private TextView view;
    private RockboxApp RockboxAppSetting = RockboxApp.getInstance();

    public RockboxSeekBarPreference(Context context) {
        super(context);
    }

    public RockboxSeekBarPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public RockboxSeekBarPreference(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    protected View onCreateView(ViewGroup parent) {
        // 定义布局
        LinearLayout layout = new LinearLayout(getContext());
        LinearLayout.LayoutParams params = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        params.gravity = Gravity.CENTER;
        layout.setOrientation(LinearLayout.VERTICAL);
        view = new TextView(getContext());
        view.setGravity(Gravity.CENTER);
 
        SeekBar bar = new SeekBar(getContext());
        bar.setMax(maximum);
        bar.setProgress((int) this.oldValue);
        bar.setOnSeekBarChangeListener(this);
  
        view.setText(getTitle()+":"+Integer.toString(bar.getProgress()));

        layout.addView(view);
        layout.addView(bar);
        layout.setId(android.R.id.widget_frame);
        return layout;
    }

    public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

        progress = Math.round(((float) progress) / interval) * interval;
        if (progress < 1)
            progress = 1; //设置范围1-100
        seekBar.setProgress(progress);
        view.setText(getTitle()+":"+Integer.toString(progress));
        this.oldValue = progress;
        updatePreference(progress);
        //notifyChanged();
    }

    public void onStartTrackingTouch(SeekBar seekBar) {
    }

    public void onStopTrackingTouch(SeekBar seekBar) {
    }

    @Override
    protected Object onGetDefaultValue(TypedArray ta, int index) {
        int dValue = (int) ta.getInt(index, 50);
        return validateValue(dValue);
    }

    @Override
    protected void onSetInitialValue(boolean restoreValue, Object defaultValue) {
        int temp = restoreValue ? getPersistedInt(50) : (Integer) defaultValue;
        if (!restoreValue)
            persistInt(temp);
        this.oldValue = temp;
    }

    private int validateValue(int value) {
        if (value > 100)
            value = 100;
        else if (value < 1)
            value = 1;
        else if (value % interval != 0)
            value = Math.round(((float) value) / interval) * interval;
        return value;
    }

    private void updatePreference(int newValue) {
        SharedPreferences.Editor editor = getEditor();
        editor.putInt(getKey(), newValue);
        editor.commit();
        RockboxAppSetting.setVol();
    }

}
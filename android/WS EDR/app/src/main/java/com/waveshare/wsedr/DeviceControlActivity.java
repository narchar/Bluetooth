package com.waveshare.wsedr;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class DeviceControlActivity extends Activity {

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_control);

        //在Activity中设置ActionBar
        getActionBar().setTitle(R.string.actionBar_choose_mode);

        showButtonTelecontroller();
        showButtonUart();
        showButtonDeviceController();
    }

    void showButtonTelecontroller()
    {
        Button button = (Button)findViewById(R.id.buttonTelecontroller);
        final String mDeviceName;
        final String mDeviceAddress;

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        button.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(DeviceControlActivity.this, Telecontroller.class);

                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_NAME, mDeviceName);
                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_ADDRESS, mDeviceAddress);

                startActivity(intent);
            }
        });
    }

    void showButtonUart()
    {
        Button button = (Button)findViewById(R.id.buttonUart);
        final String mDeviceName;
        final String mDeviceAddress;

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        button.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(DeviceControlActivity.this, Uart.class);

                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_NAME, mDeviceName);
                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_ADDRESS, mDeviceAddress);

                startActivity(intent);
            }
        });
    }

    void showButtonDeviceController()
    {
        Button button = (Button)findViewById(R.id.buttonDeviceController);
        final String mDeviceName;
        final String mDeviceAddress;

        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        button.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v) {
                final Intent intent = new Intent(DeviceControlActivity.this, DeviceController.class);

                Log.i("saber","buttonDeviceController");

                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_NAME, mDeviceName);
                intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_ADDRESS, mDeviceAddress);

                startActivity(intent);

            }
        });
    }
}

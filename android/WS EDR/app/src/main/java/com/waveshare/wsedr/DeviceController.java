package com.waveshare.wsedr;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Iterator;
import java.util.UUID;

public class DeviceController extends Activity {

    public static final String EXTRAS_DEVICE_NAME = "DEVICE_NAME";
    public static final String EXTRAS_DEVICE_ADDRESS = "DEVICE_ADDRESS";

    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int MESSAGE_CONNECT = 6;
    public static final int MESSAGE_CONNECT_SUCCEED = 7;
    public static final int MESSAGE_CONNECT_LOST = 8;
    public static final int MESSAGE_RECV = 10;
    public static final int MESSAGE_EXCEPTION_RECV = 11;

    String mDeviceName;
    String mDeviceAddress;

    private boolean mConnected = false;

    //多彩LED
    int RGB_R = 0;
    int RGB_G = 0;
    int RGB_B = 0;

   //显示相关
    String mState;
    TextView RTC_TextView;
    TextView ACC_TextView;
    TextView ADC_TextView;
    TextView JOY_TextView;
    TextView TEMP_TextView;

    //按键状态
    Boolean send_button_state = true;
    TextView buzzer_TextView;

    //蓝牙相关
    public static String EDR_UUID = "00001101-0000-1000-8000-00805F9B34FB";
    private BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mBluetoothSocket = null;
    private InputStream mInStream;
    private OutputStream mOutStream;
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_controller);

        //在Activity中设置ActionBar
        getActionBar().setTitle(R.string.deviceController_mode);

        //得到Activity的输入值
        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        //得到BluetoothAdapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter.getState() != BluetoothAdapter.STATE_ON) {
            Toast.makeText(this, R.string.bluetooth_not_supported, Toast.LENGTH_SHORT).show();
            finish();
        }

        IntentFilter mIntent = new IntentFilter();
        mIntent.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
        mIntent.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);

        registerReceiver(connectDevices, mIntent);
        mHandler.sendEmptyMessageDelayed(MESSAGE_CONNECT, 1000);

        //TextView
        showTextView();
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    public final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MESSAGE_CONNECT:
                    Toast.makeText(DeviceController.this, R.string.ble_connected_success, Toast.LENGTH_SHORT).show();
                    new Thread(new Runnable() {
                        public void run() {
                            InputStream tmpIn;
                            OutputStream tmpOut;
                            try {
                                UUID uuid = UUID.fromString(EDR_UUID);
                                BluetoothDevice btDev = mBluetoothAdapter
                                        .getRemoteDevice(mDeviceAddress);
                                mBluetoothSocket = btDev.createInsecureRfcommSocketToServiceRecord(uuid);
                                mBluetoothSocket.connect();
                                tmpIn = mBluetoothSocket.getInputStream();
                                tmpOut = mBluetoothSocket.getOutputStream();
                            } catch (Exception e) {
                                mConnected = false;
                                mInStream = null;
                                mOutStream = null;
                                mBluetoothSocket = null;
                                e.printStackTrace();
                                mHandler.sendEmptyMessage(MESSAGE_CONNECT_LOST);
                                return;
                            }
                            mInStream = tmpIn;
                            mOutStream = tmpOut;
                            mHandler.sendEmptyMessage(MESSAGE_CONNECT_SUCCEED);
                        }
                    }).start();
                    break;
                case MESSAGE_CONNECT_SUCCEED:
                    mConnected = true;
                    new Thread(new Runnable() {
                        public void run() {
                            byte[] bufRecv = new byte[1024];
                            int nRecv = 0;
                            while (mConnected) {
                                try {
                                    nRecv = mInStream.read(bufRecv);
                                    if (nRecv < 1) {
                                        Thread.sleep(100);
                                        continue;
                                    }

                                    byte[] nPacket = new byte[nRecv];
                                    System.arraycopy(bufRecv, 0, nPacket, 0, nRecv);
                                    mHandler.obtainMessage(MESSAGE_RECV,
                                            nRecv, -1, nPacket).sendToTarget();
                                    Thread.sleep(100);
                                } catch (Exception e) {
                                    mHandler.sendEmptyMessage(MESSAGE_EXCEPTION_RECV);
                                    break;
                                }
                            }
                        }
                    }).start();
                    break;
                case MESSAGE_EXCEPTION_RECV:
                    break;
                case MESSAGE_CONNECT_LOST:
                    Toast.makeText(DeviceController.this, R.string.ble_connected_failed, Toast.LENGTH_SHORT).show();
                    try {
                        if (mInStream != null)
                            mInStream.close();
                        if (mOutStream != null)
                            mOutStream.close();
                        if (mBluetoothSocket != null)
                            mBluetoothSocket.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    } finally {
                        mInStream = null;
                        mOutStream = null;
                        mBluetoothSocket = null;
                        mConnected = false;
                    }
                    break;
                case MESSAGE_WRITE:
                    break;
                case MESSAGE_READ:
                    break;
                case MESSAGE_RECV://考虑到有时会有回车换行符，以下代码并没有做通用性处理。仅供参考
                    byte[] bBuf = (byte[]) msg.obj;
                    String data = bytesToString(bBuf, msg.arg1);
                    if (data != null)
                    {
                        int count = 0;

                        int len = data.length();
                        //遍历data，确定接收到多少个JSON数据
                        for (int i = 0; i<len;i++)
                        {
                            if (data.charAt(i) == '}')
                            {
                                count++;
                            }
                        }

                        Log.i("saber","count="+count);
                        //由于JSON数据会有回车换行，通过以下方法区分开
                        if (count == 1)
                        {
                            parse_json(data);
                        }
                        else if (count == 2)
                        {
                            int index = data.indexOf('}');
                            String temp1 = data.substring(0,index+1);

                            int index2 = data.indexOf("{",index);
                            int index3 = data.indexOf("}",index2);
                            String temp2 =  data.substring(index2,index3+1);

                            parse_json(temp1);
                            parse_json(temp2);
                        }
                        else if (count == 3)
                        {
                            int index = data.indexOf('}');
                            String temp1 = data.substring(0,index+1);

                            int index2 = data.indexOf("{",index);
                            int index3 = data.indexOf("}",index2);
                            String temp2 =  data.substring(index2,index3+1);

                            int index4 = data.indexOf("{",index3);
                            int index5 = data.indexOf("}",index4);
                            String temp3 =  data.substring(index4,index5+1);

                            parse_json(temp1);
                            parse_json(temp2);
                            parse_json(temp3);
                        }
                    }
                break;
            }
        }
    };

    void parse_json(String data)
    {
        try {
            JSONObject jsonObject = new JSONObject(data);

            String key = jsonObject.keys().next().toString();

            if (key.equals("RTC"))
            {
                String RTC_str = jsonObject.getString("RTC");
                RTC_TextView.setText(getResources().getString(R.string.device_display_RTC)+":"+RTC_str);
            }
            else if(key.equals("ACC"))
            {
                String ACC_str = jsonObject.getString("ACC");
                ACC_TextView.setText(getResources().getString(R.string.device_display_ACC)+":"+ACC_str);
            }
            else if(key.equals("ADC"))
            {
                String ADC_str = jsonObject.getString("ADC");
                ADC_TextView.setText(getResources().getString(R.string.device_display_ADC)+":"+ADC_str);
            }
            else if(key.equals("JOY"))
            {
                String JOY_str = jsonObject.getString("JOY");
                JOY_TextView.setText(getResources().getString(R.string.device_display_JOY)+":"+JOY_str);
            }
            else if(key.equals("TEMP"))
            {
                String TEMP_str = jsonObject.getString("TEMP");
                TEMP_TextView.setText(getResources().getString(R.string.device_display_TEMP)+":"+TEMP_str);
            }
            else if(key.equals("BZ"))
            {
                String BZ_str = jsonObject.getString("BZ");
                if (BZ_str.equals("on"))
                {
                    send_button_state=false;
                    buzzer_TextView.setText(getResources().getString(R.string.device_display_BUZZER_OFF));
                }
                else if(BZ_str.equals("off"))
                {
                    send_button_state=true;
                    buzzer_TextView.setText(getResources().getString(R.string.device_display_BUZZER_ON));
                }
            }
        }
        catch (JSONException e)
        {
            e.printStackTrace();
        }


    }
    public static String bytesToString(byte[] b, int length) {
        StringBuffer result = new StringBuffer("");
        for (int i = 0; i < length; i++) {
            result.append((char) (b[i]));
        }

        return result.toString();
    }


    private BroadcastReceiver connectDevices = new BroadcastReceiver() {

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (action.equals(BluetoothDevice.ACTION_ACL_CONNECTED)) {
            } else if (action.equals(BluetoothDevice.ACTION_ACL_DISCONNECTED)) {

            }
        }
    };

    /* DEMO版较为简单，在编写您的应用时，请将此函数放到线程中执行，以免UI不响应 */
    void send(String strValue) {
        if (!mConnected)
            return;
        try {
            if (mOutStream == null)
                return;
            mOutStream.write(strValue.getBytes());
        } catch (Exception e) {
            Toast.makeText(DeviceController.this, R.string.ble_connected_failed, Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if (mConnected) {
                mConnected = false;
                try {
                    Thread.sleep(100);
                    if (mInStream != null)
                        mInStream.close();
                    if (mOutStream != null)
                        mOutStream.close();
                    if (mBluetoothSocket != null)
                        mBluetoothSocket.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            finish();
            return true;
        } else {
            return super.onKeyDown(keyCode, event);
        }
    }

    void showTextView() {
        RTC_TextView = (TextView) findViewById(R.id.RTC_textView);
        ACC_TextView = (TextView) findViewById(R.id.ACC_textView);
        ADC_TextView = (TextView) findViewById(R.id.ADC_textView);
        JOY_TextView = (TextView) findViewById(R.id.JOY_textView);
        TEMP_TextView = (TextView) findViewById(R.id.TEMP_textView);

        final TextView send_TextView = (TextView) findViewById(R.id.Send_textView);
        final EditText oled_EditText = (EditText) findViewById(R.id.OLED_editText);
        send_TextView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!mConnected) return;
                if (oled_EditText.length() < 1) {
                    Toast.makeText(DeviceController.this, R.string.input_send_content, Toast.LENGTH_SHORT).show();
                    return;
                }

                //通过蓝牙发送数据 {"OLED":"x"}
                send("{\"OLED\":\"" + oled_EditText.getText().toString() + "\"}");

                //隐藏软件盘
                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                if (imm.isActive())
                    imm.hideSoftInputFromWindow(oled_EditText.getWindowToken(), 0);
            }
        });

        buzzer_TextView = (TextView) findViewById(R.id.Buzzer_textView);
        buzzer_TextView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.i("saber", "buzzer_TextView");
                if (send_button_state)
                {
                //    buzzer_TextView.setText(getResources().getString(R.string.device_display_BUZZER_OFF));
                    send("{\"BZ\":\"on\"}");
                }
                else
                {
                //    buzzer_TextView.setText(getResources().getString(R.string.device_display_BUZZER_ON));
                    send("{\"BZ\":\"off\"}");
                }
            }
        });

        final TextView rgb_textview = (TextView) findViewById(R.id.RGB_LED_textView);
        final TextView rgb_color_textview = (TextView) findViewById(R.id.RGB_color_textView);
        final SeekBar LEDR_SeekBar = (SeekBar) findViewById(R.id.LEDR_seekBar);
        final SeekBar LEDG_SeekBar = (SeekBar) findViewById(R.id.LEDG_seekBar);
        final SeekBar LEDB_SeekBar = (SeekBar) findViewById(R.id.LEDB_seekBar);

        LEDR_SeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            /**
             * 拖动条停止拖动的时候调用
             */
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
               // 拖动停止
                rgb_color_textview.setBackgroundColor(Color.rgb(RGB_R, RGB_G, RGB_B));
                send("{\"RGB\":\"" + RGB_R + "," + RGB_G + "," + RGB_B + "\"}");
            }
            /**
             * 拖动条开始拖动的时候调用
             */
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            /**
             * 拖动条进度改变的时候调用
             */
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
                RGB_R = progress;
                rgb_textview.setText(getResources().getString(R.string.device_display_RGB)+":"+RGB_R+" "+RGB_G+" "+RGB_B);
            }
        });

        LEDG_SeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            /**
             * 拖动条停止拖动的时候调用
             */
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                rgb_color_textview.setBackgroundColor(Color.rgb(RGB_R, RGB_G, RGB_B));
                send("{\"RGB\":\"" + RGB_R + "," + RGB_G + "," + RGB_B + "\"}");
            }
            /**
             * 拖动条开始拖动的时候调用
             */
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            /**
             * 拖动条进度改变的时候调用
             */
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
                RGB_G = progress;
                rgb_textview.setText(getResources().getString(R.string.device_display_RGB)+":"+RGB_R+" "+RGB_G+" "+RGB_B);
            }
        });

        LEDB_SeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            /**
             * 拖动条停止拖动的时候调用
             */
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                rgb_color_textview.setBackgroundColor(Color.rgb(RGB_R, RGB_G, RGB_B));
                send("{\"RGB\":\"" + RGB_R + "," + RGB_G + "," + RGB_B + "\"}");
            }
            /**
             * 拖动条开始拖动的时候调用
             */
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }
            /**
             * 拖动条进度改变的时候调用
             */
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser) {
                RGB_B = progress;
                rgb_textview.setText(getResources().getString(R.string.device_display_RGB)+":"+RGB_R+" "+RGB_G+" "+RGB_B);
            }
        });
    }
}

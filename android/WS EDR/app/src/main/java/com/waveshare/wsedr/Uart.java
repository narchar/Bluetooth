package com.waveshare.wsedr;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class Uart extends Activity {
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

    //相关控件
    TextView mDataField;
    TextView mSendDataCount;
    TextView mReceiveDataCount;
    EditText edtSend;
    ScrollView svResult;
    Button btnSend;

    //蓝牙相关
    public static String EDR_UUID = "00001101-0000-1000-8000-00805F9B34FB";
    private BluetoothAdapter mBluetoothAdapter;
    BluetoothSocket mBluetoothSocket = null;
    private InputStream mInStream;
    private OutputStream mOutStream;

    //计数相关
    long sendDataCount = 0;
    long receiveDataCount = 0;

    //menu相关
    Menu mMenu;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_uart);

        //在Activity中设置ActionBar
        getActionBar().setTitle(R.string.actionBar_uart_mode);

        //得到Activity的输入值
        final Intent intent = getIntent();
        mDeviceName = intent.getStringExtra(EXTRAS_DEVICE_NAME);
        mDeviceAddress = intent.getStringExtra(EXTRAS_DEVICE_ADDRESS);

        //设置显示相关
        mDataField = (TextView)findViewById(R.id.uart_textView_receiveData);

        mSendDataCount = (TextView)findViewById(R.id.uart_textView_sendDataCount);
        mSendDataCount.setText(R.string.send_data);
        mSendDataCount.append("0");

        mReceiveDataCount = (TextView)findViewById(R.id.uart_textView_receiveDataCount);
        mReceiveDataCount.setText(R.string.receive_data);
        mReceiveDataCount.append("0");

        //设置EditText
        edtSend = (EditText)findViewById(R.id.uart_editText_sendData);

        //设置scrollView
        svResult = (ScrollView)findViewById(R.id.uart_scrollView_receive);

        //设置发送Button
        btnSend = (Button)findViewById(R.id.uart_button_send);
        btnSend.setEnabled(false);
        btnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (!mConnected) return;

                if (edtSend.length() < 1)
                {
                    Toast.makeText(Uart.this,R.string.input_send_content,Toast.LENGTH_SHORT).show();
                    return;
                }
                //通过蓝牙发送数据
                long count = edtSend.getText().length();
                //mBluetoothLeService.WriteValue(edtSend.getText().toString());
                send(edtSend.getText().toString());

                sendDataCount += count;
                mSendDataCount.setText(R.string.send_data);
                mSendDataCount.append(""+sendDataCount);

                //隐藏软件盘
                InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
                if(imm.isActive())
                    imm.hideSoftInputFromWindow(edtSend.getWindowToken(), 0);
            }
        });

        //得到BluetoothAdapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter.getState() != BluetoothAdapter.STATE_ON)
        {
            Toast.makeText(this,R.string.bluetooth_not_supported,Toast.LENGTH_SHORT).show();
            finish();
        }

        IntentFilter mIntent = new IntentFilter();
        mIntent.addAction(BluetoothDevice.ACTION_ACL_CONNECTED);
        mIntent.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);

        registerReceiver(connectDevices, mIntent);
        mHandler.sendEmptyMessageDelayed(MESSAGE_CONNECT, 1000);
    }

    public final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MESSAGE_CONNECT:
                    Toast.makeText(Uart.this, R.string.ble_connected_success, Toast.LENGTH_SHORT).show();
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
                    mDataField.setText("");
                    btnSend.setEnabled(true);
                    edtSend.setText(R.string.waveshare_data);
                    mMenu.findItem(R.id.uart_menu_clear).setVisible(true);
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
                    Toast.makeText(Uart.this, R.string.ble_connected_failed, Toast.LENGTH_SHORT).show();
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
                case MESSAGE_RECV:
                    byte[] bBuf = (byte[]) msg.obj;
                    String data = bytesToString(bBuf, msg.arg1);
                    if (data != null)
                    {
                        if (mDataField.length() > 5000)
                        {
                            mDataField.setText("");
                        }
                        //计数相关
                        long count = data.length();
                        receiveDataCount += count;
                        mReceiveDataCount.setText(R.string.receive_data);
                        mReceiveDataCount.append(""+receiveDataCount);

                        mDataField.append(data);
                        svResult.post(new Runnable() {
                            @Override
                            public void run() {
                                svResult.fullScroll(ScrollView.FOCUS_DOWN);
                            }
                        });
                    }
                    break;
            }
        }
    };

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
            Toast.makeText(Uart.this, R.string.ble_connected_failed, Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
    }

//    /* DEMO版较为简单，在编写您的应用时，请将此函数放到线程中执行，以免UI不响应 */
//    void send(final String strValue) {
//
//        new Thread(new Runnable() {
//            @Override
//            public void run() {
//
//                if (!mConnected)
//                    return;
//                try {
//                    if (mOutStream == null)
//                        return;
//                    mOutStream.write(strValue.getBytes());
//                } catch (Exception e) {
//                    Toast.makeText(Uart.this, R.string.ble_connected_failed, Toast.LENGTH_SHORT).show();
//                    finish();
//                    return;
//                }
//            }
//        }).start();
//
//    }

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

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        getMenuInflater().inflate(R.menu.uart_menu,menu);
        mMenu = menu;

        if (mConnected) {
            menu.findItem(R.id.uart_menu_clear).setVisible(true);
        } else {
            menu.findItem(R.id.uart_menu_clear).setVisible(false);
        }

        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        switch (item.getItemId())
        {
            case R.id.uart_menu_clear:
                sendDataCount = 0;
                mSendDataCount.setText(R.string.send_data);
                mSendDataCount.append(""+sendDataCount);

                receiveDataCount = 0;
                mReceiveDataCount.setText(R.string.receive_data);
                mReceiveDataCount.append(""+receiveDataCount);

                mDataField.setText("");

                invalidateOptionsMenu();
                return true;
        }

        return super.onOptionsItemSelected(item);
    }
}



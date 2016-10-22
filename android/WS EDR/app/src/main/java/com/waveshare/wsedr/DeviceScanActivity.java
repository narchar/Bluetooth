package com.waveshare.wsedr;

import android.app.Activity;
import android.app.ListActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;

public class DeviceScanActivity extends ListActivity {

    private BluetoothAdapter mBluetoothAdapter;
    //ListView Adapter
    private LeDeviceListAdapter mLeDeviceListAdapter;
    //是否处理扫描状态
    private boolean mScanning = false;

    //蓝牙是否打开
    public int turnBluetooth = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        //在Activity中设置ActionBar
        getActionBar().setTitle(R.string.actionBar_find_device);

        //得到BluetoothAdapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter.getState() != BluetoothAdapter.STATE_ON)
        {
            turnOnBluetooth(this,turnBluetooth);
            Log.i("saber","turnBluetooth"+turnBluetooth);
        }

        //设置listView Adapter
        mLeDeviceListAdapter = new LeDeviceListAdapter();
        setListAdapter(mLeDeviceListAdapter);

        //注册Receiver来获取蓝牙设备相关的结果
        IntentFilter intent = new IntentFilter();
        intent.addAction(BluetoothDevice.ACTION_FOUND); //用BroadcastReceiver来取得搜索结果
        intent.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(searchDevices, intent);
    }

    @Override
    protected void onPause() {
        super.onPause();
        scanLeDevice(false);
    }

    private BroadcastReceiver searchDevices = new BroadcastReceiver() {

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();

            if (action.equals(BluetoothDevice.ACTION_FOUND)) { //found device
                BluetoothDevice device = intent
                        .getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                mLeDeviceListAdapter.addDevice(device);
                mHandler.sendEmptyMessage(1);

            }
            else if (action
                    .equals(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {
                if (mScanning)
                {
                    mScanning = false;
                    //停止扫描
                    mBluetoothAdapter.cancelDiscovery();
                    Toast.makeText(DeviceScanActivity.this, "扫描完成，点击列表中的设备来尝试连接", Toast.LENGTH_SHORT).show();
                    mHandler.sendEmptyMessage(1);
                    invalidateOptionsMenu();
                }
            }
        }
    };

    //初始化时调用，建立menu
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.scan, menu);
        if (!mScanning) {
            menu.findItem(R.id.menu_stop).setVisible(false);
            menu.findItem(R.id.menu_scan).setVisible(true);
            menu.findItem(R.id.menu_refresh).setActionView(null);
        } else {
            menu.findItem(R.id.menu_stop).setVisible(true);
            menu.findItem(R.id.menu_scan).setVisible(false);
            menu.findItem(R.id.menu_refresh).setActionView(
                    R.layout.actionbar_indeterminate_progress);
        }
        return true;
    }

    //menu的选中回调函数
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId())
        {
            case R.id.menu_scan:
                mLeDeviceListAdapter.clear();
                scanLeDevice(true);
                break;
            case R.id.menu_stop:
                scanLeDevice(false);
                break;
        }
        return true;
    }

    //打开连接界面,item选中后的回调函数
    @Override
    protected void onListItemClick(ListView l, View v, int position, long id) {
        final BluetoothDevice device = mLeDeviceListAdapter.getDevice(position);

        if (device == null) return;

        if (mScanning)
        {
            //停止扫描
            mBluetoothAdapter.cancelDiscovery();;
            mScanning = false;
        }

        final Intent intent = new Intent(this, DeviceControlActivity.class);

        intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_NAME, device.getName());
        intent.putExtra(DeviceControlActivity.EXTRAS_DEVICE_ADDRESS, device.getAddress());

        startActivity(intent);
    }

    private void scanLeDevice(final boolean enable)
    {
        if (enable)
        {
            mScanning = true;
            mLeDeviceListAdapter.clear();
            mHandler.sendEmptyMessage(1);
            mBluetoothAdapter.startDiscovery();
        }
        else
        {
            mScanning = false;
            //停止扫描
            mBluetoothAdapter.cancelDiscovery();;
        }
        invalidateOptionsMenu();
    }

    void turnOnBluetooth(Activity activity,int requestCode)
    {
        Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
        startActivityForResult(enableBtIntent,requestCode);
    }
    // Hander
    public final Handler mHandler = new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what)
            {
                case 1:
                    mLeDeviceListAdapter.notifyDataSetChanged();
                    break;
            }
        }
    };
    //定义内部类
    private class LeDeviceListAdapter extends BaseAdapter {
        private ArrayList<BluetoothDevice> mLeDevices;
        //动态加载布局
        private LayoutInflater mInflator;

        public LeDeviceListAdapter()
        {
            super();
            mLeDevices = new ArrayList<BluetoothDevice>();
            mInflator = DeviceScanActivity.this.getLayoutInflater();
        }

        public void addDevice(BluetoothDevice device)
        {
            if (!mLeDevices.contains(device))
            {
                mLeDevices.add(device);
            }
        }

        public BluetoothDevice getDevice(int position)
        {
            return mLeDevices.get(position);
        }

        public void clear()
        {
            mLeDevices.clear();
        }

        @Override
        public int getCount()
        {
            return mLeDevices.size();
        }

        @Override
        public Object getItem(int position)
        {
            return mLeDevices.get(position);
        }

        @Override
        public long getItemId(int position)
        {
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent)
        {
            ViewHolder viewHolder;

            if (convertView == null)
            {
                convertView = mInflator.inflate(R.layout.listitem_device,null);
                viewHolder = new ViewHolder();
                viewHolder.deviceAddress = (TextView)convertView.findViewById(R.id.device_address);
                viewHolder.deviceName = (TextView)convertView.findViewById(R.id.device_name);
                convertView.setTag(viewHolder);
            }
            else
            {
                viewHolder = (ViewHolder) convertView.getTag();
            }

            BluetoothDevice device = mLeDevices.get(position);
            final String deviceName = device.getName();
            if ((deviceName != null) && (deviceName.length() > 0))
            {
                viewHolder.deviceName.setText(deviceName);
            }
            else
            {
                viewHolder.deviceName.setText(R.string.unknown_device);
            }
            viewHolder.deviceAddress.setText(device.getAddress());

            return convertView;
        }
    }

    static class ViewHolder
    {
        TextView deviceName;
        TextView deviceAddress;
    }
}

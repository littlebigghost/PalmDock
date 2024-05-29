package com.hong.palmdock;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Parcelable;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;

import com.hong.palmdock.adapters.ServerConfigAdapter;
import com.hong.palmdock.controls.DialogManager;
import com.hong.palmdock.controls.DialogView;
import com.hong.palmdock.models.DockItem;
import com.hong.palmdock.models.LinkServer;
import com.hong.palmdock.models.RequestModel;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private Context mContext;
    private SocketService mSocketService;
    private SocketService.LocalBinder mBinder;
    private int mItemClickedNumbers = 0;
    private ServiceConnection mServiceConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            mBinder = (SocketService.LocalBinder)iBinder;
            mSocketService = mBinder.getService();
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {

        }
    };
    private LinkedList<LinkServer> mServerList;
    private ServerConfigAdapter mAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d(TAG, "onCreate: ");
        mContext = MainActivity.this;

        Intent intent = new Intent(MainActivity.this, SocketService.class);
        bindService(intent, mServiceConnection, BIND_AUTO_CREATE);

        // 注册广播接收器
        IntentFilter filter = new IntentFilter();
        filter.addAction(SocketService.RECEIVE_MSG_ACTION);

        mServerList = new LinkedList<LinkServer>();
        importJson();
        ListView server_list_view = findViewById(R.id.serverListView);
        mAdapter = new ServerConfigAdapter(mServerList, mContext);
        server_list_view.setAdapter(mAdapter);

        server_list_view.setOnItemClickListener((adapterView, view, i, l) -> {
            if (mItemClickedNumbers++ >0)
                return;
            Log.d(TAG, "setOnItemClickListener: ");
            registerReceiver(broadcastReceiver, filter);
            LinkServer item = mServerList.get(i);
            new Thread(() -> {
                try {
                    mSocketService.connect(item.getIpAddr(), item.getPort());

                    sendMessage(new RequestModel(SendDateTypeEnum.PULL_CONFIG, 0).toJson());
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.d(TAG, "服务端未启动");
                    runOnUiThread(() -> {
                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setMessage("未能找到服务器，请稍后再试!!!");
                        builder.setPositiveButton("确定", null);
                        builder.create().show();
                    });
                }
                mItemClickedNumbers = 0;
            }).start();
        });
        server_list_view.setOnItemLongClickListener((adapterView, view, i, l) -> {
            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
            builder.setMessage("确定删除该项吗？");
            builder.setPositiveButton("确定", (dialogInterface, i1) -> {
                mServerList.remove(i);
                exportJson();
                mAdapter.notifyDataSetChanged();
            });
            builder.setNegativeButton("取消", null);
            builder.create().show();
            return true;
        });

        Button add_server_btn = findViewById(R.id.addServerBtn);
        add_server_btn.setOnClickListener(view -> {
            DialogView mAddServerDialog = DialogManager.getInstance().initView(this, R.layout.dialog_add_server_layout);
            TextView cancel_text_view = mAddServerDialog.findViewById(R.id.dialog_input_text_cancel);
            TextView determine_text_view = mAddServerDialog.findViewById(R.id.dialog_input_text_determine);
            cancel_text_view.setOnClickListener(view1 -> {
                DialogManager.getInstance().hide(mAddServerDialog);
            });
            determine_text_view.setOnClickListener(view1 -> {
                DialogManager.getInstance().hide(mAddServerDialog);
                // 添加确定逻辑
                String ipAddr = ((EditText)mAddServerDialog.findViewById(R.id.dialog_ip_address_edit_text)).getText().toString();
                int port = Integer.parseInt(((EditText)mAddServerDialog.findViewById(R.id.dialog_port_edit_text)).getText().toString());
                Log.d(TAG, "onCreate: " + ipAddr + ":" + port);

                mServerList.add(new LinkServer(ipAddr, port));
                mAdapter.notifyDataSetChanged();
                exportJson();

            });
            DialogManager.getInstance().show(mAddServerDialog);
        });

    }

    private void exportJson() {
        JSONArray rootArr = new JSONArray();
        for (LinkServer item :
                mServerList) {
            JSONObject obj = new JSONObject();
            try {
                obj.put("ip", item.getIpAddr());
                obj.put("port", item.getPort());
            } catch (JSONException e) {
                throw new RuntimeException(e);
            }
            rootArr.put(obj);
        }
        String fileName = "config.json";
        FileOutputStream outputStream;
        try {
            outputStream = openFileOutput(fileName, Context.MODE_PRIVATE);
            outputStream.write(rootArr.toString().getBytes());
            outputStream.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void importJson() {
        String fileName = "config.json";
        FileInputStream inputStream;
        try {
            inputStream = openFileInput(fileName);
            int size = inputStream.available();
            byte[] buffer = new byte[size];
            inputStream.read(buffer);
            inputStream.close();
            String jsonStr = new String(buffer, "UTF-8");

            JSONArray rootArr = new JSONArray(jsonStr);
            for (int i = 0; i < rootArr.length(); i++) {
                JSONObject obj = rootArr.getJSONObject(i);
                mServerList.add(new LinkServer(obj.getString("ip"), obj.getInt("port")));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }


    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(broadcastReceiver);
    }

    private void sendMessage(String message) {
        try {
            mSocketService.send(message);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // 在广播接收器中处理接收到的消息
    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String message = intent.getStringExtra("msg");
            // 处理接收到的消息
            Log.d(TAG, "onReceive: " + message);
            List<DockItem> dockItems = new ArrayList<>();
            try {
                JSONArray jsonArray = new JSONArray(message);
                for (int i=0; i < jsonArray.length(); i++) {
                    JSONObject jsonObject = jsonArray.getJSONObject(i);
                    String name = jsonObject.getString("name");
                    String icon_base64_str = jsonObject.getString("icon_data");
                    byte[] icon_bytes = Base64.decode(icon_base64_str, Base64.DEFAULT);
                    dockItems.add(new DockItem(name, icon_bytes));
                }
            } catch (JSONException e) {
                throw new RuntimeException(e);
            }

            Intent nextIntent = new Intent(MainActivity.this, OperateActivity.class);
            nextIntent.putParcelableArrayListExtra("dock_item_list", (ArrayList<? extends Parcelable>) dockItems);
            startActivity(nextIntent);
            unregisterReceiver(broadcastReceiver);
        }
    };

}
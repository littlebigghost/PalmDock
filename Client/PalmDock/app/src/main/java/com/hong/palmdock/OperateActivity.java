package com.hong.palmdock;

import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;

import androidx.appcompat.app.AppCompatActivity;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Parcelable;
import android.util.Base64;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import com.hong.palmdock.models.DockItem;
import com.hong.palmdock.models.RequestModel;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class OperateActivity extends AppCompatActivity {

    private static final String TAG = "OperateActivity";
    private List<DockItem> dockItems;
    private SocketService mSocketService;
    private SocketService.LocalBinder mBinder;

    private LinearLayout mBtnsLayout;

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
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate: ");
        setRequestedOrientation(SCREEN_ORIENTATION_LANDSCAPE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_operate);

        Intent intent = new Intent(OperateActivity.this, SocketService.class);
        bindService(intent, mServiceConnection, BIND_AUTO_CREATE);

        // 注册广播接收器
        IntentFilter filter = new IntentFilter();
        filter.addAction(SocketService.RECEIVE_MSG_ACTION);
        registerReceiver(broadcastReceiver, filter);

        dockItems = getIntent().getParcelableArrayListExtra("dock_item_list");

        initUI();
    }

    private void initUI() {
        Log.d(TAG, "initUI: ");
        // 设置seekbar
        FrameLayout brightness_frame_layout = findViewById(R.id.brightness_frame_layout);
        FrameLayout volume_frame_layout = findViewById(R.id.volume_frame_layout);
        com.hong.palmdock.controls.VerticalSeekBar brightness_seek_bar = findViewById(R.id.seek_bar_brightness);
        com.hong.palmdock.controls.VerticalSeekBar volume_seek_bar = findViewById(R.id.seek_bar_volume);
        brightness_frame_layout.post(() -> {
            ViewGroup.LayoutParams layoutParams = brightness_seek_bar.getLayoutParams();
            layoutParams.width = brightness_frame_layout.getHeight();
            brightness_seek_bar.setLayoutParams(layoutParams);
        });
        volume_frame_layout.post(() -> {
            ViewGroup.LayoutParams layoutParams = volume_seek_bar.getLayoutParams();
            layoutParams.width = volume_frame_layout.getHeight();
            volume_seek_bar.setLayoutParams(layoutParams);
        });
        brightness_seek_bar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                new Thread(() -> {
                    sendMessage(new RequestModel(SendDateTypeEnum.BRIGHTNESS, i).toJson());
                }).start();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        volume_seek_bar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                new Thread(() -> {
                    sendMessage(new RequestModel(SendDateTypeEnum.VOLUME, i).toJson());
                }).start();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                Log.d(TAG, "onStopTrackingTouch: " + String.valueOf(seekBar.getProgress()));
            }
        });

        mBtnsLayout = findViewById(R.id.dynamic_btns_layout);
        setBtnsLayout();
    }

    private void setBtnsLayout() {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        this.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int screenWidth = displayMetrics.widthPixels;
        int screenHeight = displayMetrics.heightPixels;

        LinearLayout horizontal_layout = null;
        LinearLayout.LayoutParams itemParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        itemParams.width = (int) (screenWidth * 0.7) / 4;
        itemParams.height = (int) (screenHeight * 0.8) / 2;

        for (int i = 1; i <= dockItems.size(); i++) {
            DockItem item = dockItems.get(i - 1);
            if (i % 4 - 1 == 0) {
                horizontal_layout = new LinearLayout(this);
                horizontal_layout.setOrientation(LinearLayout.HORIZONTAL);
                mBtnsLayout.addView(horizontal_layout);
            }
            LinearLayout item_layout = new LinearLayout(this);
            item_layout.setGravity(Gravity.CENTER);
            item_layout.setOrientation(LinearLayout.VERTICAL);
            item_layout.setLayoutParams(itemParams);
            ImageView icon_view = new ImageView(this);
            icon_view.setImageDrawable(new BitmapDrawable(getResources(), BitmapFactory.decodeByteArray(item.getIconBytes(), 0, item.getIconBytes().length)));
            TextView name_text = new TextView(this);
            name_text.setText(item.getName());
            name_text.setTextAlignment(View.TEXT_ALIGNMENT_CENTER);
            // 布局加载完，获取宽度
            item_layout.post(() -> {
                ViewGroup.LayoutParams icon_layout_params = icon_view.getLayoutParams();
                icon_layout_params.width = item_layout.getWidth() - 2 * 30;
                icon_layout_params.height = item_layout.getWidth() - 2 * 30;
                icon_view.setLayoutParams(icon_layout_params);
            });
            item_layout.addView(icon_view);
            item_layout.addView(name_text);

            int finalIndex = i;
            item_layout.setOnClickListener((view) ->
                    new Thread(() -> {
                        sendMessage(new RequestModel(SendDateTypeEnum.BUTTON_GROUP, finalIndex - 1).toJson());
                    }).start()
            );
            horizontal_layout.addView(item_layout);
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
            dockItems.clear();
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

            mBtnsLayout.removeAllViews();
            setBtnsLayout();
        }
    };
}
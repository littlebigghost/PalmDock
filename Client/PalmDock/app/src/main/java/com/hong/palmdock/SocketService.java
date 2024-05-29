package com.hong.palmdock;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

import androidx.annotation.Nullable;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.net.InetSocketAddress;
import java.net.Socket;

public class SocketService extends Service {
    private Socket mSocket;
    private InputStream mInputStream;
    private OutputStream mOutputStream;

    private static final String TAG = "SocketService";
    public static final String RECEIVE_MSG_ACTION = "RECEIVE_MSG_ACTION";

    private IBinder mBinder = new LocalBinder();

    public class LocalBinder extends Binder {
        public SocketService getService() {
            return SocketService.this;
        }
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    public void connect(String ip, int port) throws IOException {
        mSocket = new Socket();
        mSocket.connect(new InetSocketAddress(ip, port), 3000);
        mInputStream = mSocket.getInputStream();
        mOutputStream = mSocket.getOutputStream();

        new Thread(() -> {
            InputStreamReader inputStreamReader = null;
            try {
                inputStreamReader = new InputStreamReader(mInputStream, "UTF-8");
                BufferedReader reader = new BufferedReader(inputStreamReader);
                String line = null;
                while (true) {
                    line = reader.readLine();
                    if (!line.isEmpty()) {
                        Intent intent = new Intent(RECEIVE_MSG_ACTION);
                        intent.putExtra("msg", line);
                        sendBroadcast(intent);
                    }
                }
            } catch (UnsupportedEncodingException e) {
                throw new RuntimeException(e);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }

        }).start();
    }

    public void send(String message) throws IOException {
        if (mOutputStream != null) {
            mOutputStream.write(message.getBytes());
            mOutputStream.flush();
        }
    }
}

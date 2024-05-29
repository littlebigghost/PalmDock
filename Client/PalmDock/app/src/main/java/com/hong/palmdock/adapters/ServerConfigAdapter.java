package com.hong.palmdock.adapters;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.hong.palmdock.R;
import com.hong.palmdock.models.LinkServer;

import java.util.LinkedList;

public class ServerConfigAdapter extends BaseAdapter {
    private LinkedList<LinkServer> mData;
    private Context mContext;

    public ServerConfigAdapter(LinkedList<LinkServer> mData, Context mContext) {
        this.mData = mData;
        this.mContext = mContext;
    }

    @Override
    public int getCount() {
        return mData.size();
    }

    @Override
    public Object getItem(int i) {
        return mData.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        view = LayoutInflater.from(mContext).inflate(R.layout.sever_list_item, viewGroup, false);
        TextView ip_address_text = (TextView) view.findViewById(R.id.ip_address_text);
        TextView port_text = (TextView) view.findViewById(R.id.port_text);

        ip_address_text.setText(mData.get(i).getIpAddr());
        port_text.setText(String.valueOf(mData.get(i).getPort()));

        return view;
    }
}

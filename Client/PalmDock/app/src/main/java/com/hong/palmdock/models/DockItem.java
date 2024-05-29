package com.hong.palmdock.models;

import android.graphics.Bitmap;
import android.os.Parcel;
import android.os.Parcelable;

import androidx.annotation.NonNull;

public class DockItem implements Parcelable {
    private String mName;

    private byte[] mIconBytes;

    public DockItem() {
    }

    public DockItem(Parcel in) {
        mName = in.readString();
        mIconBytes = in.createByteArray();
    }

    public DockItem(String name, byte[] iconBytes) {
        this.mName = name;
        this.mIconBytes = iconBytes;
    }
    public static final Creator<DockItem> CREATOR = new Creator<DockItem>() {
        @Override
        public DockItem createFromParcel(Parcel in) {
            return new DockItem(in);
        }

        @Override
        public DockItem[] newArray(int size) {
            return new DockItem[size];
        }
    };

    public String getName() {
        return mName;
    }

    public void setName(String name) {
        this.mName = name;
    }

    public byte[] getIconBytes() {
        return mIconBytes;
    }

    public void setIconBytes(byte[] iconBytes) {
        this.mIconBytes = iconBytes;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(@NonNull Parcel parcel, int i) {
        parcel.writeString(mName);
        parcel.writeByteArray(mIconBytes);
    }
}

package com.hong.palmdock.models;

import com.hong.palmdock.SendDateTypeEnum;

import org.json.JSONException;
import org.json.JSONObject;

public class RequestModel {
    private SendDateTypeEnum mSendDateType;
    private int mValue;

    public RequestModel() {
    }

    public RequestModel(SendDateTypeEnum sendDateType, int mValue) {
        this.mSendDateType = sendDateType;
        this.mValue = mValue;
    }

    public SendDateTypeEnum getSendDateType() {
        return mSendDateType;
    }

    public void setSendDateType(SendDateTypeEnum sendDateType) {
        this.mSendDateType = sendDateType;
    }

    public int getValue() {
        return mValue;
    }

    public void setValue(int Value) {
        this.mValue = Value;
    }

    public String toJson(){
        JSONObject jsonObject = new JSONObject();
        try {
            jsonObject.put("type", mSendDateType.ordinal());
            jsonObject.put("value", mValue);
        } catch (JSONException e) {
            throw new RuntimeException(e);
        }

        return jsonObject.toString();
    }

}

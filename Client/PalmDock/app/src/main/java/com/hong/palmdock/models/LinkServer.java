package com.hong.palmdock.models;

public class LinkServer {
    private String mIpAddr;
    private int mPort;

    public LinkServer() {
    }

    public LinkServer(String ipAddr, int port) {
        this.mIpAddr = ipAddr;
        this.mPort = port;
    }

    public String getIpAddr() {
        return mIpAddr;
    }

    public void setIpAddr(String ipAddr) {
        this.mIpAddr = ipAddr;
    }

    public int getPort() {
        return mPort;
    }

    public void setPort(int port) {
        this.mPort = port;
    }


}

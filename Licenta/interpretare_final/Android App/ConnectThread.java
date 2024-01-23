package com.example.handsign;

import android.annotation.SuppressLint;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Handler;

import java.io.IOException;
import java.util.UUID;


public class ConnectThread extends Thread {
    private final BluetoothSocket mmSocket;
    private static Handler announcementHandler;
    private final static int ERROR_READ = 0;

    @SuppressLint("MissingPermission")
    ConnectThread(BluetoothDevice device, UUID MY_UUID, Handler announcementHandler) {
        BluetoothSocket tmp = null;
        ConnectThread.announcementHandler = announcementHandler;
        try {
            tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
        } catch (IOException e) {
            ConnectThread.announcementHandler.obtainMessage(ERROR_READ, "Socket's create() method failed").sendToTarget();
        }
        mmSocket = tmp;
    }

    @SuppressLint("MissingPermission")
    public void run() {
        try {
            mmSocket.connect();
        } catch (IOException connectException) {
            announcementHandler.obtainMessage(ERROR_READ, "Unable to connect to the BT device").sendToTarget();
            try {
                mmSocket.close();
            } catch (IOException closeException) {
                announcementHandler.obtainMessage(ERROR_READ, "Could not close the client socket").sendToTarget();
            }
        }
    }

    void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            announcementHandler.obtainMessage(ERROR_READ, "Could not close the client socket").sendToTarget();
        }
    }

    BluetoothSocket getMmSocket(){
        return mmSocket;
    }
}


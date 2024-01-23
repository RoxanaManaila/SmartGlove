package com.example.handsign;


import android.bluetooth.BluetoothSocket;
import android.os.Handler;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class ConnectedThread extends Thread {
    private static Handler announcementHandler;
    private final static int ERROR_READ = 0;
    private final BluetoothSocket mmSocket;
    private final InputStream mmInStream;
    private String valueRead;

    ConnectedThread(BluetoothSocket socket, Handler announcementHandler) {
        ConnectedThread.announcementHandler = announcementHandler;
        mmSocket = socket;
        InputStream tmpIn = null;
        try {
            tmpIn = socket.getInputStream();
        } catch (IOException e) {
            ConnectedThread.announcementHandler.obtainMessage(ERROR_READ, "Error occurred when creating input stream").sendToTarget();
        }
        mmInStream = tmpIn;
    }

    String getValueRead(){
        return valueRead;
    }

    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(mmInStream));
            String valueTemp = null;
            while (!"break".equals(valueTemp) && in.ready()) {
                valueTemp = in.readLine();
            }
            if (in.ready()) {
                valueRead = in.readLine();
            }
        } catch (IOException e) {
            announcementHandler.obtainMessage(ERROR_READ, "Input stream was disconnected").sendToTarget();
        }
    }

    void cancel() {
        try {
            mmSocket.close();
        } catch (IOException e) {
            announcementHandler.obtainMessage(ERROR_READ, "Could not close the connect socket").sendToTarget();
        }
    }
}
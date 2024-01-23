package com.example.handsign;


import android.Manifest;
import android.annotation.SuppressLint;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.util.Set;
import java.util.UUID;

@SuppressLint("SetTextI18n")
public class MainActivity extends AppCompatActivity {
    private static final int REQUEST_ENABLE_BT = 1;
    public static Handler announcementHandler;
    private Handler handler = new Handler();
    final int delay = 1000; // 1000 milliseconds == 1 second
    private final static int ERROR_READ = 0;
    BluetoothDevice arduinoBTModule = null;
    UUID arduinoUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    @RequiresApi(api = Build.VERSION_CODES.M)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        BluetoothManager bluetoothManager = getSystemService(BluetoothManager.class);
        final BluetoothAdapter bluetoothAdapter = bluetoothManager.getAdapter();
        final TextView btReadings = findViewById(R.id.btReadings);
        final TextView announcementText = findViewById(R.id.AnnouncementText);
        Button connectToDevice = findViewById(R.id.connectToDevice);

        announcementHandler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message msg) {
                if (msg.what == ERROR_READ) {
                    String arduinoMsg = msg.obj.toString();
                    announcementText.setText(arduinoMsg);
                }
            }
        };

        connectToDevice.setOnClickListener(new View.OnClickListener() {
            @SuppressLint("CheckResult")
            public void onClick(View view) {
                if (bluetoothAdapter == null) {
                    announcementText.setText("Device doesn't support Bluetooth");
                    return;
                }

                if (!bluetoothAdapter.isEnabled()) {
                    announcementText.setText("Bluetooth is disabled");
                    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    if (ActivityCompat.checkSelfPermission(getApplicationContext(), Manifest.permission.BLUETOOTH_PRIVILEGED) != PackageManager.PERMISSION_GRANTED) {
                        announcementText.setText("We don't BT Permissions");
                        startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
                    }
                    announcementText.setText("Try the connection again...");
                    return;
                }

                Set <BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
                if (pairedDevices.size() > 0) {
                    for (BluetoothDevice device : pairedDevices) {
                        String deviceName = device.getName();
                        if (deviceName.equals("HC-05")) {
                            arduinoUUID = device.getUuids()[0].getUuid();
                            arduinoBTModule = device;

                            btReadings.setText("");

                            if (arduinoBTModule != null) {
                                announcementText.setText("Pending Connection...");
                                final ConnectThread connectThread = new ConnectThread(arduinoBTModule, arduinoUUID, announcementHandler);
                                connectThread.run();
                                if (connectThread.getMmSocket().isConnected()) {
                                    handler.post(new Runnable() {
                                        public void run() {
                                            final ConnectedThread connectedThread = new ConnectedThread(connectThread.getMmSocket(), announcementHandler);
                                            connectedThread.run();
                                            if (connectedThread.getValueRead() != null) {
                                                announcementText.setText("Connected");
                                                btReadings.setText(connectedThread.getValueRead());
                                            }
                                            handler.postDelayed(this, delay);
                                        }
                                    });
                                }
                            } else {
                                announcementText.setText("HM-05 (Arduino) disconnected or an unexpected error happened.");
                            }
                            return;
                        }
                    }
                }
                announcementText.setText("HM-05 (Arduino) not found within the paired devices");
            }
        });
    }
}


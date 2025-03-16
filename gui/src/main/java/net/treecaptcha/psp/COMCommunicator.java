package net.treecaptcha.psp;
import com.fazecast.jSerialComm.*;

import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.HexFormat;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class COMCommunicator extends Thread implements RocketCommunicator {
    private OnDataReceive ondataReceive;
    private SerialPort serialPort;
    private long lastConnectTime;
    COMCommunicator(OnDataReceive ondataReceive, SerialPort serialPort) {
        this.ondataReceive = ondataReceive;
        this.serialPort = serialPort;
        serialPort.setComPortParameters(38400, 8, 1, 0);
        serialPort.openPort();
        this.start();
    }
    @Override
    public void setOnDataReceive(OnDataReceive ondatareceive) {
        this.ondataReceive = ondatareceive;
    }
    @Override
    public void transmit(byte[] data) {
        serialPort.writeBytes(data, data.length);
    }
    @Override
    public boolean hasConnection(){
        return serialPort.isOpen();
    }
    @Override
    public int millisSinceLastConnection() {
        return (int) (System.currentTimeMillis() - lastConnectTime);
    }

    @Override
    public void run() {
        InputStream inputStream = serialPort.getInputStream();
        serialPort.setComPortTimeouts(SerialPort.TIMEOUT_READ_BLOCKING, 1, 0);
        Scanner scanner = new Scanner(inputStream);
        while (true){
            while (serialPort.bytesAvailable() >= 2) {
                try {
                    String data = scanner.nextLine();
                    if (data.startsWith("0") && data.length() % 2 == 0)
                        ondataReceive.onDataReceive(HexFormat.of().parseHex(data));
                    else
                        System.out.println("Received bad data: " + data);
                    lastConnectTime = System.currentTimeMillis();
                }
                catch (NoSuchElementException e) {
                }
            }
            try {
                Thread.sleep(1);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}

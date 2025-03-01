package net.treecaptcha.psp;

import com.fazecast.jSerialComm.SerialPort;
import net.treecaptcha.psp.decoder.Decoder;
import net.treecaptcha.psp.decoder.PacketData;
import net.treecaptcha.psp.decoder.PacketDecodeException;

import java.util.Arrays;
import java.util.List;

public class PSPControllerGUI implements OnDataReceive, OnCommandReceive {
    MainWindow mainWindow;
    RocketCommunicator rocketComunicator;
    Decoder decoder;
    private SerialPort[] commPorts;
    public PSPControllerGUI() {
        mainWindow = new MainWindow(this);
        decoder = new Decoder();
        listPorts();
    }
    public void onDataReceive(byte[] data){
        try {
            List<PacketData> packetDatas = decoder.decodeAll(data);
            for (PacketData dataPacket : packetDatas) {
                mainWindow.update(dataPacket);
            }
        } catch (PacketDecodeException e) {
            mainWindow.displayLog("Error decoding packet: " + e.getMessage());
        }
    }
    public static void main(String[] args) {
        PSPControllerGUI pspControllerGUI = new PSPControllerGUI();
    }

    @Override
    public void onCommand(String command) {
        mainWindow.displayLog(command);
        String[] segs = command.split(" ");
        String[] args = Arrays.copyOfRange(segs, 1, segs.length);
        switch (segs[0]) {
            case "com":
                comStuff(args);
        }
    }
    private void comStuff(String[] args){
        if (args.length == 0){ mainWindow.displayLog("list | connect {n}"); return; }
        switch (args[0]) {
            case "list":
                listPorts();
                break;
            case "connect":
                connect(args);
                break;
            default:
                mainWindow.displayLog("Unknown subcommand: " + args[0]);
        }
    }

    private void listPorts() {
        commPorts = SerialPort.getCommPorts();
        if (commPorts.length == 0) mainWindow.displayLog("No ports available");
        int n = 0;
        for (SerialPort port : commPorts) {
            mainWindow.displayLog(n + ": " + port.getSystemPortName());
            n++;
        }
    }

    private void connect(String[] args) {
        if (args.length == 1) { mainWindow.displayLog("Not enough arguments for connect: expected 1 got 0."); return; }
        try {
            int n = Integer.parseInt(args[1]);
            rocketComunicator = new COMCommunicator(this, commPorts[n]);
        }
        catch(NumberFormatException e) {
            mainWindow.displayLog(args[1] + " is not a valid port number");
        }
        catch (IndexOutOfBoundsException e) {
            mainWindow.displayLog(args[1] + " is not a valid port number");
        }
    }
}

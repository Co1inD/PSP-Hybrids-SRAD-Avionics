package net.treecaptcha.psp;

import net.treecaptcha.psp.decoder.Decoder;

public class PSPControllerGUI implements OnDataReceive, OnCommandReceive {
    MainWindow mainWindow;
    RocketCommunicator rocketComunicator;
    Decoder decoder;
    public PSPControllerGUI() {
        mainWindow = new MainWindow();
        rocketComunicator = new COMCommunicator(this, "com1");
        decoder = new Decoder();
    }
    public void onDataReceive(byte[] data){
        // decoder.decodeAll(data);
    }
    public static void main(String[] args) {
        PSPControllerGUI pspControllerGUI = new PSPControllerGUI();
    }

    @Override
    public void onCommand(String command) {

    }
}

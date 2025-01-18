package net.treecaptcha.psp;

import net.treecaptcha.psp.decoder.Decoder;

public class PSPControllerGUI implements OndataReceive{
    MainWindow mainWindow;
    RocketComunicator rocketComunicator;
    Decoder decoder;
    public PSPControllerGUI() {
        mainWindow = new MainWindow();
        rocketComunicator = new RocketComunicator(this);
        decoder = new Decoder();
    }
    public void onDataReceive(byte[] data){
        // decoder.decodeAll(data);
    }
    public static void main(String[] args) {
        PSPControllerGUI pspControllerGUI = new PSPControllerGUI();
    }
}

package net.treecaptcha.psp;

public class COMCommunicator implements RocketCommunicator {
    private OnDataReceive ondataReceive;
    COMCommunicator(OnDataReceive ondataReceive, String port) {
        this.ondataReceive = ondataReceive;
    }
    @Override
    public void setOnDataReceive(OnDataReceive ondatareceive) {
        this.ondataReceive = ondatareceive;
    }
    @Override
    public void transmit(Byte[] data) {

    }
    @Override
    public boolean hasConnection(){
        return false;
    }
    @Override
    public int millisSinceLastConnection() {
        return Integer.MAX_VALUE;
    }
}

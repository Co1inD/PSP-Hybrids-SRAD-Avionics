package net.treecaptcha.psp;

public class RocketComunicator {
    private OndataReceive ondataReceive;
    RocketComunicator(OndataReceive ondataReceive) {
        this.ondataReceive = ondataReceive;
    }
    public void setOnDataReceive(OndataReceive ondatareceive) {
        this.ondataReceive = ondatareceive;
    }
    public void transmit(Byte[] data) {

    }
    public boolean hasConnection(){
        return false;
    }
    public int millisSinceLastConnection() {
        return Integer.MAX_VALUE;
    }
}

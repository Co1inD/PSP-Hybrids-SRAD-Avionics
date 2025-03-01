package net.treecaptcha.psp;

public interface RocketCommunicator {
    void setOnDataReceive(OnDataReceive ondatareceive);

    void transmit(byte[] data);

    boolean hasConnection();

    int millisSinceLastConnection();
}

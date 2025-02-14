package net.treecaptcha.psp;

public interface RocketCommunicator {
    void setOnDataReceive(OnDataReceive ondatareceive);

    void transmit(Byte[] data);

    boolean hasConnection();

    int millisSinceLastConnection();
}

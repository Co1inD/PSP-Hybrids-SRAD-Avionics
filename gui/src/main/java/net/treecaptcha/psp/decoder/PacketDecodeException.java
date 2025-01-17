package net.treecaptcha.psp.decoder;

/**
 * Exception that is thrown when packet cannot be decoded
 */
public class PacketDecodeException extends Exception{
    public PacketDecodeException(){}
    public PacketDecodeException(String message){
        super(message);
    }
    public PacketDecodeException(Throwable cause){
        super(cause);
    }
    public PacketDecodeException(String message, Throwable cause){
        super(message, cause);
    }
}

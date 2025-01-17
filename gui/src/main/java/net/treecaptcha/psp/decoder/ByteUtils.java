package net.treecaptcha.psp.decoder;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Arrays;

class ByteUtils {
    public static int intFromBytes(byte[] bytes, int offset){
        byte[] copiedBytes = Arrays.copyOfRange(bytes, offset, offset + 4);
        ByteBuffer buffer = ByteBuffer.wrap(copiedBytes).order(ByteOrder.LITTLE_ENDIAN);
        return buffer.getInt();
    }

    /**
     * Takes 4 bytes from the input array starting at offset that represent a IEEE 754-1985 single precision float and decodes them into a float
     *
     * @param bytes  The source array
     * @param offset The location of the first byte
     * @return The float value of the 4 bytes
     */
    public static float floatFromBytes(byte[] bytes, int offset) {
        byte[] copiedBytes = Arrays.copyOfRange(bytes, offset, offset + 4);
        ByteBuffer buffer = ByteBuffer.wrap(copiedBytes).order(ByteOrder.LITTLE_ENDIAN);
        return buffer.getFloat();
    }
}

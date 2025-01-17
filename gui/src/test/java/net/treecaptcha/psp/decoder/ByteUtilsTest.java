package net.treecaptcha.psp.decoder;

import static java.lang.Float.intBitsToFloat;
import static org.junit.jupiter.api.Assertions.assertEquals;
class ByteUtilsTest {

    @org.junit.jupiter.api.Test
    void intFromBytes() {
        assertEquals(6, ByteUtils.intFromBytes(new byte[]{0x06, 0x00, 0x00, 0x00}, 0));
        assertEquals(0x0A0B0C0D, ByteUtils.intFromBytes(new byte[]{0x12,0x0D, 0x0C, 0x0B, 0x0A}, 1));
    }

    @org.junit.jupiter.api.Test
    void floatFromBytes() {
        assertEquals(intBitsToFloat(0x40490FD2), ByteUtils.floatFromBytes(new byte[]{(byte) 0xD2, (byte) 0x0F, (byte) 0x49, (byte) 0x40}, 0));
        assertEquals(Float.NEGATIVE_INFINITY, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0xFF}, 0));
        assertEquals(Float.POSITIVE_INFINITY, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0x7F}, 0));
        assertEquals(Float.NaN, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0xC0, (byte) 0x7F}, 0));
        assertEquals(0.0f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00}, 0));
        assertEquals(-0.0f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x80}, 0));
        assertEquals(1.0f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0x3F}, 0));
        assertEquals(-1.0f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0xBF}, 0));
        assertEquals(3.4028235E38f, ByteUtils.floatFromBytes(new byte[]{(byte) 0xFF, (byte) 0xFF, (byte) 0x7F, (byte) 0x7F}, 0)); // Max float
        assertEquals(-3.4028235E38f, ByteUtils.floatFromBytes(new byte[]{(byte) 0xFF, (byte) 0xFF, (byte) 0x7F, (byte) 0xFF}, 0)); // Min float
        assertEquals(1.4E-45f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x00}, 0)); // Smallest positive subnormal
        assertEquals(-1.4E-45f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x80}, 0)); // Smallest negative subnormal
        assertEquals(1.17549435E-38f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0x00}, 0)); // Minimum positive normal
        assertEquals(-1.17549435E-38f, ByteUtils.floatFromBytes(new byte[]{(byte) 0x00, (byte) 0x00, (byte) 0x80, (byte) 0x80}, 0)); // Minimum negative normal
    }
}
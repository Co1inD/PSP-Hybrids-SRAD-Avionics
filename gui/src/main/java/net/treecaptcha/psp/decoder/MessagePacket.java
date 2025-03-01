package net.treecaptcha.psp.decoder;

import java.nio.charset.StandardCharsets;

public class MessagePacket implements PacketData {
    public int length;
    public String text;
    public MessagePacket(byte[] data, int offset) throws PacketDecodeException {
        if (data[offset] != 0b10) throw new PacketDecodeException("Type not marked as Log Packet");
        int txtLength = ByteUtils.intFromBytes(data, offset + 1);
        text = new String(data, offset + 5, txtLength, StandardCharsets.US_ASCII);
        length = txtLength + 5;
        System.out.println("Length: " + length);
    }

    @Override
    public int length() {
        return length;
    }
}

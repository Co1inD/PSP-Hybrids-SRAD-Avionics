package net.treecaptcha.psp.decoder;

import java.nio.charset.StandardCharsets;

public class MessagePacket implements PacketData {
    public int length;
    public String text;
    public MessagePacket(byte[] data, int offset) throws PacketDecodeException {
        if (data[offset] != 0b10) throw new PacketDecodeException("Type not marked as Log Packet");
        length = ByteUtils.intFromBytes(data, offset + 1);
        text = new String(data, offset + 5, length, StandardCharsets.US_ASCII);
    }
}

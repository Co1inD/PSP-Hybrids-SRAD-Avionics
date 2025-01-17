package net.treecaptcha.psp.decoder;

import java.util.ArrayList;
import java.util.List;

public class Decoder {
    public List<PacketData> decodeAll(byte[] data) throws PacketDecodeException {
        List<PacketData> list = new ArrayList<>();
        int offset = 0;
        while (offset < data.length) {
            PacketData packetData = decode(data, offset);
            list.add(packetData);
            offset += packetData.length;
        }
        return list;
    }
    private PacketData decode(byte[] data, int offset) throws PacketDecodeException {
        if (data[offset] == 0b01){
            return new UpdatePacket(data, offset);
        }
        else if (data[offset] == 0b10){
            throw new PacketDecodeException("Unable to proccess message packet, check back later");
        }
        throw new PacketDecodeException("Unknown packet type: " + data[offset]);
    }
}

package net.treecaptcha.psp.decoder;
import net.treecaptcha.psp.decoder.ByteUtils;
public class UpdatePacket implements PacketData{
    public final int length = 42;
    public final boolean isReady;
    public final boolean hasLaunched;
    public final boolean hasEndedBoost;
    public final boolean hasLaunchedFirstParachute;
    public final boolean hasLaunchedSecondParachute;

    public final int timeSinceSync;

    public final float AccelerationN;
    public final float AccelerationE;
    public final float AccelerationU;

    public final float VelocityN;
    public final float VelocityE;
    public final float VelocityU;

    public final float PositionN;
    public final float PositionE;
    public final float PositionU;

    public UpdatePacket(byte[] data, int offset) throws PacketDecodeException {
        if (data.length - offset >= 42) throw new PacketDecodeException("Buffer Too Small for type UpdatePacket");
        if (data[offset] != 0b01) throw new PacketDecodeException("Type not marked as UpdatePacket");
        byte status = data[offset + 1];
        isReady                    = 0!=(status & 0b00001);
        hasLaunched                = 0!=(status & 0b00010);
        hasEndedBoost              = 0!=(status & 0b00100);
        hasLaunchedFirstParachute  = 0!=(status & 0b01000);
        hasLaunchedSecondParachute = 0!=(status & 0b10000);

        timeSinceSync = ByteUtils.intFromBytes(data, offset + 2);

        AccelerationN = ByteUtils.floatFromBytes(data, offset + 6);
        AccelerationE = ByteUtils.floatFromBytes(data, offset + 10);
        AccelerationU = ByteUtils.floatFromBytes(data, offset + 14);

        VelocityN = ByteUtils.floatFromBytes(data, offset + 18);
        VelocityE = ByteUtils.floatFromBytes(data, offset + 22);
        VelocityU = ByteUtils.floatFromBytes(data, offset + 26);

        PositionN = ByteUtils.floatFromBytes(data, offset + 30);
        PositionE = ByteUtils.floatFromBytes(data, offset + 34);
        PositionU = ByteUtils.floatFromBytes(data, offset + 38);


    }

}

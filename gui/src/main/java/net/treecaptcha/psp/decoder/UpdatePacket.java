package net.treecaptcha.psp.decoder;

public class UpdatePacket implements PacketData{
    public final int length = 42;
    public final boolean isReady;
    public final boolean hasLaunched;
    public final boolean hasEndedBoost;
    public final boolean hasLaunchedFirstParachute;
    public final boolean hasLaunchedSecondParachute;

    public final int time;

    public final float accelerationN;
    public final float accelerationE;
    public final float accelerationU;

    public final float velocityN;
    public final float velocityE;
    public final float velocityU;

    public final float positionN;
    public final float positionE;
    public final float positionU;

    public final float polar;
    public final float azimuth;


    public UpdatePacket(byte[] data, int offset) throws PacketDecodeException {
        if (data.length - offset >= 50) throw new PacketDecodeException("Buffer Too Small for type UpdatePacket");
        if (data[offset] != 0b01) throw new PacketDecodeException("Type not marked as UpdatePacket");
        byte status = data[offset + 1];
        isReady                    = 0!=(status & 0b00001);
        hasLaunched                = 0!=(status & 0b00010);
        hasEndedBoost              = 0!=(status & 0b00100);
        hasLaunchedFirstParachute  = 0!=(status & 0b01000);
        hasLaunchedSecondParachute = 0!=(status & 0b10000);

        time = ByteUtils.intFromBytes(data, offset + 2);

        accelerationN = ByteUtils.floatFromBytes(data, offset + 6);
        accelerationE = ByteUtils.floatFromBytes(data, offset + 10);
        accelerationU = ByteUtils.floatFromBytes(data, offset + 14);

        velocityN = ByteUtils.floatFromBytes(data, offset + 18);
        velocityE = ByteUtils.floatFromBytes(data, offset + 22);
        velocityU = ByteUtils.floatFromBytes(data, offset + 26);

        positionN = ByteUtils.floatFromBytes(data, offset + 30);
        positionE = ByteUtils.floatFromBytes(data, offset + 34);
        positionU = ByteUtils.floatFromBytes(data, offset + 38);

        polar = ByteUtils.floatFromBytes(data, offset + 42);
        azimuth = ByteUtils.floatFromBytes(data, offset + 46);
    }

}

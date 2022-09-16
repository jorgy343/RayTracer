using System.Runtime.InteropServices;

namespace Yart.Client;

public static unsafe class Native
{
    [DllImport("Yart.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern int TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, uint subpixelCount, uint iterations, float* pixelBuffer);
}

[StructLayout(LayoutKind.Sequential, Pack = 1)]
public struct UIntVector2
{
    public uint X;
    public uint Y;

    public UIntVector2()
    {
        X = 0;
        Y = 0;
    }

    public UIntVector2(uint x, uint y)
    {
        X = x;
        Y = y;
    }
}
using System.Runtime.InteropServices;

namespace Yart.ConsoleClient;

public static unsafe class Native
{
    [DllImport("yart_engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern void create_scene();

    [DllImport("Yart.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern void* CreateScene();

    [DllImport("Yart.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern void DeleteScene(void* sceneData);

    [DllImport("Yart.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern void TraceScene(UIntVector2 screenSize, UIntVector2 inclusiveStartingPoint, UIntVector2 inclusiveEndingPoint, void* sceneData, float* pixelBuffer);
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

    public override string ToString()
    {
        return $"{{{X}, {Y}}}";
    }
}
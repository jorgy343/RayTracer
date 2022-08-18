using System.Runtime.InteropServices;

namespace RayTracer.ConsoleClient;

public static unsafe class Native
{
    [DllImport("RayTracer.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern int TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer);
}
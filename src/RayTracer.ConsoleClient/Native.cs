using System.Runtime.InteropServices;

namespace RayTracer.ConsoleClient;

public static unsafe class Native
{
    [DllImport("RayTracer.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern int TraceScene(int screenWidth, int screenHeight, int startingX, int startingY, int endingX, int endingY, int subpixelCount, int iterations, float* pixelBuffer);
}
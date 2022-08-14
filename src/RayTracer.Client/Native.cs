using System.Runtime.InteropServices;

namespace RayTracer.Client;

public class Native
{
    [DllImport("RayTracer.Engine", CallingConvention = CallingConvention.Cdecl)]
    public static extern int TestFunction(int a, int b);
}
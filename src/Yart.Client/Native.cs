﻿using System.Runtime.InteropServices;

namespace Yart.Client;

public static unsafe class Native
{
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
}
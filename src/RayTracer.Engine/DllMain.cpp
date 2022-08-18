#include "dllmain.h"

using namespace RayTracer;

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

extern "C" __declspec(dllexport) void TraceScene(int startingX, int startingY, int width, int height, float* pixelBuffer)
{
	auto perspectiveCamera = PerspectiveCamera(
		Vector3(0, 0, 0),
		Vector3(1, 0, 0),
		Vector3(0, 1, 0),
		90.0f,
		800,
		600);

	auto rayBuffer = std::make_unique<Ray[]>(width * height);
	perspectiveCamera.CreateRays(0, 0, 800, 600, rayBuffer.get());

	for (auto y = 0; y < height; y++)
	{
		for (auto x = 0; x < width; x++)
		{
			pixelBuffer[((y * width) + x) * 4 + 0] = rayBuffer[(y * width) + x].Direction.X;
			pixelBuffer[((y * width) + x) * 4 + 1] = rayBuffer[(y * width) + x].Direction.Y;
			pixelBuffer[((y * width) + x) * 4 + 2] = rayBuffer[(y * width) + x].Direction.Z;
			pixelBuffer[((y * width) + x) * 4 + 3] = 0.0f;
		}
	}
}
#pragma once

#include "Ray.h"

using namespace RayTracer;

namespace RayTracer
{
    class PerspectiveCamera
    {
    public:
        Vector3 Position;
        Vector3 LookAt;
        Vector3 Up;

        float FieldOfView;

        int ScreenWidth;
        int ScreenHeight;

        PerspectiveCamera(
            const Vector3& position,
            const Vector3& lookAt,
            const Vector3& up,
            float fieldOfView,
            int screenWidth,
            int screenHeight);

        void CreateRays(int startingX, int startingY, int width, int height, Ray* rayBuffer);
    };
}
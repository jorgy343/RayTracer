#include <cmath>

export module RayTracer.PerspectiveCamera;

import RayTracer.Vector3;
import RayTracer.Ray;

namespace RayTracer
{
    export class PerspectiveCamera
    {
    public:
        Vector3 Position{};
        Vector3 LookAt{};
        Vector3 Up{};

        float FieldOfView{0.0f};

        int ScreenWidth{0};
        int ScreenHeight{0};

        PerspectiveCamera(
            const Vector3& position,
            const Vector3& lookAt,
            const Vector3& up,
            float fieldOfView,
            int screenWidth,
            int screenHeight)
            :
            Position{position},
            LookAt{lookAt},
            Up{up},
            FieldOfView{fieldOfView},
            ScreenWidth{screenWidth},
            ScreenHeight{screenHeight}
        {

        }

        void CreateRays(int startingX, int startingY, int width, int height, Ray* rayBuffer)
        {
            Vector3 direction = (LookAt - Position).Normalize();

            Vector3 du = -(Up % direction).Normalize();
            Vector3 dv = -(du % direction).Normalize();

            float halfWidth = tanf(FieldOfView / 2.0f);
            float halfHeight = (ScreenHeight / ScreenWidth) * halfWidth;

            du *= 2.0f * halfWidth / ScreenWidth;
            dv *= 2.0f * halfWidth / ScreenHeight;

            int index = 0;
            for (int y = startingY; y < startingY + height; ++y)
            {
                for (int x = startingX; x < startingX + width; ++x)
                {
                    // Gives us the upper left coordinates of the pixel.
                    float pixelIndexX = x - 0.5f * ScreenWidth;
                    float pixelIndexY = y - 0.5f * ScreenHeight;

                    // Takes the upper left coordinates of the pixel and add an offset which will give us the coordinates somewhere inside
                    // of the pixel. Multiply that by the delta direction to get the actual direction for the pixel.
                    Vector3 u = (pixelIndexX + 0.5f) * du;
                    Vector3 v = (pixelIndexY + 0.5f) * dv;

                    rayBuffer[index++] = Ray{Position, (direction + u + v).Normalize()};
                }
            }
        }
    };
}
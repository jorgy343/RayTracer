#include <cmath>

export module RayTracer.PerspectiveCamera;

import RayTracer.Math;
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
            Vector3 forward = (Position - LookAt).Normalize();

            Vector3 u = (Up % forward).Normalize();
            Vector3 v = (forward % u).Normalize();

            float aspectRatio = static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);
            float halfWidth = tanf(FieldOfView * 0.5f);

            float viewportHeight = halfWidth * 2.0f;
            float viewportWidth = aspectRatio * viewportHeight;

            Vector3 du = viewportWidth * u;
            Vector3 dv = viewportHeight * v;

            Vector3 upperLeftCorner = Position - du * 0.5f + dv * 0.5f - forward;

            float recipricalWidth = FastReciprical(static_cast<float>(width));
            float recpiricalHeight = FastReciprical(static_cast<float>(height));

            int index = 0;
            for (int y = startingY; y < startingY + height; ++y)
            {
                for (int x = startingX; x < startingX + width; ++x)
                {
                    float normalizedX = (x + static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * recipricalWidth;
                    float normalizedY = (y + static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * recpiricalHeight;

                    Vector3 rayDirection = upperLeftCorner + (normalizedX * du) - (normalizedY * dv) - Position;

                    rayBuffer[index++] = Ray{Position, rayDirection.Normalize()};
                }
            }
        }
    };
}
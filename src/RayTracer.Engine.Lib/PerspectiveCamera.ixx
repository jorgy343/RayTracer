#include <cmath>

export module RayTracer.PerspectiveCamera;

import RayTracer.Math;
import RayTracer.Vector3;
import RayTracer.Ray;
import RayTracer.Random;

namespace RayTracer
{
    export class PerspectiveCamera
    {
    private:
        Random _random{};

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

        void CreateRays(int startingX, int startingY, int width, int height, int subpixelCount, Ray* rayBuffer)
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
            float recipricalHeight = FastReciprical(static_cast<float>(height));

            float subpixelSizeX = FastReciprical(static_cast<float>(subpixelCount)) * recipricalWidth;
            float subpixelSizeY = FastReciprical(static_cast<float>(subpixelCount)) * recipricalHeight;

            int index = 0;
            for (int y = startingY; y < startingY + height; ++y)
            {
                for (int x = startingX; x < startingX + width; ++x)
                {
                    for (int subpixelX = 0; subpixelX < subpixelCount; subpixelX++)
                    {
                        for (int subpixelY = 0; subpixelY < subpixelCount; subpixelY++)
                        {
                            float normalizedX = x * recipricalWidth;
                            float normalizedY = y * recipricalHeight;

                            normalizedX += static_cast<float>(subpixelX) * subpixelSizeX;
                            normalizedY += static_cast<float>(subpixelY) * subpixelSizeY;

                            float r1 = _random.GetNormalizedFloat();
                            float r2 = _random.GetNormalizedFloat();

                            normalizedX += _random.GetNormalizedFloat() * subpixelSizeX;
                            normalizedY += _random.GetNormalizedFloat() * subpixelSizeY;

                            Vector3 rayDirection = upperLeftCorner + (normalizedX * du) - (normalizedY * dv) - Position;

                            rayBuffer[index++] = Ray{Position, rayDirection.Normalize()};
                        }
                    }
                }
            }
        }
    };
}
module;

export module PerspectiveCamera;

import <cmath>;
import <vector>;

import Camera;
import Math;
import Random;
import Ray;
import Vector2;
import Vector3;

namespace Yart
{
    export class PerspectiveCamera : public Camera
    {
    private:
        float _recipricalWidth{};
        float _recipricalHeight{};

        float _subpixelSizeX{};
        float _subpixelSizeY{};

        Vector3 _du{};
        Vector3 _dv{};

        Vector3 _upperLeftCorner{};

    public:
        Vector3 Position{};
        Vector3 LookAt{};
        Vector3 Up{};

        UIntVector2 ScreenSize{};
        float FieldOfView{};

        inline constexpr PerspectiveCamera(
            const Vector3& position,
            const Vector3& lookAt,
            const Vector3& up,
            unsigned int subpixelCount,
            const UIntVector2& screenSize,
            float fieldOfView)
            :
            Camera{subpixelCount},
            Position{position},
            LookAt{lookAt},
            Up{up},
            ScreenSize{screenSize},
            FieldOfView{fieldOfView}
        {
            Vector3 forward = (Position - LookAt).Normalize();

            Vector3 u = (Up % forward).Normalize();
            Vector3 v = (forward % u).Normalize();

            float aspectRatio = static_cast<float>(ScreenSize.X) / static_cast<float>(ScreenSize.Y);
            float halfWidth = Math::tan(FieldOfView * 0.5f);

            float viewportHeight = halfWidth * 2.0f;
            float viewportWidth = aspectRatio * viewportHeight;

            _du = viewportWidth * u;
            _dv = viewportHeight * v;

            _upperLeftCorner = Position - _du * 0.5f + _dv * 0.5f - forward;

            _recipricalWidth = Math::rcp(static_cast<float>(ScreenSize.X));
            _recipricalHeight = Math::rcp(static_cast<float>(ScreenSize.Y));

            _subpixelSizeX = Math::rcp(static_cast<float>(subpixelCount)) * _recipricalWidth;
            _subpixelSizeY = Math::rcp(static_cast<float>(subpixelCount)) * _recipricalHeight;
        }

        Ray CreateRay(UIntVector2 pixel, UIntVector2 subpixel, const Random& random) const override
        {
            float normalizedX = static_cast<float>(ScreenSize.X - pixel.X - 1) * _recipricalWidth;
            float normalizedY = static_cast<float>(pixel.Y) * _recipricalHeight;

            normalizedX += static_cast<float>(subpixel.X) * _subpixelSizeX;
            normalizedY += static_cast<float>(subpixel.Y) * _subpixelSizeY;

            normalizedX += random.GetNormalizedFloat() * _subpixelSizeX;
            normalizedY += random.GetNormalizedFloat() * _subpixelSizeY;

            Vector3 rayDirection = _upperLeftCorner + (normalizedX * _du) - (normalizedY * _dv) - Position;
            return {Position, rayDirection.Normalize()};
        }
    };
}
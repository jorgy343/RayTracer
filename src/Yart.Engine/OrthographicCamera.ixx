export module OrthographicCamera;

import <cmath>;
import <vector>;

import "Common.h";

import Camera;
import Math;
import Random;
import Ray;
import Vector2;
import Vector3;

namespace Yart
{
    export template <real_number T = real, real_number TOutput = real>
        class OrthographicCamera : public Camera
    {
    private:
        T _recipricalWidth{};
        T _recipricalHeight{};

        T _subpixelSizeX{};
        T _subpixelSizeY{};

        Vector3T<T> _du{};
        Vector3T<T> _dv{};

        Vector3T<T> _upperLeftCorner{};

    public:
        Vector3T<T> Position{};
        Vector3T<T> LookAt{};
        Vector3T<T> Up{};

        UIntVector2 ScreenSize{};
        Vector2T<T> OrthoSize{};

        inline constexpr OrthographicCamera(
            const Vector3T<T>& position,
            const Vector3T<T>& lookAt,
            const Vector3T<T>& up,
            unsigned int subpixelCount,
            const UIntVector2& screenSize,
            const Vector2T<T>& orthoSize)
            :
            Camera{subpixelCount},
            Position{position},
            LookAt{lookAt},
            Up{up},
            ScreenSize{screenSize},
            OrthoSize{orthoSize}
        {
            Vector3T<T> forward = (Position - LookAt).Normalize();

            Vector3T<T> u = (Up % forward).Normalize();
            Vector3T<T> v = (forward % u).Normalize();

            _du = OrthoSize.X * u;
            _dv = OrthoSize.Y * v;

            _upperLeftCorner = Position - _du * T{0.5} + _dv * T{0.5} - forward;

            _recipricalWidth = Math::rcp(static_cast<T>(ScreenSize.X));
            _recipricalHeight = Math::rcp(static_cast<T>(ScreenSize.Y));

            _subpixelSizeX = Math::rcp(static_cast<T>(subpixelCount)) * _recipricalWidth;
            _subpixelSizeY = Math::rcp(static_cast<T>(subpixelCount)) * _recipricalHeight;
        }

        Ray CreateRay(UIntVector2 pixel, UIntVector2 subpixel, const Random& random) const override
        {
            T normalizedX = static_cast<T>(ScreenSize.X - pixel.X - 1) * _recipricalWidth;
            T normalizedY = static_cast<T>(pixel.Y) * _recipricalHeight;

            normalizedX += static_cast<T>(subpixel.X) * _subpixelSizeX;
            normalizedY += static_cast<T>(subpixel.Y) * _subpixelSizeY;

            normalizedX += random.GetNormalized<T>() * _subpixelSizeX;
            normalizedY += random.GetNormalized<T>() * _subpixelSizeY;

            Vector3T<T> rayDirection = (_upperLeftCorner + (normalizedX * _du) - (normalizedY * _dv) - Position).Normalize();

            // TODO: Not actually an orthographic camera since the position is the same. More of a cheaper perspective camera.
            return
            {
                Vector3{static_cast<Vector3T<TOutput>>(Position)},
                Vector3{static_cast<Vector3T<TOutput>>(rayDirection)},
            };
        }
    };
}
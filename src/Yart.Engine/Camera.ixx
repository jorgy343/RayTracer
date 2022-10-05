export module Camera;

import "Common.h";

import Math;
import Random;
import Ray;

namespace Yart
{
	export class Camera
	{
	public:
        unsigned int SubpixelCount{};

        constexpr explicit Camera(unsigned int subpixelCount)
            : SubpixelCount{subpixelCount}
        {

        }

		virtual constexpr Ray CreateRay(UIntVector2 pixel, UIntVector2 subpixel, const Random& random) const = 0;
	};
}
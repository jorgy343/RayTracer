export module Camera;

import Random;
import Ray;
import Vector2;

namespace Yart
{
	export class Camera
	{
	public:
		virtual constexpr Ray CreateRay(UIntVector2 pixel, UIntVector2 subpixel, const Random& random) const = 0;
	};
}
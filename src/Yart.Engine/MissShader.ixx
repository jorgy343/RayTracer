export module MissShader;

import "Common.h";

import Math;
import Random;
import Ray;

namespace Yart
{
    export class MissShader
    {
    public:
        virtual constexpr Color3 CalculateColor(const Ray& ray, const Random& random) const = 0;
    };
}
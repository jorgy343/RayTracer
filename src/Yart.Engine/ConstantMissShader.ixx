export module ConstantMissShader;

import "Common.h";

import Math;
import MissShader;
import Random;
import Ray;

namespace Yart
{
    export class ConstantMissShader : public MissShader
    {
    protected:
        Vector3 Color;

    public:
        constexpr ConstantMissShader(const Vector3& color)
            : Color{color}
        {

        }

        Vector3 CalculateColor(const Ray& ray, const Random& random) const override
        {
            return Color;
        }
    };
}
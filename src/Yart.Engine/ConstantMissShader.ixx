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
        Color3 Color;

    public:
        ConstantMissShader(const Color3& color)
            : Color{color}
        {

        }

        virtual Color3 CalculateColor(const Ray& ray, const Random& random) const override
        {
            return Color;
        }
    };
}
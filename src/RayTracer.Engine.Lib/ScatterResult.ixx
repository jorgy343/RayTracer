export module ScatterResult;

import Ray;

namespace RayTracer
{
    export class ScatterResult
    {
    public:
        Ray ScatterRay{};
        float Brdf{0.0f};
        float Pdf{0.0f};

        inline constexpr ScatterResult(const Ray& scatterRay, float brdf, float pdf)
            : ScatterRay{scatterRay}, Brdf{brdf}, Pdf{pdf}
        {

        }
    };
}
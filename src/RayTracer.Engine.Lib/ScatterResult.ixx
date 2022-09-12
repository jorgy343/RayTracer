export module RayTracer.ScatterResult;

import RayTracer.Ray;

namespace RayTracer
{
    export class ScatterResult
    {
    public:
        Ray ScatterRay{};
        float ScatterPdf{0.0f};

        inline constexpr ScatterResult(const Ray& scatterRay, float scatterPdf)
            : ScatterRay{scatterRay}, ScatterPdf{scatterPdf}
        {

        }
    };
}
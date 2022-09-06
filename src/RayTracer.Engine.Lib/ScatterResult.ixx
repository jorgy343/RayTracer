export module RayTracer.ScatterResult;

import RayTracer.Ray;

namespace RayTracer
{
    export class ScatterResult
    {
    public:
        Ray ScatterRay;
        float ScatterPdf;

        ScatterResult(const Ray& scatterRay, float scatterPdf)
            : ScatterRay{scatterRay}, ScatterPdf{scatterPdf}
        {

        }
    };
}
export module ScatterResult;

import Vector3;

namespace RayTracer
{
    export class ScatterResult
    {
    public:
        Vector3 OutgoingDirection{};
        float InversePdf{0.0f};

        inline constexpr ScatterResult(const Vector3& outgoingDirection, float inversePdf)
            : OutgoingDirection{outgoingDirection}, InversePdf{inversePdf}
        {

        }
    };
}
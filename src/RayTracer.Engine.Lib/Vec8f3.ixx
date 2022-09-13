module;

#include "Vcl.h"

export module Vec8f3;

using namespace vcl;

namespace RayTracer
{
    export class Vec8f3
    {
    public:
        Vec8f X{};
        Vec8f Y{};
        Vec8f Z{};

        inline Vec8f3() = default;

        inline Vec8f3(Vec8f x, Vec8f y, Vec8f z)
            : X{x}, Y{y}, Z{z}
        {

        }
    };
}
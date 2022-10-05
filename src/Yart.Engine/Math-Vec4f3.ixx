module;

#include "Vcl.h"

export module Math:Vec4f3;

import "Common.h";

using namespace vcl;

namespace Yart
{
    export class Vec4f3
    {
    public:
        Vec4f X{};
        Vec4f Y{};
        Vec4f Z{};

        inline Vec4f3() = default;

        inline Vec4f3(Vec4f x, Vec4f y, Vec4f z)
            : X{x}, Y{y}, Z{z}
        {

        }
    };
}
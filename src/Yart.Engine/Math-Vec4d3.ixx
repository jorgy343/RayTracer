module;

#include "Vcl.h"

export module Math:Vec4d3;

import "Common.h";

using namespace vcl;

namespace Yart
{
    export class Vec4d3
    {
    public:
        Vec4d X{};
        Vec4d Y{};
        Vec4d Z{};

        inline Vec4d3() = default;

        inline Vec4d3(Vec4d x, Vec4d y, Vec4d z)
            : X{x}, Y{y}, Z{z}
        {

        }
    };
}
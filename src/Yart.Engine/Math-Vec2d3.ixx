module;

#include "Vcl.h"

export module Math:Vec2d3;

import "Common.h";

using namespace vcl;

namespace Yart
{
    export class Vec2d3
    {
    public:
        Vec2d X{};
        Vec2d Y{};
        Vec2d Z{};

        inline Vec2d3() = default;

        inline Vec2d3(Vec2d x, Vec2d y, Vec2d z)
            : X{x}, Y{y}, Z{z}
        {

        }
    };
}
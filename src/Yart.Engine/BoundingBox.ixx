export module BoundingBox;

import "Common.h";

import Vector3;

namespace Yart
{
    export template <real_number T>
        class BoundingBoxT
    {
    public:
        Vector3T<T> Minimum{};
        Vector3T<T> Maximum{};

        inline constexpr BoundingBoxT(const Vector3T<T>& minimum, const Vector3T<T>& maximum)
            : Minimum{minimum}, Maximum{maximum}
        {

        }

        constexpr bool Intersects(const Vector3T<T>& other) const
        {
            return
                other.X >= Minimum.X &&
                other.Y >= Minimum.Y &&
                other.Z >= Minimum.Z &&
                other.X <= Maximum.X &&
                other.Y <= Maximum.Y &&
                other.Z <= Maximum.Z;
        }

        constexpr bool Intersects(const BoundingBoxT& other) const
        {
            return
                Minimum.X <= other.Maximum.X &&
                Minimum.Y <= other.Maximum.Y &&
                Minimum.Z <= other.Maximum.Z &&
                Maximum.X >= other.Minimum.X &&
                Maximum.Y >= other.Minimum.Y &&
                Maximum.Z >= other.Minimum.Z;
        }
    };

    export using BoundingBox = BoundingBoxT<real>;
}
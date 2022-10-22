export module BoundingBox;

import "Common.h";

import Math;

namespace Yart
{
    export template <real_number T>
        class BoundingBoxT
    {
    public:
        Vector3T<T> Minimum{};
        Vector3T<T> Maximum{};

        constexpr BoundingBoxT(const Vector3T<T>& minimum, const Vector3T<T>& maximum)
            : Minimum{minimum}, Maximum{maximum}
        {

        }

        static constexpr BoundingBoxT Infinity()
        {
            return BoundingBoxT{
                Vector3T<T>(-std::numeric_limits<T>::infinity()),
                Vector3T<T>(std::numeric_limits<T>::infinity()),
            };
        }

        static constexpr BoundingBoxT ReverseInfinity()
        {
            return BoundingBoxT{
                Vector3T<T>(std::numeric_limits<T>::infinity()),
                Vector3T<T>(-std::numeric_limits<T>::infinity()),
            };
        }

        /// @brief Increases (or decreases if sizeIncrease is negative) the size of the bounding box by sizeIncrease. Each axis
        /// of the bounding box can be adjusted independently.
        /// @param sizeIncrease The amount to increase (or decrease) the bounding box by.
        /// @return A new bounding box with the size adjusted by sizeIncrease.
        constexpr BoundingBoxT AddMargin(const Vector3T<T>& sizeIncrease) const
        {
            return BoundingBoxT{
                Minimum - sizeIncrease,
                Maximum + sizeIncrease,
            };
        }

        constexpr Vector3T<T> CalculateCenterPoint() const
        {
            return Minimum + (Maximum - Minimum) * T { 0.5 };
        }

        constexpr BoundingBoxT Union(const Vector3T<T>& other) const
        {
            return BoundingBoxT{
                Minimum.Min(other),
                Maximum.Max(other),
            };
        }

        constexpr BoundingBoxT Union(const BoundingBoxT& other) const
        {
            return BoundingBoxT{
                Vector3::Min(Minimum, other.Minimum),
                Vector3::Max(Maximum, other.Maximum),
            };
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

        constexpr std::vector<BoundingBoxT> UniformSplit(size_t count) const
        {
            constexpr T buffer = T{0.001};

            T deltaX = (Maximum.X - Minimum.X) / static_cast<T>(count);
            T deltaY = (Maximum.Y - Minimum.Y) / static_cast<T>(count);
            T deltaZ = (Maximum.Z - Minimum.Z) / static_cast<T>(count);

            std::vector<BoundingBoxT> result{};
            result.reserve(count * count * count);

            for (size_t z = 0; z < count; z++)
            {
                for (size_t y = 0; y < count; y++)
                {
                    for (size_t x = 0; x < count; x++)
                    {
                        result.push_back(BoundingBoxT{
                            Vector3T<T>{Minimum.X + (x + 0) * deltaX - buffer, Minimum.Y + (y + 0) * deltaY - buffer, Minimum.Z + (z + 0) * deltaZ - buffer},
                            Vector3T<T>{Minimum.X + (x + 1) * deltaX + buffer, Minimum.Y + (y + 1) * deltaY + buffer, Minimum.Z + (z + 1) * deltaZ + buffer}});
                    }
                }
            }

            return result;
        }
    };

    export using BoundingBox = BoundingBoxT<real>;
}
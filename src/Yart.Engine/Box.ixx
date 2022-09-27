export module Box;

import "Common.h";

import Vector3;

namespace Yart
{
    export template <real_number T>
    class BoxT
    {
    public:
        Vector3T<T> Minimum{};
        Vector3T<T> Maximum{};

        inline constexpr BoxT(const Vector3T<T>& minimum, const Vector3T<T>& maximum)
            : Minimum{minimum}, Maximum{maximum}
        {

        }
    };

    export using Box = BoxT<real>;
}
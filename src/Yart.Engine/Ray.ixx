export module Ray;

import <cmath>;
import <limits>;

import "Common.h";

import Math;

namespace Yart
{
    export template <real_number T>
        class __declspec(dllexport) alignas(16) RayT
    {
    public:
        Vector3T<T> Position{};
        Vector3T<T> Direction{};
        Vector3T<T> InverseDirection{};

        inline constexpr RayT(const Vector3T<T>& position, const Vector3T<T>& direction)
            : Position{position}, Direction{direction}, InverseDirection{Vector3{Math::rcp(direction.X), Math::rcp(direction.Y), Math::rcp(direction.Z)}}
        {

        }

        inline constexpr RayT(const Vector3T<T>& position, const Vector3T<T>& direction, const Vector3T<T>& inverseDirection)
            : Position{position}, Direction{direction}, InverseDirection{inverseDirection}
        {

        }

        template <real_number U>
        inline constexpr explicit operator RayT<U>() const
        {
            return RayT<U>(static_cast<Vector3T<U>>(Position), static_cast<Vector3T<U>>(Direction), static_cast<Vector3T<U>>(InverseDirection));
        }
    };

    export using Ray = RayT<real>;
}
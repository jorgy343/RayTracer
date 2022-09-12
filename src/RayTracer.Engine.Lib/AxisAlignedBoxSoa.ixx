#include <cmath>

#include "Vcl.h"

export module RayTracer.AxisAlignedBoxSoa;

import RayTracer.Alignment;
import RayTracer.IntersectionResult;
import RayTracer.Ray;
import RayTracer.Math;
import RayTracer.AxisAlignedBox;
import RayTracer.GeometrySoa;
import RayTracer.IntersectionResultType;

using namespace vcl;

namespace RayTracer
{
    export class AxisAlignedBoxSoa final : public GeometrySoa<AxisAlignedBox>
    {
    private:
        AlignedVector<float, 64> _minimumX{};
        AlignedVector<float, 64> _minimumY{};
        AlignedVector<float, 64> _minimumZ{};

        AlignedVector<float, 64> _maximumX{};
        AlignedVector<float, 64> _maximumY{};
        AlignedVector<float, 64> _maximumZ{};

        AlignedVector<const AxisAlignedBox*, 64> _geometries{};

    public:
        AxisAlignedBoxSoa(int initialCapacity = 32)
        {
            _minimumX.reserve(initialCapacity);
            _minimumY.reserve(initialCapacity);
            _minimumZ.reserve(initialCapacity);

            _maximumX.reserve(initialCapacity);
            _maximumY.reserve(initialCapacity);
            _maximumZ.reserve(initialCapacity);

            _geometries.reserve(initialCapacity);
        }

        long long GetCount() const override final
        {
            return _minimumX.size();
        }

        void Add(const AxisAlignedBox* geometry) override final
        {
            _minimumX.push_back(geometry->Minimum.X);
            _minimumY.push_back(geometry->Minimum.Y);
            _minimumZ.push_back(geometry->Minimum.Z);

            _maximumX.push_back(geometry->Maximum.X);
            _maximumY.push_back(geometry->Maximum.Y);
            _maximumZ.push_back(geometry->Maximum.Z);

            _geometries.push_back(geometry);
        }

        void Finalize() override final
        {
            for (long long i = _minimumX.size(); i % 8 != 0; i++)
            {
                _minimumX.push_back(std::numeric_limits<float>::infinity());
                _minimumY.push_back(std::numeric_limits<float>::infinity());
                _minimumZ.push_back(std::numeric_limits<float>::infinity());

                _maximumX.push_back(std::numeric_limits<float>::infinity());
                _maximumY.push_back(std::numeric_limits<float>::infinity());
                _maximumZ.push_back(std::numeric_limits<float>::infinity());

                _geometries.push_back(nullptr);
            }
        }

        IntersectionResult IntersectEntrance(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        IntersectionResult IntersectExit(const Ray& ray) const override final
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

        template <IntersectionResultType TIntersectionResultType>
        inline IntersectionResult PrivateIntersectSoa(const Ray& ray, int startingGeometryIndex) const
        {
            Vec8f minimumX = Vec8f{}.load_a(&_minimumX[startingGeometryIndex]);
            Vec8f minimumY = Vec8f{}.load_a(&_minimumY[startingGeometryIndex]);
            Vec8f minimumZ = Vec8f{}.load_a(&_minimumZ[startingGeometryIndex]);

            Vec8f rayPositionX{ray.Position.X};
            Vec8f rayPositionY{ray.Position.Y};
            Vec8f rayPositionZ{ray.Position.Z};

            Vec8f rayInverseDirectionX{ray.InverseDirection.X};
            Vec8f rayInverseDirectionY{ray.InverseDirection.Y};
            Vec8f rayInverseDirectionZ{ray.InverseDirection.Z};

            Vec8f minX = ConvertNanToInf((minimumX - rayPositionX) * rayInverseDirectionX);
            Vec8f minY = ConvertNanToInf((minimumY - rayPositionY) * rayInverseDirectionY);
            Vec8f minZ = ConvertNanToInf((minimumZ - rayPositionZ) * rayInverseDirectionZ);

            Vec8f maximumX = Vec8f{}.load_a(&_maximumX[startingGeometryIndex]);
            Vec8f maximumY = Vec8f{}.load_a(&_maximumY[startingGeometryIndex]);
            Vec8f maximumZ = Vec8f{}.load_a(&_maximumZ[startingGeometryIndex]);

            Vec8f maxX = ConvertNanToInf((maximumX - rayPositionX) * rayInverseDirectionX);
            Vec8f maxY = ConvertNanToInf((maximumY - rayPositionY) * rayInverseDirectionY);
            Vec8f maxZ = ConvertNanToInf((maximumZ - rayPositionZ) * rayInverseDirectionZ);

            Vec8f exitDistance = vcl::min(vcl::min(vcl::max(minX, maxX), vcl::max(minY, maxY)), vcl::max(minZ, maxZ));
            Vec8f entranceDistance = vcl::max(vcl::max(vcl::min(minX, maxX), vcl::min(minY, maxY)), vcl::min(minZ, maxZ));

            Vec8f result;
            if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
            {
                result = entranceDistance;
            }
            else
            {
                result = exitDistance;
            }

            Vec8f clampedEntranceDistance = select(exitDistance >= Vec8f(0.0f) & entranceDistance <= exitDistance, result, infinite8f());
            
            float minimumEntranceDistance = horizontal_min1(clampedEntranceDistance);
            int minimumIndex = horizontal_find_first(Vec8f(minimumEntranceDistance) == clampedEntranceDistance);

            return {
                _geometries[startingGeometryIndex + (minimumIndex == -1 ? 0 : minimumIndex)],
                minimumEntranceDistance,
            };
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        inline IntersectionResult Intersect(const Ray& ray) const
        {
            IntersectionResult result{nullptr, std::numeric_limits<float>::infinity()};

            for (int i = 0; i + 8 <= _minimumX.size(); i += 8)
            {
                IntersectionResult newResult = PrivateIntersectSoa<TIntersectionResultType>(ray, i);

                if (newResult.Distance < result.Distance)
                {
                    result = newResult;
                }
            }

            return result;
        }
    };
}
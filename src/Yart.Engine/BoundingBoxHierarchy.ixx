module;

#include "Vcl.h"

export module BoundingBoxHierarchy;

import <cassert>;
import <concepts>;
import <limits>;
import <memory>;
import <vector>;

import "Common.h";

import BoundingBox;
import Geometry;
import IntersectableGeometry;
import IntersectionResult;
import IntersectionResultType;
import Math;
import Triangle;
import Vector3;

using namespace vcl;

namespace Yart
{
    export template <real_number T, size_t NumberOfLeafs>
        class alignas(64) BoundingBoxHierarchyT : public IntersectableGeometry
    {
    protected:
        T MinimumX[NumberOfLeafs];
        T MinimumY[NumberOfLeafs];
        T MinimumZ[NumberOfLeafs];

        T MaximumX[NumberOfLeafs];
        T MaximumY[NumberOfLeafs];
        T MaximumZ[NumberOfLeafs];

        const IntersectableGeometry* Children[NumberOfLeafs];

    public:
        constexpr BoundingBoxHierarchyT()
        {
            for (size_t i = 0; i < NumberOfLeafs; i++)
            {
                MinimumX[i] = -std::numeric_limits<T>::infinity();
                MinimumY[i] = -std::numeric_limits<T>::infinity();
                MinimumZ[i] = -std::numeric_limits<T>::infinity();

                MaximumX[i] = std::numeric_limits<T>::infinity();
                MaximumY[i] = std::numeric_limits<T>::infinity();
                MaximumZ[i] = std::numeric_limits<T>::infinity();

                Children[i] = nullptr;
            }
        }

        constexpr void SetChild(size_t leafIndex, const IntersectableGeometry* child)
        {
            static_assert(leafIndex < NumberOfLeafs);
            assert(leafIndex < NumberOfLeafs);

            auto boundingBox = child->CalculateBoundingBox();

            MinimumX[leafIndex] = boundingBox.Minimum.X;
            MinimumY[leafIndex] = boundingBox.Minimum.Y;
            MinimumZ[leafIndex] = boundingBox.Minimum.Z;

            MaximumX[leafIndex] = boundingBox.Maximum.X;
            MaximumY[leafIndex] = boundingBox.Maximum.Y;
            MaximumZ[leafIndex] = boundingBox.Maximum.Z;

            Children[leafIndex] = child;
        }

        constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        constexpr IntersectionResult IntersectExit(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Exit>(ray);
        }

    private:
        template <IntersectionResultType TIntersectionResultType>
        force_inline constexpr IntersectionResult Intersect(const Ray& ray) const
        {
            //if (std::is_constant_evaluated())
            //{
            //    float closestDistance = std::numeric_limits<float>::infinity();
            //    const Sphere* closestGeometry = nullptr;

            //    for (int i = 0; i < 8; i++)
            //    {
            //        auto geometry = _geometries[i];

            //        if (geometry == nullptr)
            //        {
            //            continue;
            //        }

            //        float distance = geometry->Intersect<TIntersectionResultType>(ray);

            //        if (distance < closestDistance)
            //        {
            //            closestDistance = distance;
            //            closestGeometry = geometry;
            //        }
            //    }

            //    return {closestGeometry, closestDistance};
            //}


            if constexpr (std::same_as<float, T> && NumberOfLeafs == 8)
            {
                Vec8f minimumX = Vec8f{}.load_a(MinimumX);
                Vec8f minimumY = Vec8f{}.load_a(MinimumY);
                Vec8f minimumZ = Vec8f{}.load_a(MinimumZ);

                Vec8f rayPositionX{ray.Position.X};
                Vec8f rayPositionY{ray.Position.Y};
                Vec8f rayPositionZ{ray.Position.Z};

                Vec8f rayInverseDirectionX{ray.InverseDirection.X};
                Vec8f rayInverseDirectionY{ray.InverseDirection.Y};
                Vec8f rayInverseDirectionZ{ray.InverseDirection.Z};

                Vec8f minX = ConvertNanToInf((minimumX - rayPositionX) * rayInverseDirectionX);
                Vec8f minY = ConvertNanToInf((minimumY - rayPositionY) * rayInverseDirectionY);
                Vec8f minZ = ConvertNanToInf((minimumZ - rayPositionZ) * rayInverseDirectionZ);

                Vec8f maximumX = Vec8f{}.load_a(MaximumX);
                Vec8f maximumY = Vec8f{}.load_a(MaximumY);
                Vec8f maximumZ = Vec8f{}.load_a(MaximumZ);

                Vec8f maxX = ConvertNanToInf((maximumX - rayPositionX) * rayInverseDirectionX);
                Vec8f maxY = ConvertNanToInf((maximumY - rayPositionY) * rayInverseDirectionY);
                Vec8f maxZ = ConvertNanToInf((maximumZ - rayPositionZ) * rayInverseDirectionZ);

                Vec8f exitDistance = vcl::min(vcl::min(vcl::max(minX, maxX), vcl::max(minY, maxY)), vcl::max(minZ, maxZ));
                Vec8f entranceDistance = vcl::max(vcl::max(vcl::min(minX, maxX), vcl::min(minY, maxY)), vcl::min(minZ, maxZ));

                Vec8f clampedEntranceDistance = select(exitDistance >= Vec8f(0.0f) & entranceDistance <= exitDistance, entranceDistance, infinite8f());

                alignas(32) float distances[8];
                clampedEntranceDistance.store_a(distances);

                IntersectionResult closestIntersection{nullptr, std::numeric_limits<float>::infinity()};
                for (size_t i = 0; i < 8; i++)
                {
                    if (distances[i] != std::numeric_limits<float>::infinity() && Children[i])
                    {
                        IntersectionResult result;

                        if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
                        {
                            result = Children[i]->IntersectEntrance(ray);
                        }
                        else
                        {
                            result = Children[i]->IntersectExit(ray);
                        }

                        if (result.HitDistance < closestIntersection.HitDistance)
                        {
                            closestIntersection = result;
                        }
                    }
                }

                return closestIntersection;
            }
        }
    };

    export using BoundingBoxHierarchy = BoundingBoxHierarchyT<real, 8>;

    export std::shared_ptr<BoundingBoxHierarchy> CreateBoundingBoxHierarchy(const std::vector<Triangle>& triangles)
    {
        auto hierarchy = std::shared_ptr<BoundingBoxHierarchy>(new BoundingBoxHierarchy{});
        
        BoundingBox rootBox{
            Vector3{std::numeric_limits<float>::infinity()},
            Vector3{-std::numeric_limits<float>::infinity()},
        };

        for (const auto& triangle : triangles)
        {
            rootBox = rootBox.Union(triangle.CalculateBoundingBox());
        }

        return hierarchy;
    }
}
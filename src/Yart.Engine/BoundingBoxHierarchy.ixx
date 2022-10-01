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
import GeometryCollection;
import GeometrySoaUtilities;
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
        alignas(32) T MinimumX[NumberOfLeafs];
        alignas(32) T MinimumY[NumberOfLeafs];
        alignas(32) T MinimumZ[NumberOfLeafs];

        alignas(32) T MaximumX[NumberOfLeafs];
        alignas(32) T MaximumY[NumberOfLeafs];
        alignas(32) T MaximumZ[NumberOfLeafs];

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

        constexpr void SetChild(size_t leafIndex, const BoundingBoxT<T>& boundingBox, const IntersectableGeometry* child)
        {
            assert(leafIndex < NumberOfLeafs);

            MinimumX[leafIndex] = boundingBox.Minimum.X;
            MinimumY[leafIndex] = boundingBox.Minimum.Y;
            MinimumZ[leafIndex] = boundingBox.Minimum.Z;

            MaximumX[leafIndex] = boundingBox.Maximum.X;
            MaximumY[leafIndex] = boundingBox.Maximum.Y;
            MaximumZ[leafIndex] = boundingBox.Maximum.Z;

            Children[leafIndex] = child;
        }

        constexpr BoundingBox CalculateBoundingBox() const override
        {
            static_assert(NumberOfLeafs > 0);

            BoundingBox boundingBox{
                Vector3{MinimumX[0], MinimumY[0], MinimumZ[0]},
                Vector3{MaximumX[0], MaximumY[0], MaximumZ[0]},
            };

            for (int i = 1; i < NumberOfLeafs; i++)
            {
                BoundingBox newBoundingBox{
                    Vector3{MinimumX[i], MinimumY[i], MinimumZ[i]},
                    Vector3{MaximumX[i], MaximumY[i], MaximumZ[i]},
                };

                boundingBox = boundingBox.Union(newBoundingBox);
            }

            return boundingBox;
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

    export class BoundingBoxBuildParameters
    {
    public:
        size_t PreferredMaxItemCountPerNode{32};
        size_t MaxDepth{4};

        BoundingBoxBuildParameters() = default;

        BoundingBoxBuildParameters(size_t preferredMaxItemCountPerNode, size_t maxDepth)
            : PreferredMaxItemCountPerNode{preferredMaxItemCountPerNode}, MaxDepth{maxDepth}
        {

        }
    };

    const BoundingBoxHierarchy* BuildUniformBoundingBoxHierarchy(
        size_t currentDepth,
        const BoundingBoxBuildParameters& parameters,
        std::vector<const IntersectableGeometry*>& inputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        auto hierarchy = std::shared_ptr<BoundingBoxHierarchy>(new BoundingBoxHierarchy{});
        geometryPointers.push_back(hierarchy);

        BoundingBox rootBoundingBox{
            Vector3{std::numeric_limits<float>::infinity()},
            Vector3{-std::numeric_limits<float>::infinity()},
        };

        for (const auto* inputGeometry : inputGeometries)
        {
            rootBoundingBox = rootBoundingBox.Union(inputGeometry->CalculateBoundingBox());
        }

        // Split the root bounding box into 8 evenly sized bounding boxes.
        auto splitRootBoundingBoxes = rootBoundingBox.UniformSplit(2);

        for (size_t i = 0; i < 8; i++)
        {
            auto& splitRootBoundingBox = splitRootBoundingBoxes[i];
            std::vector<const IntersectableGeometry*> intersectedGeometries{};

            for (const auto* inputGeometry : inputGeometries)
            {
                if (splitRootBoundingBox.Intersects(inputGeometry->CalculateBoundingBox()))
                {
                    intersectedGeometries.push_back(inputGeometry);
                }
            }

            if (intersectedGeometries.size() == 0)
            {
                hierarchy->SetChild(i, splitRootBoundingBox, nullptr);
            }
            else if (currentDepth < parameters.MaxDepth && intersectedGeometries.size() > parameters.PreferredMaxItemCountPerNode)
            {
                auto childHierarchy = BuildUniformBoundingBoxHierarchy(currentDepth + 1, parameters, intersectedGeometries, geometryPointers);
                hierarchy->SetChild(i, splitRootBoundingBox, childHierarchy);
            }
            else
            {
                std::vector<const IntersectableGeometry*> finalIntersectedGeometries{};
                CreateGeometrySoaStructures(intersectedGeometries, finalIntersectedGeometries, geometryPointers);

                // TODO: Small optimization: If a single geometry, just add that to the note instead of a geometry collection.
                auto geometryCollection = std::make_shared<const GeometryCollection>(finalIntersectedGeometries);
                geometryPointers.push_back(geometryCollection);

                hierarchy->SetChild(i, splitRootBoundingBox, geometryCollection.get());
            }
        }

        return hierarchy.get();
    }

    export const BoundingBoxHierarchy* BuildUniformBoundingBoxHierarchy(
        const BoundingBoxBuildParameters& parameters,
        std::vector<const IntersectableGeometry*>& inputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        return BuildUniformBoundingBoxHierarchy(1, parameters, inputGeometries, geometryPointers);
    }
}
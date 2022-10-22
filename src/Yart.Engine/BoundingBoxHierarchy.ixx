module;

#include "range/v3/all.hpp"

#include "Vcl.h"

export module BoundingBoxHierarchy;

import <cassert>;

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

using namespace vcl;

namespace Yart
{
    export template <real_number T>
        class alignas(64) BoundingBoxHierarchyT : public IntersectableGeometry
    {
    private:
        using VclVec = typename std::conditional<std::same_as<T, float>, Vec8f, Vec4d>::type;

    public:
        static constexpr size_t NumberOfLeafs = std::same_as<T, float> ? 8 : 4;

    protected:
        alignas(sizeof(T) * 4) T MinimumX[NumberOfLeafs];
        alignas(sizeof(T) * 4) T MinimumY[NumberOfLeafs];
        alignas(sizeof(T) * 4) T MinimumZ[NumberOfLeafs];

        alignas(sizeof(T) * 4) T MaximumX[NumberOfLeafs];
        alignas(sizeof(T) * 4) T MaximumY[NumberOfLeafs];
        alignas(sizeof(T) * 4) T MaximumZ[NumberOfLeafs];

        const IntersectableGeometry* Children[NumberOfLeafs];

    public:
        BoundingBoxHierarchyT()
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

        void SetChild(size_t leafIndex, const IntersectableGeometry* child)
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

        void SetChild(size_t leafIndex, const BoundingBoxT<T>& boundingBox, const IntersectableGeometry* child)
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

        virtual BoundingBox CalculateBoundingBox() const override
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

        virtual IntersectionResult IntersectEntrance(const Ray& ray) const override
        {
            return Intersect<IntersectionResultType::Entrance>(ray);
        }

        virtual IntersectionResult IntersectExit(const Ray& ray) const override
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

            //    for (int i = 0; i < NumberOfLeafs; i++)
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

            VclVec minimumX = VclVec{}.load_a(MinimumX);
            VclVec minimumY = VclVec{}.load_a(MinimumY);
            VclVec minimumZ = VclVec{}.load_a(MinimumZ);

            VclVec rayPositionX{ray.Position.X};
            VclVec rayPositionY{ray.Position.Y};
            VclVec rayPositionZ{ray.Position.Z};

            VclVec rayInverseDirectionX{ray.InverseDirection.X};
            VclVec rayInverseDirectionY{ray.InverseDirection.Y};
            VclVec rayInverseDirectionZ{ray.InverseDirection.Z};

            VclVec minX = ConvertNanToInf((minimumX - rayPositionX) * rayInverseDirectionX);
            VclVec minY = ConvertNanToInf((minimumY - rayPositionY) * rayInverseDirectionY);
            VclVec minZ = ConvertNanToInf((minimumZ - rayPositionZ) * rayInverseDirectionZ);

            VclVec maximumX = VclVec{}.load_a(MaximumX);
            VclVec maximumY = VclVec{}.load_a(MaximumY);
            VclVec maximumZ = VclVec{}.load_a(MaximumZ);

            VclVec maxX = ConvertNanToInf((maximumX - rayPositionX) * rayInverseDirectionX);
            VclVec maxY = ConvertNanToInf((maximumY - rayPositionY) * rayInverseDirectionY);
            VclVec maxZ = ConvertNanToInf((maximumZ - rayPositionZ) * rayInverseDirectionZ);

            VclVec exitDistance = vcl::min(vcl::min(vcl::max(minX, maxX), vcl::max(minY, maxY)), vcl::max(minZ, maxZ));
            VclVec entranceDistance = vcl::max(vcl::max(vcl::min(minX, maxX), vcl::min(minY, maxY)), vcl::min(minZ, maxZ));

            VclVec clampedEntranceDistance = select(exitDistance >= VclVec{real{0.0}} &entranceDistance <= exitDistance, entranceDistance, VclVec{std::numeric_limits<real>::infinity()});

            alignas(sizeof(T) * 4) T distances[NumberOfLeafs];
            clampedEntranceDistance.store_a(distances);

            IntersectionResult closestIntersection{nullptr, std::numeric_limits<float>::infinity()};
            for (size_t i = 0; i < NumberOfLeafs; i++)
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
    };

    export using BoundingBoxHierarchy = BoundingBoxHierarchyT<real>;

    export class BoundingBoxBuildParameters
    {
    public:
        UIntVector2 PreferredNodeSize{16, 32};
        size_t MaxDepth{8};

        BoundingBoxBuildParameters() = default;

        BoundingBoxBuildParameters(
            UIntVector2 preferredNodeSize,
            size_t maxDepth)
            :
            PreferredNodeSize{preferredNodeSize},
            MaxDepth{maxDepth}
        {

        }
    };

    // TODO: Make this work for doubles.
    const BoundingBoxHierarchy* BuildUniformBoundingBoxHierarchy(
        size_t currentDepth,
        const BoundingBoxBuildParameters& parameters,
        std::vector<const IntersectableGeometry*>& inputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        auto hierarchy = std::shared_ptr<BoundingBoxHierarchy>(new BoundingBoxHierarchy{});
        geometryPointers.push_back(hierarchy);

        BoundingBox totalBoundingBox{
            Vector3{std::numeric_limits<float>::infinity()},
            Vector3{-std::numeric_limits<float>::infinity()},
        };

        for (const auto* inputGeometry : inputGeometries)
        {
            totalBoundingBox = totalBoundingBox.Union(inputGeometry->CalculateBoundingBox());
        }

        // Split the root bounding box into 8 evenly sized bounding boxes.
        auto splitRootBoundingBoxes = totalBoundingBox.UniformSplit(2);

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
            else if (currentDepth < parameters.MaxDepth && intersectedGeometries.size() > parameters.PreferredNodeSize.X)
            {
                auto childHierarchy = BuildUniformBoundingBoxHierarchy(currentDepth + 1, parameters, intersectedGeometries, geometryPointers);
                hierarchy->SetChild(i, splitRootBoundingBox, childHierarchy);
            }
            else
            {
                std::vector<const IntersectableGeometry*> finalIntersectedGeometries{};
                CreateGeometrySoaStructures(intersectedGeometries, finalIntersectedGeometries, geometryPointers);

                if (finalIntersectedGeometries.size() == 1)
                {
                    hierarchy->SetChild(i, splitRootBoundingBox, finalIntersectedGeometries[0]);
                }
                else
                {
                    auto geometryCollection = std::make_shared<const GeometryCollection>(finalIntersectedGeometries);
                    geometryPointers.push_back(geometryCollection);

                    hierarchy->SetChild(i, splitRootBoundingBox, geometryCollection.get());
                }
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

    template<real_number T>
    const BoundingBoxHierarchyT<T>* BuildSplitByLongAxisBoundingBoxHierarchy(
        size_t currentDepth,
        const BoundingBoxBuildParameters& parameters,
        std::vector<const IntersectableGeometry*>& inputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        constexpr size_t Size = std::same_as<real, float> ? 8 : 4;

        auto hierarchy = std::shared_ptr<BoundingBoxHierarchyT<T>>(new BoundingBoxHierarchyT<T>{});
        geometryPointers.push_back(hierarchy);

        BoundingBox totalBoundingBox{
            Vector3{std::numeric_limits<T>::infinity()},
            Vector3{-std::numeric_limits<T>::infinity()},
        };

        for (const auto* inputGeometry : inputGeometries)
        {
            totalBoundingBox = totalBoundingBox.Union(inputGeometry->CalculateBoundingBox());
        }

        // Determine which axis is the longest.
        Vector3 axisLengths = totalBoundingBox.Maximum - totalBoundingBox.Minimum;
        T longerLengthBetweenXAndY = Math::max(axisLengths.X, axisLengths.Y);

        size_t longestAxis = axisLengths.X > axisLengths.Y ? 0 : 1;
        longestAxis = longerLengthBetweenXAndY > axisLengths.Z ? longestAxis : 2;

        // Sort the input geometries by their longest axis and chunk them together.
        size_t geometriesPerNode = Math::max(static_cast<size_t>(1), static_cast<size_t>(Math::ceil(inputGeometries.size() / static_cast<real>(Size))));
        auto geometryChunks = ranges::actions::sort(inputGeometries, {}, [&](const auto& x) { return x->CalculateBoundingBox().CalculateCenterPoint()[longestAxis]; }) | ranges::views::chunk(geometriesPerNode);

        for (size_t i = 0; i < Size; i++)
        {
            if (i >= geometryChunks.size())
            {
                break;
            }

            auto intersectedGeometries = geometryChunks[i] | ranges::to<std::vector>();

            BoundingBox nodeBoundingBox{
                Vector3{std::numeric_limits<T>::infinity()},
                Vector3{-std::numeric_limits<T>::infinity()},
            };

            for (const auto* nodeGeometry : intersectedGeometries)
            {
                nodeBoundingBox = nodeBoundingBox.Union(nodeGeometry->CalculateBoundingBox());
            }

            if (intersectedGeometries.size() == 0)
            {
                hierarchy->SetChild(i, nodeBoundingBox, nullptr);
            }
            else if (currentDepth < parameters.MaxDepth && intersectedGeometries.size() > parameters.PreferredNodeSize.X)
            {
                auto childHierarchy = BuildSplitByLongAxisBoundingBoxHierarchy<T>(currentDepth + 1, parameters, intersectedGeometries, geometryPointers);
                hierarchy->SetChild(i, nodeBoundingBox, childHierarchy);
            }
            else
            {
                std::vector<const IntersectableGeometry*> finalIntersectedGeometries{};
                CreateGeometrySoaStructures(intersectedGeometries, finalIntersectedGeometries, geometryPointers);

                if (finalIntersectedGeometries.size() == 1)
                {
                    hierarchy->SetChild(i, nodeBoundingBox, finalIntersectedGeometries[0]);
                }
                else
                {
                    auto geometryCollection = std::make_shared<const GeometryCollection>(finalIntersectedGeometries);
                    geometryPointers.push_back(geometryCollection);

                    hierarchy->SetChild(i, nodeBoundingBox, geometryCollection.get());
                }
            }
        }

        return hierarchy.get();
    }

    export template<real_number T = real>
        const BoundingBoxHierarchyT<T>* BuildSplitByLongAxisBoundingBoxHierarchy(
            const BoundingBoxBuildParameters& parameters,
            std::vector<const IntersectableGeometry*>& inputGeometries,
            std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        return BuildSplitByLongAxisBoundingBoxHierarchy<T>(1, parameters, inputGeometries, geometryPointers);
    }
}
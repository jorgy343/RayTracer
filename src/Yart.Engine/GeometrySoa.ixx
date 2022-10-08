module;

#include "range/v3/view/chunk.hpp"

export module GeometrySoa;

import "Common.h";

import Geometry;
import IntersectableGeometry;

namespace Yart
{
    export enum class SoaSize
    {
        _128,
        _256,
    };

    export template<GeometryConcept TGeometry>
        class GeometrySoa : public IntersectableGeometry
    {
    public:
        virtual constexpr void Insert(size_t index, const TGeometry* geometry) = 0;
    };

    export template <IntersectableGeometryConcept TGeometry, typename TGeometrySoa>
        requires std::derived_from<TGeometrySoa, GeometrySoa<TGeometry>>
    void CreateGeometrySoaStructure(
        const std::vector<const TGeometry*>& inputGeometries,
        std::vector<const IntersectableGeometry*>& outputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers,
        size_t chunkSize = 8)
    {
        auto chunks = inputGeometries | ranges::views::chunk(chunkSize);
        for (const auto& chunk : chunks)
        {
            if (chunk.size() == 1)
            {
                outputGeometries.push_back(chunk[0]);
            }
            else
            {
                auto soa = std::shared_ptr<TGeometrySoa>{new TGeometrySoa{}};

                geometryPointers.push_back(soa);
                outputGeometries.push_back(soa.get());

                int index = 0;
                for (const auto* geometry : chunk)
                {
                    soa->Insert(index++, geometry);
                }
            }
        }
    }
}
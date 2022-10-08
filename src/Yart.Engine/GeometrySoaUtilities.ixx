module;

#include "range/v3/view/chunk.hpp"

#include "Vcl.h"

export module GeometrySoaUtilities;

import "Common.h";

import AxisAlignedBox;
import AxisAlignedBoxSoa;
import GeometrySoa;
import IntersectableGeometry;
import Parallelogram;
import ParallelogramSoa;
import Plane;
import PlaneSoa;
import Sphere;
import SphereSoa;
import Triangle;
import TriangleSoa;

namespace Yart
{
    export void CreateGeometrySoaStructures(
        const std::vector<const IntersectableGeometry*>& inputGeometries,
        std::vector<const IntersectableGeometry*>& outputGeometries,
        std::vector<std::shared_ptr<const IntersectableGeometry>>& geometryPointers)
    {
        std::vector<const AxisAlignedBox*> axisAlignedBoxes{};
        std::vector<const Parallelogram*> parallelograms{};
        std::vector<const Plane*> planes{};
        std::vector<const Sphere*> spheres{};
        std::vector<const Triangle*> triangles{};

        for (const auto* inputGeometry : inputGeometries)
        {
            if (auto axisAlignedBox = dynamic_cast<const AxisAlignedBox*>(inputGeometry))
            {
                axisAlignedBoxes.push_back(axisAlignedBox);
            }
            else if (auto parallelogram = dynamic_cast<const Parallelogram*>(inputGeometry))
            {
                parallelograms.push_back(parallelogram);
            }
            else if (auto plane = dynamic_cast<const Plane*>(inputGeometry))
            {
                planes.push_back(plane);
            }
            else if (auto sphere = dynamic_cast<const Sphere*>(inputGeometry))
            {
                spheres.push_back(sphere);
            }
            else if (auto triangle = dynamic_cast<const Triangle*>(inputGeometry))
            {
                triangles.push_back(triangle);
            }
        }

        if (axisAlignedBoxes.size() > 0)
        {
            if (axisAlignedBoxes.size() <= AxisAlignedBoxSoa<SoaSize::_128>::Elements)
            {
                CreateGeometrySoaStructure<AxisAlignedBox, AxisAlignedBoxSoa<SoaSize::_128>>(axisAlignedBoxes, outputGeometries, geometryPointers, AxisAlignedBoxSoa<SoaSize::_128>::Elements);
            }
            else
            {
                CreateGeometrySoaStructure<AxisAlignedBox, AxisAlignedBoxSoa<SoaSize::_256>>(axisAlignedBoxes, outputGeometries, geometryPointers, AxisAlignedBoxSoa<SoaSize::_256>::Elements);
            }
        }

        if (parallelograms.size() > 0)
        {
            if (parallelograms.size() <= ParallelogramSoa<SoaSize::_128>::Elements)
            {
                CreateGeometrySoaStructure<Parallelogram, ParallelogramSoa<SoaSize::_128>>(parallelograms, outputGeometries, geometryPointers, ParallelogramSoa<SoaSize::_128>::Elements);
            }
            else
            {
                CreateGeometrySoaStructure<Parallelogram, ParallelogramSoa<SoaSize::_256>>(parallelograms, outputGeometries, geometryPointers, ParallelogramSoa<SoaSize::_256>::Elements);
            }
        }

        if (planes.size() > 0)
        {
            if (planes.size() <= PlaneSoa<SoaSize::_128>::Elements)
            {
                CreateGeometrySoaStructure<Plane, PlaneSoa<SoaSize::_128>>(planes, outputGeometries, geometryPointers, PlaneSoa<SoaSize::_128>::Elements);
            }
            else
            {
                CreateGeometrySoaStructure<Plane, PlaneSoa<SoaSize::_256>>(planes, outputGeometries, geometryPointers, PlaneSoa<SoaSize::_256>::Elements);
            }
        }

        if (spheres.size() > 0)
        {
            if (spheres.size() <= SphereSoa<SoaSize::_128>::Elements)
            {
                CreateGeometrySoaStructure<Sphere, SphereSoa<SoaSize::_128>>(spheres, outputGeometries, geometryPointers, SphereSoa<SoaSize::_128>::Elements);
            }
            else
            {
                CreateGeometrySoaStructure<Sphere, SphereSoa<SoaSize::_256>>(spheres, outputGeometries, geometryPointers, SphereSoa<SoaSize::_256>::Elements);
            }
        }

        if (triangles.size() > 0)
        {
            if (triangles.size() <= TriangleSoa<SoaSize::_128>::Elements)
            {
                CreateGeometrySoaStructure<Triangle, TriangleSoa<SoaSize::_128>>(triangles, outputGeometries, geometryPointers, TriangleSoa<SoaSize::_128>::Elements);
            }
            else
            {
                CreateGeometrySoaStructure<Triangle, TriangleSoa<SoaSize::_256>>(triangles, outputGeometries, geometryPointers, TriangleSoa<SoaSize::_256>::Elements);
            }
        }
    }
}
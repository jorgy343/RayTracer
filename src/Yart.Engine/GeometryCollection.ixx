export module GeometryCollection;

import <initializer_list>;

import "Common.h";

import IntersectableGeometry;
import IntersectionResult;
import IntersectionResultType;
import Ray;

namespace Yart
{
	export class GeometryCollection : public IntersectableGeometry
	{
    protected:
		std::vector<const IntersectableGeometry*> Children{};

	public:
		explicit GeometryCollection(std::initializer_list<const IntersectableGeometry*> childGeometries)
			: Children{childGeometries}
		{

		}

        explicit GeometryCollection(std::vector<const IntersectableGeometry*> childGeometries)
            : Children{childGeometries}
        {

        }

        virtual BoundingBoxT<real> CalculateBoundingBox() const override
        {
            BoundingBoxT<real> boundingBox = BoundingBoxT<real>::ReverseInfinity();

            for (const auto& child : Children)
            {
                boundingBox = boundingBox.Union(child->CalculateBoundingBox());
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
		force_inline IntersectionResult Intersect(const Ray& ray) const
		{
			IntersectionResult closestResult{nullptr, std::numeric_limits<real>::infinity()};

			for (auto geometry : Children)
			{
				IntersectionResult result;
				if constexpr (TIntersectionResultType == IntersectionResultType::Entrance)
				{
					result = geometry->IntersectEntrance(ray);
				}
				else
				{
					result = geometry->IntersectExit(ray);
				}

				if (result.HitDistance < closestResult.HitDistance)
				{
					closestResult = result;
				}
			}

			return closestResult;
		}
	};
}
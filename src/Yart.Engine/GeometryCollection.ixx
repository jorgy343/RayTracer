export module GeometryCollection;

import <initializer_list>;
import <limits>;
import <vector>;

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
		std::vector<const IntersectableGeometry*> ChildGeometries{};

	public:
		inline constexpr explicit GeometryCollection(std::initializer_list<const IntersectableGeometry*> childGeometries)
			: ChildGeometries{childGeometries}
		{

		}

		inline constexpr IntersectionResult IntersectEntrance(const Ray& ray) const override
		{
			return Intersect<IntersectionResultType::Entrance>(ray);
		}

		inline constexpr IntersectionResult IntersectExit(const Ray& ray) const override
		{
			return Intersect<IntersectionResultType::Exit>(ray);
		}

	private:
		template <IntersectionResultType TIntersectionResultType>
		force_inline constexpr IntersectionResult Intersect(const Ray& ray) const
		{
			IntersectionResult closestResult{nullptr, std::numeric_limits<float>::infinity()};

			for (auto geometry : ChildGeometries)
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
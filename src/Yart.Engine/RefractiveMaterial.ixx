export module RefractiveMaterial;

import "Common.h";

import Geometry;
import Material;
import Random;
import Ray;
import Scene;
import Vector3;

namespace Yart
{
    export class RefractiveMaterial : public Material
    {
    protected:
        float RefractionIndex{};

    public:
        inline constexpr RefractiveMaterial(float refractionIndex)
            : RefractionIndex{refractionIndex}
        {

        }

        inline Vector3 CalculateRenderingEquation(
            const Scene& scene,
            const Random& random,
            int currentDepth,
            const Geometry* hitGeometry,
            const Vector3& hitPosition,
            const Vector3& hitNormal,
            const Vector3& incomingDirection) const override
        {
            Vector3 refractionDirection = Vector3::Refract(incomingDirection, hitNormal, 1.0f, RefractionIndex);
            if (refractionDirection.LengthSquared() < 0.01f)
            {
                return Vector3{};
            }

            refractionDirection.Normalize();

            Ray refractionRay = Ray{hitPosition, refractionDirection};
            float exitDistance = hitGeometry->IntersectExit(refractionRay).HitDistance;

            Vector3 exitPosition = refractionRay.Position + exitDistance * refractionRay.Direction;

            // Reverse the refraction direction so that the CalculateNormal method will see the ray
            // as coming in towards the geometry rather than coming out of it.
            refractionRay = Ray{refractionRay.Position, -refractionRay.Direction};

            // Because we flipped the refraction direction, the normal should be pointing away
            // from the geometry.
            Vector3 exitNormal = hitGeometry->CalculateNormal(refractionRay, exitPosition);
            exitPosition += exitNormal * NormalBump;

            // Create the outgoing ray. Use the non reversed refraction direction and the reversed
            // exit normal.
            Vector3 outgoingDirection = Vector3::Refract(refractionDirection, -exitNormal, RefractionIndex, 1.0f);
            if (outgoingDirection.LengthSquared() < 0.01f)
            {
                return Vector3{};
            }

            outgoingDirection.Normalize();

            Ray outgoingRay{exitPosition, outgoingDirection};
            Vector3 outputColor = scene.CastRayColor(outgoingRay, currentDepth + 1, random);

            return outputColor;
        }
    };
}
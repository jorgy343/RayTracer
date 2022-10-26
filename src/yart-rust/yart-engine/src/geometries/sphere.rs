use super::{
    geometry::Geometry, intersectable_geometry::IntersectableGeometry,
    intersection_result::IntersectionResult, ray::Ray,
};
use crate::{
    common::Real,
    materials::material::Material,
    math::{vector::Vector, vector3::Vector3},
};

#[derive(Debug)]
pub struct Sphere<'a> {
    position: Vector3,
    radius: Real,
    material: &'a dyn Material<'a>,
}

impl<'a> Geometry<'a> for Sphere<'a> {
    fn get_material(&self) -> &'a dyn Material {
        self.material
    }

    fn calculate_normal(&self, _ray: &Ray, hit_position: &Vector3) -> Vector3 {
        Vector3::normalize(&(hit_position - self.position))
    }
}

impl<'a> IntersectableGeometry<'a> for Sphere<'a> {
    fn intersect(&'a self, ray: &Ray) -> Option<IntersectionResult> {
        let v = ray.position() - self.position;

        let a = ray.direction() ^ ray.direction();
        let b = v ^ ray.direction();
        let c = (v ^ v) - (self.radius * self.radius);

        let discriminant = (b * b) - (a * c);
        if discriminant < 0.0 {
            return None;
        }

        let discriminant_sqrt = Real::sqrt(discriminant);

        let reciprocal_a = Real::recip(a);
        let negative_b = -b;

        let exit_distance = (negative_b + discriminant_sqrt) * reciprocal_a;
        if exit_distance < 0.0 {
            return None;
        }

        let entrance_distance = (negative_b + discriminant_sqrt) * reciprocal_a;

        Some(IntersectionResult::new(
            self,
            entrance_distance,
            exit_distance,
            0.0,
            None,
        ))
    }
}

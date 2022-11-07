use super::{
    has_material::HasMaterial, intersectable::Intersectable, intersection::Intersection,
    normal_calculator::NormalCalculator, ray::Ray,
};
use crate::{
    common::Real,
    materials::material::MaterialIndex,
    math::{vector::Vector, vector3::Vector3},
};

#[derive(Debug)]
pub struct Sphere {
    position: Vector3,
    radius: Real,
    material_index: MaterialIndex,
}

impl Sphere {
    pub fn new(position: &Vector3, radius: Real, material_index: MaterialIndex) -> Self {
        Self {
            position: *position,
            radius,
            material_index,
        }
    }
}

impl HasMaterial for Sphere {
    fn material_index(&self) -> MaterialIndex {
        self.material_index
    }
}

impl NormalCalculator for Sphere {
    fn calculate_normal(&self, _ray: &Ray, hit_position: &Vector3) -> Vector3 {
        Vector3::normalize(&(hit_position - self.position))
    }
}

impl Intersectable for Sphere {
    fn intersect(&self, ray: &Ray) -> Option<Intersection> {
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

        Some(Intersection::new(
            self,
            entrance_distance,
            exit_distance,
            0.0,
            0,
        ))
    }
}

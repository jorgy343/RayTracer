use super::material::Material;
use crate::{
    common::{Real, EPSILON, NORMAL_BUMP},
    geometries::{geometry::Geometry, ray::Ray},
    math::{color3::Color3, vector::Vector, vector3::Vector3},
    normalize,
    scene::Scene,
};

#[derive(Debug)]
pub struct RefractiveMaterial {
    refraction_index: Real,
}

impl RefractiveMaterial {
    pub fn new(refraction_index: Real) -> Self {
        Self { refraction_index }
    }
}

impl Material for RefractiveMaterial {
    fn calculate_rendering_equation(
        &self,
        scene: &Scene,
        current_depth: u16,
        hit_geometry: &dyn Geometry,
        hit_position: &Vector3,
        hit_normal: &Vector3,
        incoming_direction: &Vector3,
    ) -> Color3 {
        let refraction_direction =
            Vector3::refract(incoming_direction, hit_normal, 1.0, self.refraction_index);

        if refraction_direction.length_squared() < EPSILON {
            return Color3::default();
        }

        let refraction_direction_normalized = normalize!(refraction_direction);

        let refraction_ray = Ray::new(hit_position, &refraction_direction_normalized);
        let intersection_opt = hit_geometry.intersect(&refraction_ray);

        if intersection_opt.is_none() {
            return Color3::default();
        }

        let intersection = intersection_opt.unwrap();
        let mut exit_position = refraction_ray.position_along(intersection.exit_distance);

        // Reverse the refraction direction so that the CalculateNormal method will see the ray
        // as coming in towards the geometry rather than coming out of it.
        let refraction_ray = Ray::new(refraction_ray.position(), &-refraction_ray.direction());

        // Because we flipped the refraction direction, the normal should be pointing away
        // from the geometry.
        let exit_normal = hit_geometry.calculate_normal(&refraction_ray, hit_position);
        exit_position += exit_normal * NORMAL_BUMP;

        // Create the outgoing ray. Use the non reversed refraction direction and the reversed
        // exit normal.
        let outgoing_direction = Vector3::refract(
            &refraction_direction,
            &-exit_normal,
            self.refraction_index,
            1.0,
        );

        if outgoing_direction.length_squared() < EPSILON {
            return Color3::default();
        }

        let outgoing_direction = normalize!(outgoing_direction);
        let outgoing_ray = Ray::new(&exit_position, &outgoing_direction);

        scene.cast_ray_color_depth(&outgoing_ray, current_depth + 1)
    }
}

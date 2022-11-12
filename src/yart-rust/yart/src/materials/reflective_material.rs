use super::material::Material;
use crate::{
    geometries::{geometry::Geometry, ray::Ray},
    math::{color3::Color3, vector3::Vector3},
    normalize,
    scene::Scene,
};

#[derive(Debug)]
pub struct ReflectiveMaterial {}

impl ReflectiveMaterial {
    pub fn new() -> Self {
        Self {}
    }
}

impl Material for ReflectiveMaterial {
    fn calculate_rendering_equation(
        &self,
        scene: &Scene,
        current_depth: u16,
        _hit_geometry: &dyn Geometry,
        hit_position: &Vector3,
        hit_normal: &Vector3,
        incoming_direction: &Vector3,
    ) -> Color3 {
        let reflected_direction = normalize!(incoming_direction.reflect(hit_normal));
        let outgoing_ray = Ray::new(hit_position, &reflected_direction);

        scene.cast_ray_color_depth(&outgoing_ray, current_depth + 1)
    }
}

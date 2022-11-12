use super::material::Material;
use crate::{
    geometries::geometry::Geometry,
    math::{color3::Color3, vector3::Vector3},
    scene::Scene,
};

#[derive(Debug)]
pub struct EmissiveMaterial {
    pub emissive_color: Color3,
}

impl EmissiveMaterial {
    pub fn new(emissive_color: &Color3) -> Self {
        Self {
            emissive_color: *emissive_color,
        }
    }
}

impl Material for EmissiveMaterial {
    fn calculate_rendering_equation(
        &self,
        _scene: &Scene,
        _current_depth: u16,
        _hit_geometry: &dyn Geometry,
        _hit_position: &Vector3,
        _hit_normal: &Vector3,
        _incoming_direction: &Vector3,
    ) -> Color3 {
        self.emissive_color
    }
}

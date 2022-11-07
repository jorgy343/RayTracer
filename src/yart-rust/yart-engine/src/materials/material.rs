use crate::{
    geometries::geometry::Geometry,
    math::{color3::Color3, vector3::Vector3},
    scene::Scene,
};
use std::fmt::Debug;

pub type MaterialIndex = usize;

pub trait Material: Debug {
    fn calculate_rendering_equation(
        &self,
        scene: &Scene,
        current_depth: u16,
        hit_geometry: &dyn Geometry,
        hit_position: &Vector3,
        hit_normal: &Vector3,
        incoming_direction: &Vector3,
    ) -> Color3;
}

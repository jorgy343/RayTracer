use crate::{geometries::geometry::Geometry, math::color3::Color3, scene::Scene};
use std::fmt::Debug;

pub trait Material<'a>: Debug {
    fn calculate_rendering_equation(
        &self,
        scene: &Scene,
        current_depth: u16,
        hit_geometry: &'a dyn Geometry<'a>,
    ) -> Color3;
}

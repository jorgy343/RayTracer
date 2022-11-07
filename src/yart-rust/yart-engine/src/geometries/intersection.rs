use super::geometry::Geometry;
use crate::{common::Real, materials::material::MaterialIndex};

#[derive(Debug, Copy, Clone)]
pub struct Intersection<'g> {
    pub hit_geometry: &'g dyn Geometry,
    pub entrance_distance: Real,
    pub exit_distance: Real,
    pub mix_amount: Real,
    pub material_index_override: MaterialIndex,
}

impl<'g> Intersection<'g> {
    pub fn new(
        hit_geometry: &'g dyn Geometry,
        entrance_distance: Real,
        exit_distance: Real,
        mix_amount: Real,
        material_index_override: MaterialIndex,
    ) -> Intersection<'g> {
        Self {
            hit_geometry,
            entrance_distance,
            exit_distance,
            mix_amount,
            material_index_override,
        }
    }
}

use super::geometry::Geometry;
use crate::{common::Real, materials::material::Material};

#[derive(Debug, Copy, Clone)]
pub struct IntersectionResult<'a> {
    pub hit_geometry: &'a dyn Geometry<'a>,
    pub entrance_distance: Real,
    pub exit_distance: Real,
    pub mix_amount: Real,
    pub material_override: Option<&'a dyn Material<'a>>,
}

impl<'a> IntersectionResult<'a> {
    pub fn new(
        hit_geometry: &'a dyn Geometry<'a>,
        entrance_distance: Real,
        exit_distance: Real,
        mix_amount: Real,
        material_override: Option<&'a dyn Material<'a>>,
    ) -> IntersectionResult<'a> {
        Self {
            hit_geometry,
            entrance_distance,
            exit_distance,
            mix_amount,
            material_override,
        }
    }
}

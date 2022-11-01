use super::{
    has_material::HasMaterial, intersectable::Intersectable, normal_calculator::NormalCalculator,
};

pub trait Geometry: Intersectable + HasMaterial + NormalCalculator {}

impl<T> Geometry for T where T: Intersectable + HasMaterial + NormalCalculator {}

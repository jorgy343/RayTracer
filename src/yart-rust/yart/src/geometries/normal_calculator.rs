use super::ray::Ray;
use crate::math::vector3::Vector3;
use std::fmt::Debug;

pub trait NormalCalculator: Debug {
    fn calculate_normal(&self, ray: &Ray, hit_position: &Vector3) -> Vector3;
}

use super::ray::Ray;
use crate::{materials::material::Material, math::vector3::Vector3};
use std::fmt::Debug;

pub trait Geometry<'a>: Debug {
    fn get_material(&self) -> &'a dyn Material;
    fn calculate_normal(&self, ray: &Ray, hit_position: &Vector3) -> Vector3;
}

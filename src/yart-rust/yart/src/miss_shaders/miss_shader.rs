use crate::{geometries::ray::Ray, math::color3::Color3};

pub trait MissShader {
    fn calculate_color(&self, ray: &Ray) -> Color3;
}

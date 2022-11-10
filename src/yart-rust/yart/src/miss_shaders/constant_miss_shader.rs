use crate::math::color3::Color3;

use super::miss_shader::MissShader;

#[derive(Debug)]
pub struct ConstantMissShader {
    pub color: Color3,
}

impl ConstantMissShader {
    pub fn new(color: &Color3) -> Self {
        Self { color: *color }
    }
}

impl MissShader for ConstantMissShader {
    fn calculate_color(&self, _ray: &crate::geometries::ray::Ray) -> Color3 {
        self.color
    }
}

use crate::{
    common::Real,
    geometries::{intersectable_geometry::IntersectableGeometry, ray::Ray},
    lights::light::Light,
    materials::material::Material,
    math::color3::Color3,
    miss_shaders::miss_shader::MissShader,
};

pub struct Scene<'a> {
    pub materials: Vec<Box<dyn Material<'a>>>,
    pub geometries: Vec<Box<dyn IntersectableGeometry<'a>>>,
    pub lights: Vec<Box<dyn Light>>,
    pub miss_shader: Box<dyn MissShader>,
    //pub root_geometry: &'a dyn IntersectableGeometry<'a>,
}

impl<'a> Scene<'a> {
    pub fn new(
        materials: Vec<Box<dyn Material<'a>>>,
        geometries: Vec<Box<dyn IntersectableGeometry<'a>>>,
        lights: Vec<Box<dyn Light>>,
        miss_shader: Box<dyn MissShader>,
        //root_geometry: &'a dyn IntersectableGeometry<'a>,
    ) -> Self {
        Self {
            materials,
            geometries,
            lights,
            miss_shader,
            //root_geometry,
        }
    }

    pub fn cast_ray_color(&self, ray: &Ray) -> Color3 {
        todo!()
    }

    pub fn cast_ray_distance(&self, ray: &Ray) -> Real {
        todo!()
    }
}

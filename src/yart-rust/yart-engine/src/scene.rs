use crate::{
    cameras::camera::Camera,
    common::Real,
    geometries::{intersectable::Intersectable, ray::Ray},
    lights::light::Light,
    materials::material::Material,
    math::color3::Color3,
    miss_shaders::miss_shader::MissShader,
};

pub struct Scene<'g> {
    pub camera: Box<dyn Camera>,
    pub materials: Vec<Box<dyn Material<'g>>>,
    pub lights: Vec<Box<dyn Light>>,
    pub miss_shader: Box<dyn MissShader>,
    pub root_geometry: Box<dyn Intersectable>,
}

impl<'g> Scene<'g> {
    pub fn new(
        camera: Box<dyn Camera>,
        materials: Vec<Box<dyn Material<'g>>>,
        lights: Vec<Box<dyn Light>>,
        miss_shader: Box<dyn MissShader>,
        root_geometry: Box<dyn Intersectable>,
    ) -> Self {
        Self {
            camera,
            materials,
            lights,
            miss_shader,
            root_geometry,
        }
    }

    pub fn cast_ray_color(&self, ray: &Ray) -> Color3 {
        todo!()
    }

    pub fn cast_ray_distance(&self, ray: &Ray) -> Real {
        todo!()
    }
}

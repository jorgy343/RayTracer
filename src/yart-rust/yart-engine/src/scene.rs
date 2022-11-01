use crate::{
    common::Real,
    geometries::{intersectable::Intersectable, ray::Ray},
    lights::light::Light,
    materials::material::Material,
    math::color3::Color3,
    miss_shaders::miss_shader::MissShader,
};

pub struct Scene<'g> {
    pub materials: Vec<Box<dyn Material<'g>>>,
    pub geometries: Vec<Box<dyn Intersectable>>,
    pub lights: Vec<Box<dyn Light>>,
    pub miss_shader: Box<dyn MissShader>,
    //pub root_geometry: &'a dyn Intersectable<'a>,
}

impl<'g> Scene<'g> {
    pub fn new(
        materials: Vec<Box<dyn Material<'g>>>,
        geometries: Vec<Box<dyn Intersectable>>,
        lights: Vec<Box<dyn Light>>,
        miss_shader: Box<dyn MissShader>,
        //root_geometry: &'a dyn Intersectable<'a>,
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

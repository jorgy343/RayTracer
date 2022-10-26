use crate::{common::Real, geometries::ray::Ray, math::color3::Color3, scene_data::SceneData};

pub struct Scene<'a> {
    scene_data: SceneData<'a>,
}

impl<'a> Scene<'a> {
    pub fn new(scene_data: SceneData<'a>) -> Self {
        Self { scene_data }
    }

    pub fn cast_ray_color(&self, ray: &Ray) -> Color3 {
        todo!()
    }

    pub fn cast_ray_distance(&self, ray: &Ray) -> Real {
        todo!()
    }
}

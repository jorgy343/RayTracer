use super::light::Light;
use crate::{
    common::Real,
    geometries::ray::Ray,
    math::{color3::Color3, vector3::Vector3},
    scene::Scene,
};

#[derive(Debug)]
pub struct DirectionalLight {
    pub color: Color3,
    direction: Vector3,
    reversed_direction: Vector3,
}

impl DirectionalLight {
    pub fn new(color: &Color3, direction: &Vector3) -> DirectionalLight {
        Self {
            color: *color,
            direction: *direction,
            reversed_direction: -direction,
        }
    }

    pub fn direction(&self) -> Vector3 {
        self.direction
    }

    pub fn reversed_direction(&self) -> Vector3 {
        self.reversed_direction
    }
}

impl Light for DirectionalLight {
    fn color(&self) -> Color3 {
        self.color
    }

    fn get_direction_towards_light(
        &self,
        _hit_position: &Vector3,
        _hit_normal: &Vector3,
    ) -> Vector3 {
        self.reversed_direction
    }

    fn is_in_shadow(
        &self,
        scene: &Scene,
        hit_position: &Vector3,
        _hit_normal: &Vector3,
        _direction_to_light: &Vector3,
    ) -> bool {
        let ray = Ray::new(hit_position, &self.reversed_direction);
        let distance = scene.cast_ray_distance(&ray);

        distance != Real::INFINITY
    }
}

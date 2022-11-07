use super::light::Light;
use crate::{
    common::EPSILON,
    geometries::ray::Ray,
    math::{color3::Color3, vector::Vector, vector3::Vector3},
    scene::Scene,
};

#[derive(Debug)]
pub struct PointLight {
    pub color: Color3,
    pub position: Vector3,
}

impl PointLight {
    pub fn new(color: &Color3, position: &Vector3) -> PointLight {
        Self {
            color: *color,
            position: *position,
        }
    }
}

impl Light for PointLight {
    fn color(&self) -> Color3 {
        self.color
    }

    fn get_direction_towards_light(
        &self,
        hit_position: &Vector3,
        _hit_normal: &Vector3,
    ) -> Vector3 {
        Vector3::normalize(&(self.position - hit_position))
    }

    fn is_in_shadow(
        &self,
        scene: &Scene,
        hit_position: &Vector3,
        _hit_normal: &Vector3,
        _direction_to_light: &Vector3,
    ) -> bool {
        let actual_direction_to_light = self.position - hit_position;
        let distance_to_light = actual_direction_to_light.length();

        let normalized_actual_direction_to_light = Vector3::normalize(&actual_direction_to_light);

        let ray = Ray::new(hit_position, &normalized_actual_direction_to_light);
        let distance = scene.cast_ray_distance(&ray);

        match distance {
            Some(distance_some) => distance_some >= distance_to_light - EPSILON,
            None => false,
        }
    }
}

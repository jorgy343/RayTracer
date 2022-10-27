use super::material::Material;
use crate::{
    common::Real,
    math::{color3::Color3, vector3::Vector3},
};

#[derive(Debug)]
pub struct PhongMaterial {
    pub ambient_color: Color3,
    pub diffuse_color: Color3,
    pub specular_color: Color3,

    pub shininess: Real,
}

impl PhongMaterial {
    pub fn new(
        ambient_color: Color3,
        diffuse_color: Color3,
        specular_color: Color3,
        shininess: Real,
    ) -> Self {
        Self {
            ambient_color,
            diffuse_color,
            specular_color,
            shininess,
        }
    }
}

impl<'a> Material<'a> for PhongMaterial {
    fn calculate_rendering_equation(
        &self,
        scene: &crate::scene::Scene,
        _current_depth: u16,
        _hit_geometry: &'a dyn crate::geometries::geometry::Geometry<'a>,
        hit_position: &Vector3,
        hit_normal: &Vector3,
        incoming_direction: &Vector3,
    ) -> Color3 {
        let ambient_component = self.ambient_color;
        let mut diffuse_component = Color3::default();
        let mut specular_component = Color3::default();

        for light in &scene.lights {
            let direction_to_light = light.get_direction_towards_light(hit_position, hit_normal);

            if light.is_in_shadow(scene, hit_position, hit_normal, &direction_to_light) {
                continue;
            }

            let light_dot_normal = direction_to_light ^ hit_normal;
            if light_dot_normal >= 0.0 {
                diffuse_component += light_dot_normal * self.diffuse_color * light.color();

                let reflection_direction = direction_to_light.reflect(hit_normal);
                let reflection_dot_view = reflection_direction ^ incoming_direction;

                if reflection_dot_view >= 0.0 {
                    specular_component += Real::powf(reflection_dot_view, self.shininess)
                        * self.specular_color
                        * light.color();
                }
            }
        }

        // TODO: Handle area lights.

        ambient_component + diffuse_component + specular_component
    }
}

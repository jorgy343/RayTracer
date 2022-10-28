use super::parse_math::{parse_color3, parse_vector3};
use crate::lights::{directional_light::DirectionalLight, light::Light, point_light::PointLight};
use yaml_rust::Yaml;

pub fn parse_lights(node: &Yaml) -> Vec<Box<dyn Light>> {
    let mut lights: Vec<Box<dyn Light>> = vec![];

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec().unwrap() {
            let directional_light_node = &child_node["directional"];
            let point_light_node = &child_node["point"];

            if !directional_light_node.is_badvalue() {
                lights.push(parse_directional_light(&directional_light_node))
            } else if !point_light_node.is_badvalue() {
                lights.push(parse_point_light(&point_light_node))
            }
        }
    }

    lights
}

fn parse_directional_light(node: &Yaml) -> Box<DirectionalLight> {
    let color = parse_color3(&node["color"]).unwrap();
    let direction = parse_vector3(&node["direction"]).unwrap();

    Box::new(DirectionalLight::new(&color, &direction))
}

fn parse_point_light(node: &Yaml) -> Box<PointLight> {
    let color = parse_color3(&node["color"]).unwrap();
    let position = parse_vector3(&node["position"]).unwrap();

    Box::new(PointLight::new(&color, &position))
}

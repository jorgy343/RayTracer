use super::parse_math::{parse_color3, parse_vector3};
use crate::lights::{directional_light::DirectionalLight, light::Light, point_light::PointLight};
use yaml_rust::Yaml;

pub fn parse_lights(node: &Yaml) -> Option<Vec<Box<dyn Light>>> {
    let mut lights = Vec::new();

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec()? {
            lights.push(parse_light(child_node)?);
        }
    }

    Some(lights)
}

fn parse_light(node: &Yaml) -> Option<Box<dyn Light>> {
    let directional_light_node = &node["directional"];
    let point_light_node = &node["point"];

    if !directional_light_node.is_badvalue() {
        return parse_directional_light(directional_light_node);
    } else if !point_light_node.is_badvalue() {
        return parse_point_light(point_light_node);
    }

    None
}

fn parse_directional_light(node: &Yaml) -> Option<Box<dyn Light>> {
    let color = parse_color3(&node["color"])?;
    let direction = parse_vector3(&node["direction"])?;

    Some(Box::new(DirectionalLight::new(&color, &direction)))
}

fn parse_point_light(node: &Yaml) -> Option<Box<dyn Light>> {
    let color = parse_color3(&node["color"])?;
    let position = parse_vector3(&node["position"])?;

    Some(Box::new(PointLight::new(&color, &position)))
}

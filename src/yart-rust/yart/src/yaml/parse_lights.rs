use super::parse_math::{parse_color3, parse_vector3};
use crate::lights::{directional_light::DirectionalLight, light::Light, point_light::PointLight};
use yaml_rust::Yaml;

fn create_function_map() -> Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn Light>>)> {
    let mut map: Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn Light>>)> = Vec::new();

    map.push(("directional", parse_directional_light));
    map.push(("point", parse_point_light));

    map
}

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
    for (name, function) in create_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            return function(child_node);
        }
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

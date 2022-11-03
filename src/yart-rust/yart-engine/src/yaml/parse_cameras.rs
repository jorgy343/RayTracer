use crate::cameras::{camera::Camera, perspective_camera::PerspectiveCamera};
use yaml_rust::Yaml;

use super::parse_math::{parse_vector2u32, parse_vector3};

fn create_function_map() -> Vec<(&'static str, fn(&Yaml) -> Box<dyn Camera>)> {
    let mut map: Vec<(&'static str, fn(&Yaml) -> Box<dyn Camera>)> = Vec::new();

    map.push(("perspective", parse_perspective_camera));

    map
}

pub fn parse_camera(node: &Yaml) -> Option<Box<dyn Camera>> {
    for (name, function) in create_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            return Some(function(child_node));
        }
    }

    None
}

fn parse_perspective_camera(node: &Yaml) -> Box<dyn Camera> {
    let position = parse_vector3(&node["position"]).unwrap();
    let look_at = parse_vector3(&node["lookAt"]).unwrap();
    let up = parse_vector3(&node["up"]).unwrap();

    let field_of_view = node["fov"].as_f64().unwrap();
    let screen_size = parse_vector2u32(&node["screenSize"]).unwrap();
    let subpixel_count = node["subpixelCount"].as_i64().unwrap() as u32;

    Box::new(PerspectiveCamera::new(
        &position,
        &look_at,
        &up,
        subpixel_count,
        screen_size,
        field_of_view,
    ))
}

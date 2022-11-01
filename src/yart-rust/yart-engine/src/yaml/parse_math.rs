use crate::{
    common::Real,
    math::{color3::Color3, color4::Color4, vector2::Vector2, vector3::Vector3, vector4::Vector4},
};
use yaml_rust::Yaml;

pub fn parse_vector4(node: &Yaml) -> Option<Vector4> {
    if !node.is_array() {
        return None;
    }

    let vec = node.as_vec().unwrap();

    match vec.len() {
        1 => Some(Vector4::new(
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
        )),
        4 => Some(Vector4::new(
            vec[0].as_f64().unwrap() as Real,
            vec[1].as_f64().unwrap() as Real,
            vec[2].as_f64().unwrap() as Real,
            vec[3].as_f64().unwrap() as Real,
        )),
        _ => None,
    }
}

pub fn parse_vector3(node: &Yaml) -> Option<Vector3> {
    if !node.is_array() {
        return None;
    }

    let vec = node.as_vec().unwrap();

    match vec.len() {
        1 => Some(Vector3::new(
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
        )),
        3 => Some(Vector3::new(
            vec[0].as_f64().unwrap() as Real,
            vec[1].as_f64().unwrap() as Real,
            vec[2].as_f64().unwrap() as Real,
        )),
        _ => None,
    }
}

pub fn parse_vector2(node: &Yaml) -> Option<Vector2> {
    if !node.is_array() {
        return None;
    }

    let vec = node.as_vec().unwrap();

    match vec.len() {
        1 => Some(Vector2::new(
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
        )),
        2 => Some(Vector2::new(
            vec[0].as_f64().unwrap() as Real,
            vec[1].as_f64().unwrap() as Real,
        )),
        _ => None,
    }
}

pub fn parse_color4(node: &Yaml) -> Option<Color4> {
    if !node.is_array() {
        return None;
    }

    let vec = node.as_vec().unwrap();

    match vec.len() {
        1 => Some(Color4::new(
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
        )),
        4 => Some(Color4::new(
            vec[0].as_f64().unwrap() as Real,
            vec[1].as_f64().unwrap() as Real,
            vec[2].as_f64().unwrap() as Real,
            vec[3].as_f64().unwrap() as Real,
        )),
        _ => None,
    }
}

pub fn parse_color3(node: &Yaml) -> Option<Color3> {
    if !node.is_array() {
        return None;
    }

    let vec = node.as_vec().unwrap();

    match vec.len() {
        1 => Some(Color3::new(
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
            vec[0].as_f64().unwrap() as Real,
        )),
        3 => Some(Color3::new(
            vec[0].as_f64().unwrap() as Real,
            vec[1].as_f64().unwrap() as Real,
            vec[2].as_f64().unwrap() as Real,
        )),
        _ => None,
    }
}

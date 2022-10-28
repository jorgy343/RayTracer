use super::parse_math::parse_color3;
use crate::{
    common::Real,
    materials::{material::Material, phong_material::PhongMaterial},
};
use std::collections::HashMap;
use yaml_rust::Yaml;

pub fn parse_materials(node: &Yaml) -> HashMap<String, Box<dyn Material>> {
    let mut material_map: HashMap<String, Box<dyn Material>> = HashMap::new();

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec().unwrap() {
            let (name, material) = parse_material(child_node).unwrap();
            material_map.insert(name, material);
        }
    }

    material_map
}

fn parse_material(node: &Yaml) -> Option<(String, Box<dyn Material>)> {
    for child_node in node.as_vec().unwrap() {
        let phong_material_node = &child_node["phong"];

        let name = child_node["name"].as_str().unwrap().to_string();

        if !phong_material_node.is_badvalue() {
            let material = parse_phong(child_node);
            return Some((name, material));
        }
    }

    None
}

fn parse_phong(node: &Yaml) -> Box<dyn Material> {
    let ambient_color = parse_color3(&node["ambientColor"]).unwrap();
    let diffuse_color = parse_color3(&node["diffuseColor"]).unwrap();
    let specular_color = parse_color3(&node["specularColor"]).unwrap();

    let shininess = node["shininess"].as_f64().unwrap() as Real;

    Box::new(PhongMaterial::new(
        &ambient_color,
        &diffuse_color,
        &specular_color,
        shininess,
    ))
}

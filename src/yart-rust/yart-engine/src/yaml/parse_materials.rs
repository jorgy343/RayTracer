use super::parse_math::parse_color3;
use crate::{
    common::Real,
    materials::{
        material::{Material, MaterialIndex},
        phong_material::PhongMaterial,
    },
    math::color3::Color3,
};
use std::collections::HashMap;
use yaml_rust::Yaml;

pub fn parse_materials<'g>(
    node: &Yaml,
) -> Option<(Vec<Box<dyn Material<'g>>>, HashMap<String, MaterialIndex>)> {
    let mut materials = Vec::new();
    let mut material_name_to_index_map = HashMap::new();

    materials.push(create_default_material());

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec()? {
            let (name, material) = parse_material(child_node)?;

            material_name_to_index_map.insert(name, materials.len() as MaterialIndex);
            materials.push(material);
        }
    }

    Some((materials, material_name_to_index_map))
}

fn create_default_material<'g>() -> Box<dyn Material<'g>> {
    // TODO: Change this to a black emissive material when emissive materials are supported.
    Box::new(PhongMaterial::new(
        &Color3::from_value(0.0),
        &Color3::from_value(0.0),
        &Color3::from_value(0.0),
        0.0,
    ))
}

fn parse_material<'g>(node: &Yaml) -> Option<(String, Box<dyn Material<'g>>)> {
    let phong_material_node = &node["phong"];

    if !phong_material_node.is_badvalue() {
        let material = parse_phong(phong_material_node)?;
        let name = phong_material_node["name"].as_str()?.to_string();

        return Some((name, material));
    }

    None
}

fn parse_phong<'g>(node: &Yaml) -> Option<Box<dyn Material<'g>>> {
    let ambient_color = parse_color3(&node["ambientColor"])?;
    let diffuse_color = parse_color3(&node["diffuseColor"])?;
    let specular_color = parse_color3(&node["specularColor"])?;

    let shininess = node["shininess"].as_f64()? as Real;

    Some(Box::new(PhongMaterial::new(
        &ambient_color,
        &diffuse_color,
        &specular_color,
        shininess,
    )))
}

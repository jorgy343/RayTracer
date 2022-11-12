use super::parse_math::parse_color3;
use crate::{
    common::Real,
    materials::{
        emissive_material::EmissiveMaterial,
        material::{Material, MaterialIndex},
        phong_material::PhongMaterial,
        reflective_material::ReflectiveMaterial,
        refractive_material::RefractiveMaterial,
    },
    math::color3::Color3,
};
use std::collections::HashMap;
use yaml_rust::Yaml;

fn create_function_map() -> Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn Material>>)> {
    let mut map: Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn Material>>)> = Vec::new();

    map.push(("emissive", parse_emissive));
    map.push(("phong", parse_phong));
    map.push(("reflective", parse_reflective));
    map.push(("refractive", parse_refractive));

    map
}

pub fn parse_materials(
    node: &Yaml,
) -> Option<(Vec<Box<dyn Material>>, HashMap<String, MaterialIndex>)> {
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

fn create_default_material() -> Box<dyn Material> {
    Box::new(EmissiveMaterial::new(&Color3::from_value(0.0)))
}

fn parse_material(node: &Yaml) -> Option<(String, Box<dyn Material>)> {
    for (name, function) in create_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            let material = function(child_node)?;
            let name = child_node["name"].as_str()?.to_string();

            return Some((name, material));
        }
    }

    None
}

fn parse_emissive(node: &Yaml) -> Option<Box<dyn Material>> {
    let emissive_color = parse_color3(&node["emissiveColor"])?;

    Some(Box::new(EmissiveMaterial::new(&emissive_color)))
}

fn parse_phong(node: &Yaml) -> Option<Box<dyn Material>> {
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

fn parse_reflective(_node: &Yaml) -> Option<Box<dyn Material>> {
    Some(Box::new(ReflectiveMaterial::new()))
}

fn parse_refractive(node: &Yaml) -> Option<Box<dyn Material>> {
    let refractive_index = node["refractiveIndex"].as_f64()? as Real;

    Some(Box::new(RefractiveMaterial::new(refractive_index)))
}

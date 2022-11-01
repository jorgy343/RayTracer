use super::parse_math::parse_vector3;
use crate::{
    common::Real,
    geometries::{intersectable::Intersectable, sphere::Sphere},
    materials::material::MaterialIndex,
};
use std::collections::HashMap;
use yaml_rust::Yaml;

pub fn parse_geometries<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Vec<Box<dyn Intersectable>> {
    let mut geometries: Vec<Box<dyn Intersectable>> = vec![];

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec().unwrap() {
            geometries.push(parse_geometry(child_node, material_name_to_index_map).unwrap());
        }
    }

    geometries
}

fn parse_geometry<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Option<Box<dyn Intersectable>> {
    let geometry_collection_node = &node["geometryCollection"];
    let sphere_node = &node["sphere"];

    if !geometry_collection_node.is_badvalue() {
    } else if !sphere_node.is_badvalue() {
        return Some(parse_sphere(node, material_name_to_index_map));
    }

    None
}

fn parse_sphere<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Box<dyn Intersectable> {
    let material_name = node["material"].as_str().unwrap();

    let position = parse_vector3(&node["position"]).unwrap();
    let radius = node["radius"].as_f64().unwrap() as Real;

    let material_index = match material_name_to_index_map.get(material_name) {
        Some(x) => *x,
        None => 0 as MaterialIndex,
    };

    Box::new(Sphere::new(position, radius, material_index))
}

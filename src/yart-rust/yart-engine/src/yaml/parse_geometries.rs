use super::parse_math::parse_vector3;
use crate::{
    common::Real,
    geometries::{
        geometry::Geometry, intersectable::Intersectable,
        intersectable_collection::IntersectableCollection, sphere::Sphere,
    },
    materials::material::MaterialIndex,
};
use std::collections::HashMap;
use yaml_rust::Yaml;

enum GeometryEnum {
    Intersectable(Box<dyn Intersectable>),
    Geometry(Box<dyn Geometry>),
}

fn create_intersectable_function_map<'a>() -> Vec<(
    &'static str,
    fn(&Yaml, &'a HashMap<String, MaterialIndex>) -> GeometryEnum,
)> {
    let mut map: Vec<(
        &'static str,
        fn(&Yaml, &'a HashMap<String, MaterialIndex>) -> GeometryEnum,
    )> = Vec::new();

    map.push(("sphere", parse_sphere));
    map.push(("collection", parse_intersectable_collection));

    map
}

fn create_geometry_function_map<'a>() -> Vec<(
    &'static str,
    fn(&Yaml, &'a HashMap<String, MaterialIndex>) -> GeometryEnum,
)> {
    let mut map: Vec<(
        &'static str,
        fn(&Yaml, &'a HashMap<String, MaterialIndex>) -> GeometryEnum,
    )> = Vec::new();

    map.push(("sphere", parse_sphere));

    map
}

pub fn parse_intersectable<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Option<Box<dyn Intersectable>> {
    let mut found_geometry_enum: Option<GeometryEnum> = None;

    for (name, function) in create_intersectable_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            found_geometry_enum = Some(function(child_node, material_name_to_index_map));
        }
    }

    match found_geometry_enum {
        Some(geometry_enum) => match geometry_enum {
            GeometryEnum::Intersectable(intersectable) => Some(intersectable),
            GeometryEnum::Geometry(geometry) => Some(geometry as Box<dyn Intersectable>),
        },
        None => None,
    }
}

fn parse_intersectables<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Vec<Box<dyn Intersectable>> {
    let mut intersectables = Vec::new();

    if !node.is_badvalue() && node.is_array() {
        for child_node in node.as_vec().unwrap() {
            intersectables
                .push(parse_intersectable(child_node, material_name_to_index_map).unwrap());
        }
    }

    intersectables
}

fn parse_geometry<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> Option<Box<dyn Geometry>> {
    let mut found_geometry_enum: Option<GeometryEnum> = None;

    for (name, function) in create_geometry_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            found_geometry_enum = Some(function(child_node, material_name_to_index_map));
        }
    }

    match found_geometry_enum {
        Some(geometry_enum) => match geometry_enum {
            GeometryEnum::Intersectable(_) => None,
            GeometryEnum::Geometry(geometry) => Some(geometry),
        },
        None => None,
    }
}

fn parse_sphere<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> GeometryEnum {
    let material_name = node["material"].as_str().unwrap();

    let position = parse_vector3(&node["position"]).unwrap();
    let radius = node["radius"].as_f64().unwrap() as Real;

    let material_index = match material_name_to_index_map.get(material_name) {
        Some(x) => *x,
        None => 0 as MaterialIndex,
    };

    GeometryEnum::Geometry(Box::new(Sphere::new(position, radius, material_index)))
}

fn parse_intersectable_collection<'a>(
    node: &Yaml,
    material_name_to_index_map: &'a HashMap<String, MaterialIndex>,
) -> GeometryEnum {
    let children = parse_intersectables(&node["children"], material_name_to_index_map);

    GeometryEnum::Intersectable(Box::new(IntersectableCollection::new(children)))
}

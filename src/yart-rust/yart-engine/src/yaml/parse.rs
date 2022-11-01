use super::{
    parse_geometries::parse_geometries, parse_lights::parse_lights,
    parse_materials::parse_materials, parse_miss_shaders::parse_miss_shader,
};
use crate::scene::Scene;
use std::fs::{self};
use yaml_rust::{Yaml, YamlLoader};

pub fn load_scene<'g>() -> Scene<'g> {
    let yaml_data = fs::read_to_string("../../../../scenes/rust-scene.yaml").unwrap();
    let doc = YamlLoader::load_from_str(yaml_data.as_str()).unwrap();

    let scene = parse_scene(&doc[0]);
    scene
}

fn parse_scene<'g>(node: &Yaml) -> Scene<'g> {
    let miss_shader = parse_miss_shader(&node["missShader"]).unwrap();
    let lights = parse_lights(&node["lights"]);
    let (materials, material_name_to_index_map) = parse_materials(&node["materials"]);
    let geometries = parse_geometries(&node["geometry"], &material_name_to_index_map);

    Scene::new(materials, geometries, lights, miss_shader)
}

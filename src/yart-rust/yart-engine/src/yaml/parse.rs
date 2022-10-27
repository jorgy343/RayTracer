use super::{parse_lights::parse_lights, parse_miss_shaders::parse_miss_shader};
use crate::{
    geometries::intersectable_geometry::IntersectableGeometry, materials::material::Material,
    scene::Scene,
};
use std::fs::{self};
use yaml_rust::{Yaml, YamlLoader};

pub fn load_scene<'a>() -> Scene<'a> {
    let yaml_data = fs::read_to_string("scene1.yaml").unwrap();
    let doc = YamlLoader::load_from_str(yaml_data.as_str()).unwrap();

    parse_scene(&doc[0])
}

fn parse_scene<'a>(node: &Yaml) -> Scene<'a> {
    let miss_shader = parse_miss_shader(&node["missShader"]).unwrap();
    let lights = parse_lights(&node["lights"]);

    let materials: Vec<Box<dyn Material<'a>>> = vec![];
    let geometries: Vec<Box<dyn IntersectableGeometry<'a>>> = vec![];

    Scene::new(materials, geometries, lights, miss_shader)
}

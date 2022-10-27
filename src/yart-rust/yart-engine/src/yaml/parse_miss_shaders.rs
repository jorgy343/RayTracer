use super::parse_math::parse_color3;
use crate::miss_shaders::{constant_miss_shader::ConstantMissShader, miss_shader::MissShader};
use yaml_rust::Yaml;

pub fn parse_miss_shader<'a>(node: &Yaml) -> Option<Box<dyn MissShader>> {
    let constant_node = &node["constant"];

    if !constant_node.is_badvalue() {
        Some(parse_constant_miss_shader(&constant_node))
    } else {
        None
    }
}

fn parse_constant_miss_shader<'a>(node: &Yaml) -> Box<ConstantMissShader> {
    let color = parse_color3(&node["color"]).unwrap();

    Box::new(ConstantMissShader::new(&color))
}

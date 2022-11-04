use super::parse_math::parse_color3;
use crate::miss_shaders::{constant_miss_shader::ConstantMissShader, miss_shader::MissShader};
use yaml_rust::Yaml;

pub fn parse_miss_shader(node: &Yaml) -> Option<Box<dyn MissShader>> {
    let constant_node = &node["constant"];

    if !constant_node.is_badvalue() {
        parse_constant_miss_shader(&constant_node)
    } else {
        None
    }
}

fn parse_constant_miss_shader(node: &Yaml) -> Option<Box<dyn MissShader>> {
    let color = parse_color3(&node["color"])?;

    Some(Box::new(ConstantMissShader::new(&color)))
}

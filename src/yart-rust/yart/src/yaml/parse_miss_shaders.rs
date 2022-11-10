use super::parse_math::parse_color3;
use crate::miss_shaders::{constant_miss_shader::ConstantMissShader, miss_shader::MissShader};
use yaml_rust::Yaml;

fn create_function_map() -> Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn MissShader>>)> {
    let mut map: Vec<(&'static str, fn(&Yaml) -> Option<Box<dyn MissShader>>)> = Vec::new();

    map.push(("constant", parse_constant_miss_shader));

    map
}

pub fn parse_miss_shader(node: &Yaml) -> Option<Box<dyn MissShader>> {
    for (name, function) in create_function_map() {
        let child_node = &node[name];

        if !child_node.is_badvalue() {
            return function(child_node);
        }
    }

    None
}

fn parse_constant_miss_shader(node: &Yaml) -> Option<Box<dyn MissShader>> {
    let color = parse_color3(&node["color"])?;

    Some(Box::new(ConstantMissShader::new(&color)))
}

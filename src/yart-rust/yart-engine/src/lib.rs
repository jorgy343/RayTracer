#![feature(trait_upcasting)]

pub mod cameras;
pub mod common;
pub mod geometries;
pub mod lights;
pub mod materials;
pub mod math;
pub mod miss_shaders;
pub mod scene;
pub mod yaml;

use yaml::parse::load_scene;

#[no_mangle]
pub fn create_scene() {
    let scene = load_scene();
}

#![feature(test)]

extern crate test;

use std::path::Path;
use test::Bencher;
use yart::{self, math::color3::Color3, yaml::parse::load_scene};

#[bench]
fn scene_bench(bench: &mut Bencher) {
    const WIDTH: u32 = 800;
    const HEIGHT: u32 = 600;

    let scene = load_scene(Path::new("../../scenes/rust-scene.yaml"));

    bench.iter(|| {
        let mut color = Color3::default();

        for y in 0..HEIGHT {
            for x in 0..WIDTH {
                let ray = scene.camera.create_ray((x, y), (0, 0));
                color += scene.cast_ray_color(&ray);
            }
        }

        color
    });
}

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

use image::{ImageBuffer, Rgb, RgbImage};
use scene::Scene;
use std::error::Error;
use yaml::parse::load_scene;

fn main() -> Result<(), Box<dyn Error>> {
    let scene = load_scene();
    let pixels = render(&scene);

    write_image_file(&pixels)?;
    Ok(())
}

fn render(scene: &Scene) -> Vec<Rgb<u8>> {
    let mut pixels: Vec<Rgb<u8>> = Vec::new();

    for y in 0..1080 {
        for x in 0..1920 {
            let ray = scene.camera.create_ray((x, y), (0, 0));
            let color = scene.cast_ray_color(&ray);

            let pixel = Rgb::from([
                (color.r * 255.0) as u8,
                (color.g * 255.0) as u8,
                (color.b * 255.0) as u8,
            ]);

            pixels.push(pixel);
        }
    }

    pixels
}

fn write_image_file(pixels: &Vec<Rgb<u8>>) -> Result<(), Box<dyn Error>> {
    let mut image: RgbImage = ImageBuffer::new(1920, 1080);

    for y in 0..1080 {
        for x in 0..1920 {
            image.put_pixel(x, y, pixels[(y * 1920 + x) as usize]);
        }
    }

    image.save("test.png")?;
    Ok(())
}

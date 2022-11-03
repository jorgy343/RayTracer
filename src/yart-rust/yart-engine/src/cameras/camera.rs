use crate::geometries::ray::Ray;

pub trait Camera {
    fn create_ray(&self, pixel: (u32, u32), subpixel: (u32, u32)) -> Ray;
}

use super::camera::Camera;
use crate::{
    common::Real,
    geometries::ray::Ray,
    math::{vector::Vector, vector3::Vector3},
};

pub struct PerspectiveCamera {
    reciprical_width: Real,
    reciprical_height: Real,

    subpixel_size_x: Real,
    subpixel_size_y: Real,

    du: Vector3,
    dv: Vector3,

    upper_left_corner: Vector3,

    pub position: Vector3,
    pub look_at: Vector3,
    pub up: Vector3,

    pub screen_size: (u32, u32),
    pub field_of_view: Real,
}

impl PerspectiveCamera {
    pub fn new(
        position: &Vector3,
        look_at: &Vector3,
        up: &Vector3,
        subpixel_count: u32,
        screen_size: (u32, u32),
        field_of_view: Real,
    ) -> PerspectiveCamera {
        let forward = Vector3::normalize(&(position - look_at));

        let u = Vector3::normalize(&(up % forward));
        let v = Vector3::normalize(&(forward % u));

        let aspect_ratio = (screen_size.0 as Real) / (screen_size.1 as Real);
        let half_width = Real::tan(field_of_view * 0.5);

        let viewport_height = half_width * 2.0;
        let viewport_width = aspect_ratio * viewport_height;

        let du = viewport_width * u;
        let dv = viewport_height * v;

        let upper_left_corner = position - du * 0.5 + dv * 0.5 - forward;

        let reciprical_width = Real::recip(screen_size.0 as Real);
        let reciprical_height = Real::recip(screen_size.1 as Real);

        let subpixel_size_x = Real::recip((subpixel_count as Real) * reciprical_width);
        let subpixel_size_y = Real::recip((subpixel_count as Real) * reciprical_height);

        Self {
            reciprical_width,
            reciprical_height,
            subpixel_size_x,
            subpixel_size_y,
            du,
            dv,
            upper_left_corner,
            position: *position,
            look_at: *look_at,
            up: *up,
            screen_size,
            field_of_view,
        }
    }
}

impl Camera for PerspectiveCamera {
    fn create_ray(&self, pixel: (u32, u32), subpixel: (u32, u32)) -> Ray {
        let mut normalized_x =
            ((self.screen_size.0 as Real) - (pixel.0 as Real) - 1.0) * self.reciprical_width;

        let mut normalized_y = (pixel.1 as Real) * self.reciprical_height;

        normalized_x += (subpixel.0 as Real) * self.subpixel_size_x;
        normalized_y += (subpixel.1 as Real) * self.subpixel_size_y;

        normalized_x += 1.0 * self.subpixel_size_x;
        normalized_y += 1.0 * self.subpixel_size_y;

        let ray_direction = Vector3::normalize(
            &(self.upper_left_corner + (normalized_x * self.du)
                - (normalized_y * self.dv)
                - self.position),
        );

        Ray::new(&self.position, &ray_direction)
    }
}

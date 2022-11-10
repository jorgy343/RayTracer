use crate::{
    cameras::camera::Camera,
    common::{Real, NORMAL_BUMP},
    geometries::{intersectable::Intersectable, ray::Ray},
    lights::light::Light,
    materials::material::Material,
    math::color3::Color3,
    miss_shaders::miss_shader::MissShader,
};

pub struct Scene {
    pub camera: Box<dyn Camera>,
    pub materials: Vec<Box<dyn Material>>,
    pub lights: Vec<Box<dyn Light>>,
    pub miss_shader: Box<dyn MissShader>,
    pub root_geometry: Box<dyn Intersectable>,
}

impl Scene {
    pub fn new(
        camera: Box<dyn Camera>,
        materials: Vec<Box<dyn Material>>,
        lights: Vec<Box<dyn Light>>,
        miss_shader: Box<dyn MissShader>,
        root_geometry: Box<dyn Intersectable>,
    ) -> Self {
        Self {
            camera,
            materials,
            lights,
            miss_shader,
            root_geometry,
        }
    }

    pub fn cast_ray_color(&self, ray: &Ray) -> Color3 {
        self.cast_ray_color_depth(ray, 1)
    }

    fn cast_ray_color_depth(&self, ray: &Ray, depth: u16) -> Color3 {
        if depth > 7 {
            return Color3::default();
        }

        let intersection = self.root_geometry.intersect(ray);

        match intersection {
            Some(intersection_some) => {
                let material = if intersection_some.material_index_override > 0 {
                    self.materials
                        .get(intersection_some.material_index_override)
                } else {
                    self.materials
                        .get(intersection_some.hit_geometry.material_index())
                };

                let mut hit_position =
                    ray.position() + intersection_some.entrance_distance * ray.direction();

                let hit_normal = intersection_some
                    .hit_geometry
                    .calculate_normal(ray, &hit_position);

                hit_position += hit_normal * NORMAL_BUMP;

                match material {
                    Some(material_some) => material_some.calculate_rendering_equation(
                        self,
                        depth,
                        intersection_some.hit_geometry,
                        &hit_position,
                        &hit_normal,
                        ray.direction(),
                    ),
                    None => Color3::default(),
                }
            }
            None => self.miss_shader.calculate_color(ray),
        }
    }

    pub fn cast_ray_distance(&self, ray: &Ray) -> Option<Real> {
        self.cast_ray_distance_depth(ray, 1)
    }

    fn cast_ray_distance_depth(&self, ray: &Ray, _depth: u16) -> Option<Real> {
        let intersection = self.root_geometry.intersect(ray)?;
        Some(Real::max(0.0, intersection.entrance_distance))
    }
}

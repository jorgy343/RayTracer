use crate::{geometries::intersectable_geometry::IntersectableGeometry, lights::light::Light};

#[derive(Debug)]
pub struct SceneData<'a> {
    pub geometries: Vec<Box<&'a dyn IntersectableGeometry<'a>>>,
    pub lights: Vec<Box<&'a dyn Light>>,
}

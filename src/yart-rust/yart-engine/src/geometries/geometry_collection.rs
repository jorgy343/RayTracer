use super::intersectable_geometry::IntersectableGeometry;

#[derive(Debug)]
pub struct GeometryCollection<'a> {
    children: Vec<&'a dyn IntersectableGeometry<'a>>,
}

impl<'a> GeometryCollection<'a> {
    pub fn new(children: Vec<&'a dyn IntersectableGeometry<'a>>) -> Self {
        Self { children }
    }
}

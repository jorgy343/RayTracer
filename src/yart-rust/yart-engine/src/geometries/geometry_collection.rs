use super::{
    intersectable_geometry::IntersectableGeometry, intersection_result::IntersectionResult,
};

#[derive(Debug)]
pub struct GeometryCollection<'a> {
    children: Vec<&'a dyn IntersectableGeometry<'a>>,
}

impl<'a> GeometryCollection<'a> {
    pub fn new(children: Vec<&'a dyn IntersectableGeometry<'a>>) -> Self {
        Self { children }
    }
}

impl<'a> IntersectableGeometry<'a> for GeometryCollection<'a> {
    fn intersect(&'a self, ray: &super::ray::Ray) -> Option<IntersectionResult> {
        let mut closest_intersection: Option<IntersectionResult> = None;

        for &geometry in &self.children {
            let intersection = geometry.intersect(ray);

            closest_intersection = match closest_intersection {
                Some(closest_intersection_some) => match intersection {
                    Some(intersection_some) => {
                        if intersection_some.entrance_distance
                            < closest_intersection_some.entrance_distance
                        {
                            intersection
                        } else {
                            closest_intersection
                        }
                    }
                    None => closest_intersection,
                },
                None => closest_intersection,
            }
        }

        closest_intersection
    }
}

use super::{geometry::Geometry, intersection_result::IntersectionResult, ray::Ray};
use std::fmt::Debug;

pub trait IntersectableGeometry<'a>: Geometry<'a> + Debug {
    fn intersect(&'a self, ray: &Ray) -> Option<IntersectionResult<'a>>;

    // TODO: Uncomment when bounding box is supported.
    // fn calculate_bounding_box() -> BoundingBox;
}

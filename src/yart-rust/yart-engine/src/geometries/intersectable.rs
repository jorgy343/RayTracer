use super::{intersection::Intersection, ray::Ray};
use std::fmt::Debug;

pub trait Intersectable: Debug {
    fn intersect(&self, ray: &Ray) -> Option<Intersection>;
}

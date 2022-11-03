use super::{intersectable::Intersectable, intersection::Intersection};

#[derive(Debug)]
pub struct IntersectableCollection {
    children: Vec<Box<dyn Intersectable>>,
}

impl IntersectableCollection {
    pub fn new(children: Vec<Box<dyn Intersectable>>) -> Self {
        Self { children }
    }
}

impl Intersectable for IntersectableCollection {
    fn intersect(&self, ray: &super::ray::Ray) -> Option<Intersection> {
        let mut closest_intersection: Option<Intersection> = None;

        for geometry in &self.children {
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

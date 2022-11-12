use crate::{
    common::Real,
    math::{vector::Vector, vector3::Vector3},
};

#[derive(Debug)]
pub struct Ray {
    position: Vector3,
    direction: Vector3,
    inverse_direction: Vector3,
}

impl Ray {
    pub fn new(position: &Vector3, direction: &Vector3) -> Self {
        Self {
            position: *position,
            direction: *direction,
            inverse_direction: Vector3::reciprical(direction),
        }
    }

    // TODO: Better name?
    pub fn new2(position: &Vector3, direction: &Vector3, inverse_direction: &Vector3) -> Self {
        Self {
            position: *position,
            direction: *direction,
            inverse_direction: *inverse_direction,
        }
    }

    pub fn position(&self) -> &Vector3 {
        &self.position
    }

    pub fn direction(&self) -> &Vector3 {
        &self.direction
    }

    pub fn inverse_direction(&self) -> &Vector3 {
        &self.inverse_direction
    }

    #[inline(always)]
    pub fn position_along(&self, distance: Real) -> Vector3 {
        self.position + self.direction * distance
    }
}

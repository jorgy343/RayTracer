use crate::math::{vector::Vector, vector3::Vector3};

#[derive(Debug)]
pub struct Ray {
    position: Vector3,
    direction: Vector3,
    inverse_direction: Vector3,
}

impl Ray {
    pub fn new(position: &Vector3, direction: &Vector3) -> Self {
        Self {
            position: position.clone(),
            direction: direction.clone(),
            inverse_direction: Vector3::reciprical(direction),
        }
    }

    // TODO: Better nane?
    pub fn new2(position: &Vector3, direction: &Vector3, inverse_direction: &Vector3) -> Self {
        Self {
            position: position.clone(),
            direction: direction.clone(),
            inverse_direction: inverse_direction.clone(),
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
}

use crate::common::*;
use std::ops::Add;

pub trait Numerics {}

impl Numerics for f32{}
impl Numerics for f64{}

#[derive(Default)]
pub struct Vector3T<T> {
    x: T,
    y: T,
    z: T,
}

impl<T> Vector3T<T> {
    pub fn new(x: T, y: T, z: T) -> Self {
        Self { x, y, z }
    }
}

impl<T: Copy + Add<Output = T>> Add<T> for Vector3T<T> {
    type Output = Self;

    fn add(self, rhs: T) -> Self::Output {
        Self::Output::new(self.x + rhs, self.y + rhs, self.z + rhs)
    }
}

// impl<T: Numerics> Add<Vector3T<T>> for T {
//     type Output = Vector3T<T>;

//     fn add(self, rhs: Vector3T<T>) -> Self::Output {
//         Self::Output::new(self + rhs.x, self + rhs.y, self + rhs.z)
//     }
// }

// impl Add<Vector3T<f64>> for f64 {
//     type Output = Vector3T<f64>;

//     fn add(self, rhs: Vector3T<f64>) -> Self::Output {
//         Self::Output::new(self + rhs.x, self + rhs.y, self + rhs.z)
//     }
// }

pub type Vector3 = Vector3T<r>;

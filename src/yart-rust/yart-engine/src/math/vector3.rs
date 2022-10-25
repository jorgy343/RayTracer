use super::{vector::Vector, vector2::Vector2};
use crate::common::*;
use impl_ops::*;
use std::ops::{self, Index, IndexMut};

#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct Vector3 {
    pub x: Real,
    pub y: Real,
    pub z: Real,
}

impl Vector3 {
    pub fn new(x: Real, y: Real, z: Real) -> Self {
        Self { x, y, z }
    }

    pub fn from_value(value: Real) -> Self {
        Self::new(value, value, value)
    }

    pub fn from_vector2(vector2: &Vector2, z: Real) -> Self {
        Self::new(vector2.x, vector2.y, z)
    }

    pub fn build_perpendicular_vector(value: &Self) -> Self {
        // From: Efficient Construction of Perpendicular Vectors Without Branching.
        let a = Self::abs(value);

        let xm = if a.x - a.y < 0.0 && a.x - a.z < 0.0 {
            1u32
        } else {
            0u32
        };
        let ym = if a.y - a.z < 0.0 { 1u32 ^ xm } else { 0u32 };
        let zm = 1u32 ^ (xm | ym);

        return value % Self::new(xm as Real, ym as Real, zm as Real);
    }

    pub fn cross(left: &Self, right: &Self) -> Self {
        Self::new(
            left.y * right.z - left.z * right.y,
            left.z * right.x - left.x * right.z,
            left.x * right.y - left.y * right.x,
        )
    }

    pub fn project_onto(&self, vector_to_project_onto: &Self) -> Self {
        vector_to_project_onto
            * ((self ^ vector_to_project_onto) / (vector_to_project_onto ^ vector_to_project_onto))
    }

    pub fn reflect(&self, normal: &Self) -> Self {
        self - 2.0 * (self ^ normal) * normal
    }

    pub fn refract(
        &self,
        incoming_direction: &Self,
        normal: &Self,
        from_index: Real,
        to_index: Real,
    ) -> Self {
        let n = from_index / to_index;
        let cos = -(incoming_direction ^ normal);

        let under_sqrt_root = 1.0 - n * n * (1.0 - cos * cos);
        if under_sqrt_root < 0.0 {
            Self::default()
        } else {
            (n * incoming_direction) - (n * cos - Real::sqrt(under_sqrt_root)) * normal
        }
    }

    pub fn schlick_approximation(
        &self,
        incoming_direction: &Self,
        normal: &Self,
        from_index: Real,
        to_index: Real,
    ) -> Real {
        let r = (from_index / to_index) / (from_index + to_index);
        let r_2 = r * r;

        let cos = -(incoming_direction ^ normal);
        let x = 1.0 - cos;

        r_2 + (1.0 - r_2) * x * x * x * x * x
    }
}

impl Vector for Vector3 {
    fn abs(value: &Self) -> Self {
        Self::new(Real::abs(value.x), Real::abs(value.y), Real::abs(value.z))
    }

    fn abs_mut(&mut self) -> &Self {
        let temp = Self::abs(self);
        *self = temp;

        self
    }

    fn component_mul(left: &Self, right: &Self) -> Self {
        Self::new(left.x * right.x, left.y * right.y, left.z * right.z)
    }

    fn distance(left: &Self, right: &Self) -> Real {
        Real::sqrt(Self::distance_squared(left, right))
    }

    fn distance_squared(left: &Self, right: &Self) -> Real {
        let x = left.x - right.x;
        let y = left.y - right.y;
        let z = left.z - right.z;

        x * x + y * y + z * z
    }

    fn dot(left: &Self, right: &Self) -> Real {
        left.x * right.x + left.y * right.y + left.z * right.z
    }

    fn exp(value: &Self) -> Self {
        Self::new(Real::exp(value.x), Real::exp(value.y), Real::exp(value.z))
    }

    fn exp_mut(&mut self) -> &Self {
        let temp = Self::exp(self);
        *self = temp;

        self
    }

    fn length(&self) -> Real {
        Real::sqrt(self.length_squared())
    }

    fn length_squared(&self) -> Real {
        self.x * self.x + self.y * self.y + self.z * self.z
    }

    fn ln(value: &Self) -> Self {
        Self::new(Real::ln(value.x), Real::ln(value.y), Real::ln(value.z))
    }

    fn ln_mut(&mut self) -> &Self {
        let temp = Self::ln(self);
        *self = temp;

        self
    }

    fn max(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::max(left.x, right.x),
            Real::max(left.y, right.y),
            Real::max(left.z, right.z),
        )
    }

    fn min(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::min(left.x, right.x),
            Real::min(left.y, right.y),
            Real::min(left.z, right.z),
        )
    }

    fn normalize(value: &Self) -> Self {
        let recip_length = value.length().recip();
        Self::new(
            value.x * recip_length,
            value.y * recip_length,
            value.z * recip_length,
        )
    }

    fn normalize_mut(&mut self) -> &Self {
        let temp = Self::normalize(self);
        *self = temp;

        self
    }

    fn reciprical(value: &Self) -> Self {
        Self::new(value.x.recip(), value.y.recip(), value.z.recip())
    }

    fn reciprical_mut(&mut self) -> &Self {
        let temp = Self::reciprical(self);
        *self = temp;

        self
    }
}

// Index operators.
impl Index<usize> for Vector3 {
    type Output = Real;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.x,
            1 => &self.y,
            2 => &self.z,
            _ => &self.x,
        }
    }
}

impl IndexMut<usize> for Vector3 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.x,
            1 => &mut self.y,
            2 => &mut self.z,
            _ => &mut self.x,
        }
    }
}

// Vector unary operators.
impl_op_ex!(-|value: &Vector3| -> Vector3 { Vector3::new(-value.x, -value.y, -value.z) });

// Vector binary operators.
impl_op_ex!(+|left: &Vector3, right: &Vector3| -> Vector3 {
    Vector3::new(left.x + right.x, left.y + right.y, left.z + right.z)
});

impl_op_ex!(-|left: &Vector3, right: &Vector3| -> Vector3 {
    Vector3::new(left.x - right.x, left.y - right.y, left.z - right.z)
});

impl_op_ex!(^|left: &Vector3, right: &Vector3| -> Real {
    Vector3::dot(left, right)
});

impl_op_ex!(%|left: &Vector3, right: &Vector3| -> Vector3 {
    Vector3::cross(left, right)
});

// Vector binary assignment operators.
impl_op_ex!(+=|left: &mut Vector3, right: &Vector3| {
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
});

impl_op_ex!(-=|left: &mut Vector3, right: &Vector3| {
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
});

// Scalar binary operators.
impl_op_ex_commutative!(+|left: &Vector3, right: &Real| -> Vector3 {
    Vector3::new(left.x + right, left.y + right, left.z + right)
});

impl_op_ex!(-|left: &Vector3, right: &Real| -> Vector3 {
    Vector3::new(left.x - right, left.y - right, left.z - right)
});

impl_op_ex!(-|left: &Real, right: &Vector3| -> Vector3 {
    Vector3::new(left - right.x, left - right.y, left - right.z)
});

impl_op_ex_commutative!(*|left: &Vector3, right: &Real| -> Vector3 {
    Vector3::new(left.x * right, left.y * right, left.z * right)
});

impl_op_ex!(/|left: &Vector3, right: &Real| -> Vector3 {
    Vector3::new(left.x / right, left.y / right, left.z / right)
});

impl_op_ex!(/|left: &Real, right: &Vector3| -> Vector3 {
    Vector3::new(left / right.x, left / right.y, left / right.z)
});

// Scalar assignment binary operators.
impl_op_ex!(+=|left: &mut Vector3, right: &Real| {
    left.x += right;
    left.y += right;
    left.z += right;
});

impl_op_ex!(-=|left: &mut Vector3, right: &Real| {
    left.x -= right;
    left.y -= right;
    left.z -= right;
});

impl_op_ex!(*=|left: &mut Vector3, right: &Real| {
    left.x *= right;
    left.y *= right;
    left.z *= right;
});

impl_op_ex!(/=|left: &mut Vector3, right: &Real| {
    left.x /= right;
    left.y /= right;
    left.z /= right;
});

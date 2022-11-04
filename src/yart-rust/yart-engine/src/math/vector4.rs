use super::{color4::Color4, vector::Vector, vector2::Vector2, vector3::Vector3};
use crate::common::*;
use impl_ops::*;
use std::ops::{self, Index, IndexMut};

#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct Vector4 {
    pub x: Real,
    pub y: Real,
    pub z: Real,
    pub w: Real,
}

impl Vector4 {
    pub fn new(x: Real, y: Real, z: Real, w: Real) -> Self {
        Self { x, y, z, w }
    }

    pub fn from_value(value: Real) -> Self {
        Self::new(value, value, value, value)
    }

    pub fn from_vector2(vector2: &Vector2, z: Real, w: Real) -> Self {
        Self::new(vector2.x, vector2.y, z, w)
    }

    pub fn from_vector3(vector3: &Vector3, w: Real) -> Self {
        Self::new(vector3.x, vector3.y, vector3.z, w)
    }

    pub fn from_color4(color4: &Color4) -> Self {
        Self::new(color4.r, color4.g, color4.b, color4.a)
    }
}

impl Vector for Vector4 {
    fn abs(value: &Self) -> Self {
        Self::new(
            Real::abs(value.x),
            Real::abs(value.y),
            Real::abs(value.z),
            Real::abs(value.w),
        )
    }

    fn abs_mut(&mut self) -> &Self {
        let temp = Self::abs(self);
        *self = temp;

        self
    }

    fn component_mul(left: &Self, right: &Self) -> Self {
        Self::new(
            left.x * right.x,
            left.y * right.y,
            left.z * right.z,
            left.w * right.w,
        )
    }

    fn distance(left: &Self, right: &Self) -> Real {
        Real::sqrt(Self::distance_squared(left, right))
    }

    fn distance_squared(left: &Self, right: &Self) -> Real {
        let x = left.x - right.x;
        let y = left.y - right.y;
        let z = left.z - right.z;
        let w = left.w - right.w;

        x * x + y * y + z * z + w * w
    }

    fn dot(left: &Self, right: &Self) -> Real {
        left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w
    }

    fn exp(value: &Self) -> Self {
        Self::new(
            Real::exp(value.x),
            Real::exp(value.y),
            Real::exp(value.z),
            Real::exp(value.w),
        )
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
        self.x * self.x + self.y * self.y + self.z * self.z + self.w * self.w
    }

    fn ln(value: &Self) -> Self {
        Self::new(
            Real::ln(value.x),
            Real::ln(value.y),
            Real::ln(value.z),
            Real::ln(value.w),
        )
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
            Real::max(left.w, right.w),
        )
    }

    fn min(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::min(left.x, right.x),
            Real::min(left.y, right.y),
            Real::min(left.z, right.z),
            Real::min(left.w, right.w),
        )
    }

    fn normalize(value: &Self) -> Self {
        let recip_length = value.length().recip();
        Self::new(
            value.x * recip_length,
            value.y * recip_length,
            value.z * recip_length,
            value.w * recip_length,
        )
    }

    fn normalize_mut(&mut self) -> &Self {
        let temp = Self::normalize(self);
        *self = temp;

        self
    }

    fn reciprical(value: &Self) -> Self {
        Self::new(
            value.x.recip(),
            value.y.recip(),
            value.z.recip(),
            value.w.recip(),
        )
    }

    fn reciprical_mut(&mut self) -> &Self {
        let temp = Self::reciprical(self);
        *self = temp;

        self
    }
}

// Index operators.
impl Index<usize> for Vector4 {
    type Output = Real;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.x,
            1 => &self.y,
            2 => &self.z,
            3 => &self.w,
            _ => &self.x,
        }
    }
}

impl IndexMut<usize> for Vector4 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.x,
            1 => &mut self.y,
            2 => &mut self.z,
            3 => &mut self.w,
            _ => &mut self.x,
        }
    }
}

// Vector unary operators.
impl_op_ex!(-|value: &Vector4| -> Vector4 { Vector4::new(-value.x, -value.y, -value.z, -value.w) });

// Vector binary operators.
impl_op_ex!(+|left: &Vector4, right: &Vector4| -> Vector4 {
    Vector4::new(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w)
});

impl_op_ex!(-|left: &Vector4, right: &Vector4| -> Vector4 {
    Vector4::new(
        left.x - right.x,
        left.y - right.y,
        left.z - right.z,
        left.w - right.w,
    )
});

impl_op_ex!(^|left: &Vector4, right: &Vector4| -> Real {
    Vector4::dot(left, right)
});

// Vector binary assignment operators.
impl_op_ex!(+=|left: &mut Vector4, right: &Vector4| {
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;
    left.w += right.w;
});

impl_op_ex!(-=|left: &mut Vector4, right: &Vector4| {
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;
    left.w -= right.w;
});

// Scalar binary operators.
impl_op_ex_commutative!(+|left: &Vector4, right: &Real| -> Vector4 {
    Vector4::new(left.x + right, left.y + right, left.z + right, left.w + right)
});

impl_op_ex!(-|left: &Vector4, right: &Real| -> Vector4 {
    Vector4::new(
        left.x - right,
        left.y - right,
        left.z - right,
        left.w - right,
    )
});

impl_op_ex!(-|left: &Real, right: &Vector4| -> Vector4 {
    Vector4::new(
        left - right.x,
        left - right.y,
        left - right.z,
        left - right.w,
    )
});

impl_op_ex_commutative!(*|left: &Vector4, right: &Real| -> Vector4 {
    Vector4::new(
        left.x * right,
        left.y * right,
        left.z * right,
        left.w * right,
    )
});

impl_op_ex!(/|left: &Vector4, right: &Real| -> Vector4 {
    Vector4::new(left.x / right, left.y / right, left.z / right, left.w / right)
});

impl_op_ex!(/|left: &Real, right: &Vector4| -> Vector4 {
    Vector4::new(left / right.x, left / right.y, left / right.z, left / right.w)
});

// Scalar assignment binary operators.
impl_op_ex!(+=|left: &mut Vector4, right: &Real| {
    left.x += right;
    left.y += right;
    left.z += right;
    left.w += right;
});

impl_op_ex!(-=|left: &mut Vector4, right: &Real| {
    left.x -= right;
    left.y -= right;
    left.z -= right;
    left.w -= right;
});

impl_op_ex!(*=|left: &mut Vector4, right: &Real| {
    left.x *= right;
    left.y *= right;
    left.z *= right;
    left.w *= right;
});

impl_op_ex!(/=|left: &mut Vector4, right: &Real| {
    left.x /= right;
    left.y /= right;
    left.z /= right;
    left.w /= right;
});

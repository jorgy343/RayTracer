use super::{color::Color, vector3::Vector3};
use crate::common::*;
use impl_ops::*;
use std::ops::{self, Index, IndexMut};

#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct Color3 {
    pub r: Real,
    pub g: Real,
    pub b: Real,
}

impl Color3 {
    pub fn new(r: Real, g: Real, b: Real) -> Self {
        Self { r, g, b }
    }

    pub fn from_value(value: Real) -> Self {
        Self::new(value, value, value)
    }

    pub fn from_vector3(vector3: &Vector3) -> Self {
        Self::new(vector3.x, vector3.y, vector3.z)
    }
}

impl Color for Color3 {
    fn abs(value: &Self) -> Self {
        Self::new(Real::abs(value.r), Real::abs(value.g), Real::abs(value.b))
    }

    fn abs_mut(&mut self) -> &Self {
        let temp = Self::abs(self);
        *self = temp;

        self
    }

    fn exp(value: &Self) -> Self {
        Self::new(Real::exp(value.r), Real::exp(value.g), Real::exp(value.b))
    }

    fn exp_mut(&mut self) -> &Self {
        let temp = Self::exp(self);
        *self = temp;

        self
    }

    fn ln(value: &Self) -> Self {
        Self::new(Real::ln(value.r), Real::ln(value.g), Real::ln(value.b))
    }

    fn ln_mut(&mut self) -> &Self {
        let temp = Self::ln(self);
        *self = temp;

        self
    }

    fn max(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::max(left.r, right.r),
            Real::max(left.g, right.g),
            Real::max(left.b, right.b),
        )
    }

    fn min(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::min(left.r, right.r),
            Real::min(left.g, right.g),
            Real::min(left.b, right.b),
        )
    }

    fn reciprical(value: &Self) -> Self {
        Self::new(value.r.recip(), value.g.recip(), value.b.recip())
    }

    fn reciprical_mut(&mut self) -> &Self {
        let temp = Self::reciprical(self);
        *self = temp;

        self
    }
}

// Index operators.
impl Index<usize> for Color3 {
    type Output = Real;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.r,
            1 => &self.g,
            2 => &self.g,
            _ => &self.r,
        }
    }
}

impl IndexMut<usize> for Color3 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.r,
            1 => &mut self.g,
            2 => &mut self.g,
            _ => &mut self.r,
        }
    }
}

// Vector unary operators.
impl_op_ex!(-|value: &Color3| -> Color3 { Color3::new(-value.r, -value.g, -value.b) });

// Vector binary operators.
impl_op_ex!(+|left: &Color3, right: &Color3| -> Color3 {
    Color3::new(left.r + right.r, left.g + right.g, left.b + right.b)
});

impl_op_ex!(-|left: &Color3, right: &Color3| -> Color3 {
    Color3::new(left.r - right.r, left.g - right.g, left.b - right.b)
});

impl_op_ex!(*|left: &Color3, right: &Color3| -> Color3 {
    Color3::new(left.r * right.r, left.g * right.g, left.b * right.b)
});

impl_op_ex!(/|left: &Color3, right: &Color3| -> Color3 {
    Color3::new(
        left.r / right.r,
        left.g / right.g,
        left.b / right.b,
    )
});

// Vector binary assignment operators.
impl_op_ex!(+=|left: &mut Color3, right: &Color3| {
    left.r += right.r;
    left.g += right.g;
    left.b += right.b;
});

impl_op_ex!(-=|left: &mut Color3, right: &Color3| {
    left.r -= right.r;
    left.g -= right.g;
    left.b -= right.b;
});

impl_op_ex!(*=|left: &mut Color3, right: &Color3| {
    left.r *= right.r;
    left.g *= right.g;
    left.b *= right.b;
});

impl_op_ex!(/=|left: &mut Color3, right: &Color3| {
    left.r /= right.r;
    left.g /= right.g;
    left.b /= right.b;
});

// Scalar binary operators.
impl_op_ex_commutative!(+|left: &Color3, right: &Real| -> Color3 {
    Color3::new(left.r + right, left.g + right, left.b + right)
});

impl_op_ex!(-|left: &Color3, right: &Real| -> Color3 {
    Color3::new(left.r - right, left.g - right, left.b - right)
});

impl_op_ex!(-|left: &Real, right: &Color3| -> Color3 {
    Color3::new(left - right.r, left - right.g, left - right.b)
});

impl_op_ex_commutative!(*|left: &Color3, right: &Real| -> Color3 {
    Color3::new(left.r * right, left.g * right, left.b * right)
});

impl_op_ex!(/|left: &Color3, right: &Real| -> Color3 {
    Color3::new(left.r / right, left.g / right, left.b / right)
});

impl_op_ex!(/|left: &Real, right: &Color3| -> Color3 {
    Color3::new(left / right.r, left / right.g, left / right.b)
});

// Scalar assignment binary operators.
impl_op_ex!(+=|left: &mut Color3, right: &Real| {
    left.r += right;
    left.g += right;
    left.b += right;
});

impl_op_ex!(-=|left: &mut Color3, right: &Real| {
    left.r -= right;
    left.g -= right;
    left.b -= right;
});

impl_op_ex!(*=|left: &mut Color3, right: &Real| {
    left.r *= right;
    left.g *= right;
    left.b *= right;
});

impl_op_ex!(/=|left: &mut Color3, right: &Real| {
    left.r /= right;
    left.g /= right;
    left.b /= right;
});

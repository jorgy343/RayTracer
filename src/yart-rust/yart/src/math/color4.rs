use super::{color::Color, vector4::Vector4};
use crate::common::*;
use impl_ops::*;
use std::ops::{self, Index, IndexMut};

#[derive(Default, Debug, Clone, Copy, PartialEq)]
pub struct Color4 {
    pub r: Real,
    pub g: Real,
    pub b: Real,
    pub a: Real,
}

impl Color4 {
    pub fn new(r: Real, g: Real, b: Real, a: Real) -> Self {
        Self { r, g, b, a }
    }

    pub fn from_value(value: Real) -> Self {
        Self::new(value, value, value, value)
    }

    pub fn from_vector4(vector4: &Vector4) -> Self {
        Self::new(vector4.x, vector4.y, vector4.z, vector4.w)
    }
}

impl Color for Color4 {
    fn abs(value: &Self) -> Self {
        Self::new(
            Real::abs(value.r),
            Real::abs(value.g),
            Real::abs(value.b),
            Real::abs(value.a),
        )
    }

    fn abs_mut(&mut self) -> &Self {
        let temp = Self::abs(self);
        *self = temp;

        self
    }

    fn exp(value: &Self) -> Self {
        Self::new(
            Real::exp(value.r),
            Real::exp(value.g),
            Real::exp(value.b),
            Real::exp(value.a),
        )
    }

    fn exp_mut(&mut self) -> &Self {
        let temp = Self::exp(self);
        *self = temp;

        self
    }

    fn ln(value: &Self) -> Self {
        Self::new(
            Real::ln(value.r),
            Real::ln(value.g),
            Real::ln(value.b),
            Real::ln(value.a),
        )
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
            Real::max(left.a, right.a),
        )
    }

    fn min(left: &Self, right: &Self) -> Self {
        Self::new(
            Real::min(left.r, right.r),
            Real::min(left.g, right.g),
            Real::min(left.b, right.b),
            Real::min(left.a, right.a),
        )
    }

    fn reciprical(value: &Self) -> Self {
        Self::new(
            value.r.recip(),
            value.g.recip(),
            value.b.recip(),
            value.a.recip(),
        )
    }

    fn reciprical_mut(&mut self) -> &Self {
        let temp = Self::reciprical(self);
        *self = temp;

        self
    }
}

// Index operators.
impl Index<usize> for Color4 {
    type Output = Real;

    fn index(&self, index: usize) -> &Self::Output {
        match index {
            0 => &self.r,
            1 => &self.g,
            2 => &self.g,
            3 => &self.a,
            _ => &self.r,
        }
    }
}

impl IndexMut<usize> for Color4 {
    fn index_mut(&mut self, index: usize) -> &mut Self::Output {
        match index {
            0 => &mut self.r,
            1 => &mut self.g,
            2 => &mut self.g,
            3 => &mut self.a,
            _ => &mut self.r,
        }
    }
}

// Vector unary operators.
impl_op_ex!(-|value: &Color4| -> Color4 { Color4::new(-value.r, -value.g, -value.b, -value.a) });

// Vector binary operators.
impl_op_ex!(+|left: &Color4, right: &Color4| -> Color4 {
    Color4::new(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a)
});

impl_op_ex!(-|left: &Color4, right: &Color4| -> Color4 {
    Color4::new(
        left.r - right.r,
        left.g - right.g,
        left.b - right.b,
        left.a - right.a,
    )
});

impl_op_ex!(*|left: &Color4, right: &Color4| -> Color4 {
    Color4::new(
        left.r * right.r,
        left.g * right.g,
        left.b * right.b,
        left.a * right.a,
    )
});

impl_op_ex!(/|left: &Color4, right: &Color4| -> Color4 {
    Color4::new(
        left.r / right.r,
        left.g / right.g,
        left.b / right.b,
        left.a / right.a,
    )
});

// Vector binary assignment operators.
impl_op_ex!(+=|left: &mut Color4, right: &Color4| {
    left.r += right.r;
    left.g += right.g;
    left.b += right.b;
    left.a += right.a;
});

impl_op_ex!(-=|left: &mut Color4, right: &Color4| {
    left.r -= right.r;
    left.g -= right.g;
    left.b -= right.b;
    left.a -= right.a;
});

impl_op_ex!(*=|left: &mut Color4, right: &Color4| {
    left.r *= right.r;
    left.g *= right.g;
    left.b *= right.b;
    left.a *= right.a;
});

impl_op_ex!(/=|left: &mut Color4, right: &Color4| {
    left.r /= right.r;
    left.g /= right.g;
    left.b /= right.b;
    left.a /= right.a;
});

// Scalar binary operators.
impl_op_ex_commutative!(+|left: &Color4, right: &Real| -> Color4 {
    Color4::new(left.r + right, left.g + right, left.b + right, left.a + right)
});

impl_op_ex!(-|left: &Color4, right: &Real| -> Color4 {
    Color4::new(
        left.r - right,
        left.g - right,
        left.b - right,
        left.a - right,
    )
});

impl_op_ex!(-|left: &Real, right: &Color4| -> Color4 {
    Color4::new(
        left - right.r,
        left - right.g,
        left - right.b,
        left - right.a,
    )
});

impl_op_ex_commutative!(*|left: &Color4, right: &Real| -> Color4 {
    Color4::new(
        left.r * right,
        left.g * right,
        left.b * right,
        left.a * right,
    )
});

impl_op_ex!(/|left: &Color4, right: &Real| -> Color4 {
    Color4::new(left.r / right, left.g / right, left.b / right, left.a / right)
});

impl_op_ex!(/|left: &Real, right: &Color4| -> Color4 {
    Color4::new(left / right.r, left / right.g, left / right.b, left / right.a)
});

// Scalar assignment binary operators.
impl_op_ex!(+=|left: &mut Color4, right: &Real| {
    left.r += right;
    left.g += right;
    left.b += right;
    left.a += right;
});

impl_op_ex!(-=|left: &mut Color4, right: &Real| {
    left.r -= right;
    left.g -= right;
    left.b -= right;
    left.a -= right;
});

impl_op_ex!(*=|left: &mut Color4, right: &Real| {
    left.r *= right;
    left.g *= right;
    left.b *= right;
    left.a *= right;
});

impl_op_ex!(/=|left: &mut Color4, right: &Real| {
    left.r /= right;
    left.g /= right;
    left.b /= right;
    left.a /= right;
});

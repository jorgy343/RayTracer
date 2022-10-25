use crate::common::*;

pub trait Vector {
    fn abs(value: &Self) -> Self;
    fn abs_mut(&mut self) -> &Self;

    fn component_mul(left: &Self, right: &Self) -> Self;

    fn distance(left: &Self, right: &Self) -> Real;
    fn distance_squared(left: &Self, right: &Self) -> Real;

    fn dot(left: &Self, right: &Self) -> Real;

    fn exp(value: &Self) -> Self;
    fn exp_mut(&mut self) -> &Self;

    fn length(&self) -> Real;
    fn length_squared(&self) -> Real;

    fn ln(value: &Self) -> Self;
    fn ln_mut(&mut self) -> &Self;

    fn max(left: &Self, right: &Self) -> Self;
    fn min(left: &Self, right: &Self) -> Self;

    fn normalize(value: &Self) -> Self;
    fn normalize_mut(&mut self) -> &Self;

    fn reciprical(value: &Self) -> Self;
    fn reciprical_mut(&mut self) -> &Self;
}

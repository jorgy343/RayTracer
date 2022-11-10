pub trait Color {
    fn abs(value: &Self) -> Self;
    fn abs_mut(&mut self) -> &Self;

    fn exp(value: &Self) -> Self;
    fn exp_mut(&mut self) -> &Self;

    fn ln(value: &Self) -> Self;
    fn ln_mut(&mut self) -> &Self;

    fn max(left: &Self, right: &Self) -> Self;
    fn min(left: &Self, right: &Self) -> Self;

    fn reciprical(value: &Self) -> Self;
    fn reciprical_mut(&mut self) -> &Self;
}

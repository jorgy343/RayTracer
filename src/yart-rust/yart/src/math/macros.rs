#[macro_export]
macro_rules! normalize {
    ($expr:expr) => {
        $crate::math::vector::Vector::normalize(&($expr))
    };
}

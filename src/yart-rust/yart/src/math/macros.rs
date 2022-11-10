#[macro_export]
macro_rules! normalize4 {
    ($expr:expr) => {
        $crate::math::vector4::Vector4::normalize(&($expr))
    };
}

#[macro_export]
macro_rules! normalize3 {
    ($expr:expr) => {
        $crate::math::vector3::Vector3::normalize(&($expr))
    };
}

#[macro_export]
macro_rules! normalize2 {
    ($expr:expr) => {
        $crate::math::vector2::Vector2::normalize(&($expr))
    };
}

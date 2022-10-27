pub mod common;
pub mod geometries;
pub mod lights;
pub mod materials;
pub mod math;
pub mod miss_shaders;
pub mod scene;
pub mod yaml;

use common::*;
use math::vector::*;
use math::vector3::*;

#[no_mangle]
pub fn test_abc(x: Real, y: Real, z: Real) -> Real {
    let a = Vector3::new(x, y, z);
    let b = 0.5 + a;

    let c = Vector3::abs(&b);

    return c.length();
}

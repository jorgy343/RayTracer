use crate::materials::material::MaterialIndex;
use std::fmt::Debug;

pub trait HasMaterial: Debug {
    fn material_index(&self) -> MaterialIndex;
}

use strum_macros::EnumIter;
use enum_map_derive::Enum;

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum Color {
    White,
    Black,
}

impl From<char> for Color {
    fn from(c: char) -> Self {
        match c.to_ascii_lowercase() {
            'w' => Color::White,
            'b' => Color::Black,
            _ => panic!(),
        }
    }
}

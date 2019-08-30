use enum_map_derive::Enum;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum Color {
    White,
    Black,
}

impl Color {
    pub fn set_ascii_case(self, c: char) -> char {
        match self {
            Color::White => c.to_ascii_uppercase(),
            Color::Black => c.to_ascii_lowercase(),
        }
    }
    pub fn other(self) -> Self {
        match self {
            Color::White => Color::Black,
            Color::Black => Color::White,
        }
    }
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

impl From<Color> for char {
    fn from(color: Color) -> Self {
        match color {
            Color::White => 'w',
            Color::Black => 'b',
        }
    }
}

use enum_map::enum_map;
use super::color::Color;
use super::coordinates::{Bitboard,Square, SQUARE_COUNT};
use lazy_static::lazy_static;
use enum_map_derive::Enum;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Piece {
    pub role: Role,
    pub color: Color,
}

impl From<Piece> for char {
    fn from(piece: Piece) -> Self {
        let c_by_role = enum_map! {
            Role::Pawn => 'P',
            Role::Knight => 'N',
            Role::Bishop => 'B',
            Role::Rook => 'R',
            Role::Queen => 'Q',
            Role::King => 'K',
        };
        let c = c_by_role[piece.role];
        let c_by_color = enum_map! {
            Color::White => c,
            Color::Black => c.to_ascii_lowercase(),
        };
        c_by_color[piece.color]
    }
}

impl From<char> for Piece {
    fn from(c: char) -> Self {
        Piece::new(Role::from(c), Color::from(c))
    }
}

impl Piece {
    pub fn new(role: Role, color: Color) -> Self {
        Piece { role, color }
    }
}

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum Role {
    Pawn,
    Knight,
    Bishop,
    Rook,
    King,
    Queen,
}

lazy_static! {
    pub static ref KNIGHT: [Bitboard; SQUARE_COUNT] = {
        unimplemented!()
    };
}

impl Role {
    pub fn attacks(self, from: Square) -> Bitboard {
        unimplemented!()
    }
}

impl From<char> for Role {
    fn from(c: char) -> Self {
        match c.to_ascii_uppercase() {
            'P' => Role::Pawn,
            'N' => Role::Knight,
            'B' => Role::Bishop,
            'R' => Role::Rook,
            'Q' => Role::Queen,
            'K' => Role::King,
            _ => panic!(),
        }
    }
}

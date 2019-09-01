use super::color::Color;
use super::coordinates::{Bitboard, Square, SQUARE_COUNT};
use crate::chess::coordinates::{Coordinate, ToBb};
use enum_map_derive::Enum;
use lazy_static::lazy_static;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Piece {
    pub role: Role,
    pub color: Color,
}

impl Piece {
    pub fn new(role: Role, color: Color) -> Self {
        Piece { role, color }
    }
}

impl From<Piece> for char {
    fn from(piece: Piece) -> Self {
        let c = char::from(piece.role);
        piece.color.set_ascii_case(c)
    }
}

impl From<char> for Piece {
    fn from(c: char) -> Self {
        Piece::new(Role::from(c), Color::from_char_case(c))
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

impl Role {
    pub fn attacks(self, from: Square) -> Bitboard {
        match self {
            Role::Knight => KNIGHT[from.i() as usize],
            Role::King => KING[from.i() as usize],
            _ => panic!(),
        }
    }
}

impl From<char> for Role {
    fn from(c: char) -> Self {
        match c.to_ascii_lowercase() {
            'p' => Role::Pawn,
            'n' => Role::Knight,
            'b' => Role::Bishop,
            'r' => Role::Rook,
            'q' => Role::Queen,
            'k' => Role::King,
            _ => panic!(),
        }
    }
}

impl From<Role> for char {
    fn from(role: Role) -> Self {
        match role {
            Role::Pawn => 'p',
            Role::Knight => 'n',
            Role::Bishop => 'b',
            Role::Rook => 'r',
            Role::Queen => 'q',
            Role::King => 'k',
        }
    }
}

lazy_static! {
    pub static ref KNIGHT: [Bitboard; SQUARE_COUNT] = {
        let shifts = [
            (-1, 2),
            (1, 2),
            (2, 1),
            (2, -1),
            (1, -2),
            (-1, -2),
            (-2, -1),
            (-2, 1),
        ];
        let mut bitboards = [0; SQUARE_COUNT];
        for square in Square::all() {
            let mut bb = 0;
            for shift in shifts.iter() {
                if let (Some(file), Some(rank)) =
                    (square.file().shift(shift.0), square.rank().shift(shift.1))
                {
                    bb |= Square::at(file, rank).to_bb();
                }
            }
            bitboards[square.i() as usize] = bb;
        }
        bitboards
    };
    pub static ref KING: [Bitboard; SQUARE_COUNT] = {
        let shifts = [-1, 1];
        let mut bitboards = [0; SQUARE_COUNT];
        for square in Square::all() {
            let mut files = square.file().to_bb();
            let mut ranks = square.rank().to_bb();
            for shift in shifts.iter() {
                if let Some(file) = square.file().shift(*shift) {
                    files |= file.to_bb();
                }
                if let Some(rank) = square.rank().shift(*shift) {
                    ranks |= rank.to_bb();
                }
            }
            bitboards[square.i() as usize] = (files & ranks) ^ square.to_bb();
        }
        bitboards
    };
    //pub static ref ROOK: [Bitboard; 256] = {
    //    let mut bitboards = [0; 256];
    //    for i in (0..256) {
    //        let mut bb = 0;
    //        for square in 
    //        for shift in shifts.iter() {
    //            if let (Some(file), Some(rank)) =
    //                (square.file().shift(shift.0), square.rank().shift(shift.1))
    //            {
    //                bb |= Square::at(file, rank).to_bb();
    //            }
    //        }
    //        bitboards[square.i() as usize] = bb;
    //    }
    //    bitboards
    //};
}

#[cfg(test)]
mod test {
    use super::*;
    use crate::chess::coordinates::ToBb;
    use std::str::FromStr;

    #[test]
    fn piece_from_char() {
        assert_eq!(Piece::from('N'), Piece::new(Role::Knight, Color::White));
        assert_eq!(Piece::from('q'), Piece::new(Role::Queen, Color::Black));
    }

    #[test]
    fn char_from_piece() {
        assert_eq!(char::from(Piece::new(Role::King, Color::White)), 'K');
        assert_eq!(char::from(Piece::new(Role::Pawn, Color::Black)), 'p');
    }

    #[test]
    fn knight_attacks_a2() {
        let attacker = Square::from_str("a2").unwrap();
        let attacks = Square::from_str("c1").unwrap().to_bb()
            | Square::from_str("c3").unwrap().to_bb()
            | Square::from_str("b4").unwrap().to_bb();
        assert_eq!(KNIGHT[attacker.i() as usize], attacks);
    }
    #[test]
    fn king_attacks_f8() {
        let attacker = Square::from_str("f8").unwrap();
        assert_eq!(KING[attacker.i() as usize], 0xc0_40c0_0000_0000);
    }
}

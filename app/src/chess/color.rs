use super::{Coordinate, Rank};
use crate::err::Error;
use enum_map_derive::Enum;
use std::ops;
use std::str::FromStr;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, Enum, EnumIter, Hash, PartialEq, Eq)]
pub enum Color {
    White,
    Black,
}

impl Color {
    pub fn from_char_case(c: char) -> Self {
        if c.is_ascii_uppercase() {
            Color::White
        } else {
            Color::Black
        }
    }

    pub fn set_ascii_case(self, c: char) -> char {
        match self {
            Color::White => c.to_ascii_uppercase(),
            Color::Black => c.to_ascii_lowercase(),
        }
    }

    pub fn backrank(self) -> Rank {
        match self {
            Color::White => Rank::new(0),
            Color::Black => Rank::new(7),
        }
    }

    pub fn pawn_rank(self) -> Rank {
        match self {
            Color::White => Rank::new(1),
            Color::Black => Rank::new(6),
        }
    }

    pub fn fourth_rank(self) -> Rank {
        match self {
            Color::White => Rank::new(3),
            Color::Black => Rank::new(4),
        }
    }

    pub fn seventh_rank(self) -> Rank {
        match self {
            Color::White => Rank::new(6),
            Color::Black => Rank::new(1),
        }
    }
}

impl ops::Not for Color {
    type Output = Self;

    fn not(self) -> Self::Output {
        match self {
            Color::White => Color::Black,
            Color::Black => Color::White,
        }
    }
}

impl FromStr for Color {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s.chars().next().map(|c| c.to_ascii_lowercase()) {
            Some('w') => Ok(Color::White),
            Some('b') => Ok(Color::Black),
            _ => Err(Error::InvalidColor),
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

#[cfg(test)]
mod test {
    use super::*;
    use crate::chess::{Board, File, Move, Piece, Role, Square};
    use std::str::FromStr;

    #[test]
    fn color_from_char() {
        assert_eq!(Color::from_str("w"), Ok(Color::White));
        assert_eq!(Color::from_str("B"), Ok(Color::Black));
    }

    #[test]
    fn char_from_color() {
        assert_eq!(char::from(Color::White), 'w');
        assert_eq!(char::from(Color::Black), 'b');
    }

    #[test]
    fn not_black_is_white() {
        assert_eq!(!Color::Black, Color::White);
    }

    #[test]
    fn backrank_contains_rook_in_new_board() {
        let board = Board::default();
        let square = Square::at(File::A, Color::White.backrank());
        assert_eq!(
            board.piece_opt_at(square),
            Some(Piece {
                role: Role::Rook,
                color: Color::White,
            })
        );
    }

    #[test]
    fn pawn_double_push_is_on_fourth_rank() {
        let mut board = Board::default();
        let square = Square::at(File::E, Color::White.fourth_rank());
        board.do_move(Move::from_str("e2e4").unwrap());
        assert_eq!(
            board.piece_opt_at(square),
            Some(Piece {
                role: Role::Pawn,
                color: Color::White,
            })
        );
    }
}

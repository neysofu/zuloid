use super::Rank;
use crate::err::Error;
use enum_map_derive::Enum;
use std::ops;
use std::str::FromStr;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, Enum, EnumIter, Hash, PartialEq, Eq)]
pub enum Color {
    W,
    B,
}

impl Color {
    pub fn from_char_case(c: char) -> Self {
        if c.is_ascii_uppercase() {
            Color::W
        } else {
            Color::B
        }
    }

    pub fn set_ascii_case(self, c: char) -> char {
        match self {
            Color::W => c.to_ascii_uppercase(),
            Color::B => c.to_ascii_lowercase(),
        }
    }

    pub fn backrank(self) -> Rank {
        Rank::new_with_side(0, self)
    }

    pub fn pawn_rank(self) -> Rank {
        Rank::new_with_side(1, self)
    }

    pub fn fourth_rank(self) -> Rank {
        Rank::new_with_side(3, self)
    }

    pub fn seventh_rank(self) -> Rank {
        Rank::new_with_side(6, self)
    }
}

impl ops::Not for Color {
    type Output = Self;

    fn not(self) -> Self::Output {
        match self {
            Color::W => Color::B,
            Color::B => Color::W,
        }
    }
}

impl FromStr for Color {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match s.chars().next().map(|c| c.to_ascii_lowercase()) {
            Some('w') => Ok(Color::W),
            Some('b') => Ok(Color::B),
            _ => Err(Error::InvalidColor),
        }
    }
}

impl From<Color> for char {
    fn from(color: Color) -> Self {
        match color {
            Color::W => 'w',
            Color::B => 'b',
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use crate::chess::{Board, File, Move, Piece, Role, Square};
    use std::str::FromStr;
    use strum::IntoEnumIterator;

    #[test]
    fn color_case_is_consistent() {
        for c in Color::iter() {
            assert_eq!(Color::from_char_case(c.set_ascii_case('x')), c);
        }
    }

    #[test]
    fn color_from_char() {
        assert_eq!(Color::from_str("w"), Ok(Color::W));
        assert_eq!(Color::from_str("B"), Ok(Color::B));
    }

    #[test]
    fn char_from_color() {
        assert_eq!(char::from(Color::W), 'w');
        assert_eq!(char::from(Color::B), 'b');
    }

    #[test]
    fn not_black_is_white() {
        assert_eq!(!Color::B, Color::W);
    }

    #[test]
    fn backrank_contains_rook_in_new_board() {
        let board = Board::default();
        let square = Square::at(File::A, Color::W.backrank());
        assert_eq!(
            board.piece_opt_at(square),
            Some(Piece {
                role: Role::Rook,
                color: Color::W,
            })
        );
    }

    #[test]
    fn pawn_double_push_is_on_fourth_rank() {
        let mut board = Board::default();
        let square = Square::at(File::E, Color::W.fourth_rank());
        board.do_move(Move::from_str("e2e4").unwrap());
        assert_eq!(
            board.piece_opt_at(square),
            Some(Piece {
                role: Role::Pawn,
                color: Color::W,
            })
        );
    }
}

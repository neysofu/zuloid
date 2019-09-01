use super::color::*;
use super::coordinates::*;
use super::piece::*;
use crate::result::Error;
use enum_map::{enum_map, EnumMap};
use enum_map_derive::Enum;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::str::FromStr;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

pub struct Board {
    pub bb_colors: EnumMap<Color, Bitboard>,
    pub bb_roles: EnumMap<Role, Bitboard>,
    pub castling_rights: CastlingRights,
    pub color_to_move: Color,
    pub reversible_moves_count: usize,
}

impl Board {
    pub fn empty() -> Self {
        Board {
            bb_colors: EnumMap::default(),
            bb_roles: EnumMap::default(),
            castling_rights: CastlingRights::empty(),
            ..Default::default()
        }
    }

    fn at(&self, square: Square) -> Option<Piece> {
        let bb = square.to_bb();
        let mut square_role = None;
        for role in Role::iter() {
            if self.bb_roles[role] & bb != 0 {
                square_role = Some(role);
                break;
            }
        }
        let mut square_color = None;
        for color in Color::iter() {
            if self.bb_colors[color] & bb != 0 {
                square_color = Some(color);
                break;
            }
        }
        if let (Some(role), Some(color)) = (square_role, square_color) {
            Some(Piece::new(role, color))
        } else {
            None
        }
    }

    pub fn from_fen<S: AsRef<str>>(fields: &mut impl Iterator<Item = S>) -> Self {
        let mut board = Board::empty();
        let piece_map_str = fields.next().unwrap();
        let piece_map_by_rank = piece_map_str.as_ref().split('/');
        let mut rank = Rank::max();
        for rank_piece_map in piece_map_by_rank {
            let mut file = File::min();
            for c in rank_piece_map.chars() {
                if let Some(digit) = c.to_digit(9) {
                    for _ in 0..digit {
                        let square = Square::at(file, rank);
                        board.set_at_square(square, None);
                        file = file.shift(1).unwrap();
                    }
                } else {
                    let square = Square::at(file, rank);
                    let piece = Piece::from(c);
                    board.set_at_square(square, Some(piece));
                    file = file.shift(1).unwrap();
                }
            }
            if let Some(rank_below) = rank.shift(1) {
                rank = rank_below;
            } else {
                break;
            }
        }
        let color_to_move_str = fields.next().unwrap();
        board.color_to_move = Color::from(color_to_move_str.as_ref().chars().next().unwrap());
        board.castling_rights = CastlingRights::from_str(fields.next().unwrap().as_ref()).unwrap();
        board
    }

    pub fn set_at_square(&mut self, square: Square, piece: Option<Piece>) {
        let bb = square.to_bb();
        for color in Color::iter() {
            self.bb_colors[color] &= !bb;
        }
        for role in Role::iter() {
            self.bb_roles[role] &= !bb;
        }
        if let Some(piece) = piece {
            self.bb_colors[piece.color] |= bb;
            self.bb_roles[piece.role] |= bb;
        }
    }

    pub fn do_move(&mut self, mv: Move) {
        self.set_at_square(mv.to, self.at(mv.from));
        self.set_at_square(mv.from, None);
    }
}

impl Default for Board {
    fn default() -> Self {
        let mut bb_colors = EnumMap::default();
        bb_colors[Color::White] = Rank::from('1').to_bb() | Rank::from('2').to_bb();
        bb_colors[Color::Black] = Rank::from('7').to_bb() | Rank::from('8').to_bb();
        let mut bb_roles = EnumMap::default();
        bb_roles[Role::Pawn] = Rank::from('2').to_bb() | Rank::from('7').to_bb();
        bb_roles[Role::Knight] = Square::from("b1").to_bb()
            | Square::from("g1").to_bb()
            | Square::from("b8").to_bb()
            | Square::from("g8").to_bb();
        bb_roles[Role::Bishop] = Square::from("c1").to_bb()
            | Square::from("f1").to_bb()
            | Square::from("c8").to_bb()
            | Square::from("f8").to_bb();
        bb_roles[Role::Rook] = Square::from("a1").to_bb()
            | Square::from("h1").to_bb()
            | Square::from("a8").to_bb()
            | Square::from("h8").to_bb();
        bb_roles[Role::Queen] = Square::from("d1").to_bb() | Square::from("d8").to_bb();
        bb_roles[Role::King] = Square::from("e1").to_bb() | Square::from("e8").to_bb();
        Board {
            bb_colors,
            bb_roles,
            castling_rights: CastlingRights::default(),
            color_to_move: Color::White,
            reversible_moves_count: 0,
        }
    }
}

impl PartialEq for Board {
    fn eq(&self, other: &Self) -> bool {
        self.bb_colors[Color::White] == other.bb_colors[Color::White]
            && self.bb_colors[Color::Black] == other.bb_colors[Color::Black]
            && self.bb_roles[Role::Pawn] == other.bb_roles[Role::Pawn]
            && self.bb_roles[Role::Knight] == other.bb_roles[Role::Knight]
            && self.bb_roles[Role::Bishop] == other.bb_roles[Role::Bishop]
            && self.bb_roles[Role::Rook] == other.bb_roles[Role::Rook]
            && self.bb_roles[Role::King] == other.bb_roles[Role::King]
            && self.color_to_move == other.color_to_move
    }
}

impl Eq for Board {}

impl Hash for Board {
    fn hash<H: Hasher>(&self, _state: &mut H) {
        unimplemented!()
    }
}

impl fmt::Display for Board {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(fmt, "     A B C D E F G H")?;
        writeln!(fmt, "   +-----------------+")?;
        for rank in Rank::all().rev() {
            write!(fmt, " {} | ", char::from(rank))?;
            for file in File::all() {
                let square = Square::at(file, rank);
                if let Some(piece) = self.at(square) {
                    write!(fmt, "{} ", char::from(piece))?;
                } else {
                    write!(fmt, ". ")?;
                }
            }
            writeln!(fmt, "| {}", char::from(rank))?;
        }
        writeln!(fmt, "   +-----------------+")?;
        writeln!(fmt, "     A B C D E F G H")?;
        Ok(())
    }
}

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum CastlingSide {
    King,
    Queen,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Move {
    pub from: Square,
    pub to: Square,
    pub promotion: Option<Role>,
}

impl FromStr for Move {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        assert!(s.len() >= 4);
        let from = Square::from_str(&s[0..2])?;
        let to = Square::from_str(&s[2..4])?;
        let promotion = s.chars().nth(5).map(Role::from);
        Ok(Move {
            from,
            to,
            promotion,
        })
    }
}

impl fmt::Display for Move {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", self.from)?;
        write!(fmt, "{}", self.to)
    }
}

pub struct CastlingRights(EnumMap<Color, EnumMap<CastlingSide, bool>>);

impl CastlingRights {
    pub fn empty() -> Self {
        CastlingRights(EnumMap::default())
    }
}

impl Default for CastlingRights {
    fn default() -> Self {
        let both = enum_map! {
            CastlingSide::King => true,
            CastlingSide::Queen => true,
        };
        CastlingRights(enum_map! {
            Color::White => both,
            Color::Black => both,
        })
    }
}

impl FromStr for CastlingRights {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut castling_rights = CastlingRights::empty();
        for c in s.chars() {
            match c {
                'K' => castling_rights.0[Color::White][CastlingSide::King] = true,
                'k' => castling_rights.0[Color::Black][CastlingSide::King] = true,
                'Q' => castling_rights.0[Color::White][CastlingSide::Queen] = true,
                'q' => castling_rights.0[Color::Black][CastlingSide::Queen] = true,
                _ => (),
            }
        }
        Ok(castling_rights)
    }
}

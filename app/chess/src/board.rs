use crate::color::*;
use crate::coordinates::*;
use crate::moving::Move;
use crate::piece::*;
use crate::Error;
use enum_map::{enum_map, EnumMap};
use enum_map_derive::Enum;
use lazy_static::lazy_static;
use std::fmt;
use std::str::FromStr;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

#[derive(Clone, Debug, Hash, PartialEq, Eq)]
pub struct Board {
    pub bb_colors: EnumMap<Color, BitBoard>,
    pub bb_roles: EnumMap<Role, BitBoard>,
    pub castling_rights: CastlingRights,
    pub color_to_move: Color,
    pub reversible_moves_count: usize,
    pub en_passant_target_square: Option<Square>,
    pub half_moves_counter: usize,
    pub full_moves_counter: usize,
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

    pub fn piece_opt_at(&self, square: Square) -> Option<Piece> {
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
        self.set_at_square(mv.to, self.piece_opt_at(mv.from));
        self.set_at_square(mv.from, None);
    }

    fn square_centric_chars(&self) -> SquareCentricBoard<char> {
        let mut chars_by_square_i = ['.'; Square::count()];
        for square in Square::iter() {
            if let Some(piece) = self.piece_opt_at(square) {
                chars_by_square_i[square.i()] = char::from(piece);
            }
        }
        SquareCentricBoard::from(chars_by_square_i)
    }

    // BitBoard accessibility utilities.

    pub fn attackers(&self) -> BitBoard {
        self.bb_colors[self.color_to_move]
    }

    pub fn attackers_with_role(&self, role: Role) -> BitBoard {
        self.attackers() & self.bb_roles[role]
    }

    pub fn defenders(&self) -> BitBoard {
        self.bb_colors[!self.color_to_move]
    }

    pub fn bb_all(&self) -> BitBoard {
        self.bb_colors[Color::White] | self.bb_colors[Color::Black]
    }
}

impl Default for Board {
    fn default() -> Self {
        BOARD_DEFAULT.clone()
    }
}

impl fmt::Display for Board {
    fn fmt(&self, fmt: &mut fmt::Formatter) -> fmt::Result {
        self.square_centric_chars().fmt(fmt)
    }
}

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum CastlingSide {
    King,
    Queen,
}

#[derive(Clone, Debug, Hash, PartialEq, Eq)]
pub struct CastlingRights(EnumMap<Color, EnumMap<CastlingSide, bool>>);

impl CastlingRights {
    pub fn empty() -> Self {
        CastlingRights(EnumMap::default())
    }
}

impl Default for CastlingRights {
    fn default() -> Self {
        let both_sides = enum_map! {
            CastlingSide::King => true,
            CastlingSide::Queen => true,
        };
        CastlingRights(enum_map! {
            Color::White => both_sides,
            Color::Black => both_sides,
        })
    }
}

impl FromStr for CastlingRights {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut castling_rights = CastlingRights::empty();
        for c in s.chars() {
            let color = Color::from_char_case(c);
            let side = match c.to_ascii_uppercase() {
                'K' => CastlingSide::King,
                'Q' => CastlingSide::Queen,
                _ => break,
            };
            castling_rights.0[color][side] = true;
        }
        Ok(castling_rights)
    }
}

impl fmt::Display for CastlingRights {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        let mut any = false;
        if self.0[Color::White][CastlingSide::King] {
            write!(fmt, "K")?;
            any = true;
        }
        if self.0[Color::White][CastlingSide::Queen] {
            write!(fmt, "Q")?;
            any = true;
        }
        if self.0[Color::Black][CastlingSide::King] {
            write!(fmt, "k")?;
            any = true;
        }
        if self.0[Color::Black][CastlingSide::Queen] {
            write!(fmt, "q")?;
            any = true;
        }
        if !any {
            write!(fmt, "-")?;
        }
        Ok(())
    }
}

lazy_static! {
    // The poor man's const fn.
    pub static ref BOARD_DEFAULT: Board = {
        let mut bb_colors = EnumMap::default();
        bb_colors[Color::White] = Rank::FIRST.to_bb() | Rank::SECOND.to_bb();
        bb_colors[Color::Black] = Rank::SEVENTH.to_bb() | Rank::EIGHTH.to_bb();
        let mut bb_roles = EnumMap::default();
        bb_roles[Role::Pawn] = Rank::SECOND.to_bb() | Rank::SEVENTH.to_bb();
        bb_roles[Role::Knight] = Square::B1.to_bb()
            | Square::G1.to_bb()
            | Square::B8.to_bb()
            | Square::G8.to_bb();
        bb_roles[Role::Bishop] = Square::C1.to_bb()
            | Square::F1.to_bb()
            | Square::C8.to_bb()
            | Square::F8.to_bb();
        bb_roles[Role::Rook] = Square::A1.to_bb()
            | Square::H1.to_bb()
            | Square::A8.to_bb()
            | Square::H8.to_bb();
        bb_roles[Role::Queen] = Square::D1.to_bb() | Square::D8.to_bb();
        bb_roles[Role::King] = Square::E1.to_bb() | Square::E8.to_bb();
        Board {
            bb_colors,
            bb_roles,
            castling_rights: CastlingRights::default(),
            color_to_move: Color::White,
            reversible_moves_count: 0,
            en_passant_target_square: None,
            half_moves_counter: 0,
            full_moves_counter: 1,
        }
    };
}

pub struct SquareCentricBoard<T> {
    by_square_i: [T; Square::count()],
}

impl<T> From<[T; Square::count()]> for SquareCentricBoard<T> {
    fn from(by_square_i: [T; Square::count()]) -> Self {
        SquareCentricBoard { by_square_i }
    }
}

impl fmt::Display for SquareCentricBoard<char> {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(fmt, "     A B C D E F G H")?;
        writeln!(fmt, "   +-----------------+")?;
        for rank in Rank::iter().rev() {
            write!(fmt, " {} | ", char::from(rank))?;
            for file in File::iter() {
                let square = Square::at(file, rank);
                write!(fmt, "{} ", self.by_square_i[square.i()])?;
            }
            writeln!(fmt, "| {}", char::from(rank))?;
        }
        writeln!(fmt, "   +-----------------+")?;
        write!(fmt, "     A B C D E F G H")?;
        Ok(())
    }
}

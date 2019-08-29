use enum_map::{enum_map, EnumMap};
use enum_map_derive::Enum;
use std::fmt;
use std::hash::{Hash, Hasher};
use std::iter::{self, DoubleEndedIterator};
use std::str::FromStr;
use strum::IntoEnumIterator;
use strum_macros::EnumIter;

#[derive(Copy, Clone, Debug, Enum, EnumIter, PartialEq)]
pub enum Color {
    White,
    Black,
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Piece {
    role: Role,
    color: Color,
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
        let role = match c.to_ascii_uppercase() {
            'P' => Role::Pawn,
            'N' => Role::Knight,
            'B' => Role::Bishop,
            'R' => Role::Rook,
            'Q' => Role::Queen,
            'K' => Role::King,
            _ => panic!(),
        };
        let color = if c.is_ascii_uppercase() {
            Color::White
        } else {
            Color::Black
        };
        Piece::new(role, color)
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

// COORDINATES
// -----------

pub type Bitboard = u64;

impl<T: Into<Bitboard>> ToBb for T {}

pub trait Coordinate: Into<Bitboard> + From<char> {
    fn new(i: u8) -> Self;
}

pub trait ToBb: Into<Bitboard> {
    fn to_bb(self) -> Bitboard {
        self.into()
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct File(u8);

impl File {
    fn all() -> impl DoubleEndedIterator<Item = Self> {
        (0..8).map(File::new)
    }
    fn min() -> Self {
        File::from('a')
    }
}

impl Coordinate for File {
    fn new(i: u8) -> Self {
        assert!((0..8).contains(&i));
        File(i)
    }
}

impl Into<Bitboard> for File {
    fn into(self) -> Bitboard {
        0xff << (self.0 * 8)
    }
}

impl From<char> for File {
    fn from(c: char) -> Self {
        let i = (c as u8) - b'a';
        assert!((0..8).contains(&i));
        File(i)
    }
}

impl From<File> for char {
    fn from(file: File) -> Self {
        (file.0 + b'a') as Self
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Rank(u8);

impl Rank {
    fn all() -> impl DoubleEndedIterator<Item = Self> {
        (0..8).map(Rank::new)
    }
    fn max() -> Self {
        Rank::from('8')
    }
}

impl Coordinate for Rank {
    fn new(i: u8) -> Self {
        assert!((0..8).contains(&i));
        Rank(i)
    }
}

impl Into<Bitboard> for Rank {
    fn into(self) -> Bitboard {
        0x0101_0101_0101_0101 << self.0
    }
}

impl From<char> for Rank {
    fn from(c: char) -> Self {
        let i = c.to_digit(9).unwrap() - 1;
        assert!((0..8).contains(&i));
        Rank(i as u8)
    }
}

impl From<Rank> for char {
    fn from(rank: Rank) -> Self {
        (rank.0 + b'1') as Self
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Square(File, Rank);

impl Square {
    pub fn new(file: File, rank: Rank) -> Self {
        Square(file, rank)
    }
}

impl Into<Bitboard> for Square {
    fn into(self) -> Bitboard {
        self.0.to_bb() & self.1.to_bb()
    }
}

impl<S: AsRef<str>> From<S> for Square {
    fn from(s: S) -> Self {
        Square::from_str(s.as_ref()).unwrap()
    }
}

impl FromStr for Square {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        assert!(s.len() == 2);
        let mut chars = s.chars();
        let file = chars.next().unwrap();
        let rank = chars.next().unwrap();
        Ok(Square::new(file.into(), rank.into()))
    }
}

// BOARD LOGIC
// -----------

pub struct Board {
    bb_colors: EnumMap<Color, Bitboard>,
    bb_roles: EnumMap<Role, Bitboard>,
    castling_rights: EnumMap<Color, EnumMap<CastlingSide, bool>>,
    color_to_move: Color,
    reversible_moves_count: usize,
}

impl Board {
    pub fn empty() -> Self {
        Board {
            bb_colors: EnumMap::default(),
            bb_roles: EnumMap::default(),
            castling_rights: EnumMap::default(),
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
            rank.0 -= 1;
            let mut file = File::min();
            for c in rank_piece_map.chars() {
                if let Some(digit) = c.to_digit(9) {
                    for _ in 0..digit {
                        file.0 += 1;
                    }
                } else {
                    file.0 += 1;
                }
            }
        }
        unimplemented!()
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

    pub fn do_move(&mut self, mv: &Move) {
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
        let castling_rights = enum_map! {
            Color::White => enum_map! {
                CastlingSide::King => true,
                CastlingSide::Queen => true,
            },
            Color::Black => enum_map! {
                CastlingSide::King => true,
                CastlingSide::Queen => true,
            },
        };
        Board {
            bb_colors,
            bb_roles,
            castling_rights,
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
                let square = Square::new(file, rank);
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

pub struct Move {
    from: Square,
    to: Square,
    promotion: Option<Role>,
}

impl FromStr for Move {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        assert!(s.len() >= 4);
        let from = Square::from_str(&s[0..2])?;
        let to = Square::from_str(&s[2..4])?;
        Ok(Move {
            from,
            to,
            promotion: None,
        })
    }
}

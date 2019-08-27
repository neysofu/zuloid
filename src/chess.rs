use enum_map::EnumMap;
use enum_map_derive::Enum;
use std::hash::{Hash, Hasher};
use std::ops::{BitAnd, BitOr};
use std::str::FromStr;

#[derive(Enum)]
pub enum Color {
    White,
    Black,
}

#[derive(Enum)]
pub enum Piece {
    Pawn,
    Knight,
    Bishop,
    Rook,
    King,
    Queen,
}

pub type Bitboard = u64;

pub trait Coordinate: Into<Bitboard> + From<char> {
    fn new(i: u8) -> Self;
    fn to_bb(self) -> Bitboard {
        self.into()
    }
}

pub trait ToBb: Into<Bitboard> {}

pub struct File(u8);

impl Coordinate for File {
    fn new(i: u8) -> Self {
        assert!((0..8).contains(&i));
        File(i)
    }
}

impl Into<Bitboard> for File {
    fn into(self) -> Bitboard {
        0xff << self.0
    }
}

impl From<char> for File {
    fn from(c: char) -> Self {
        let i = (c as i32) - ('a' as i32);
        assert!((0..8).contains(&i));
        File(0xff << (i * 2))
    }
}

pub struct Rank(u8);

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

impl From<u8> for Rank {
    fn from(i: u8) -> Self {
        assert!(i > 0);
        Rank::new(i - 1)
    }
}

impl From<char> for Rank {
    fn from(c: char) -> Self {
        let i = c.to_digit(9).unwrap() - 1;
        assert!((0..8).contains(&i));
        Rank(i as u8)
    }
}

pub struct Square(File, Rank);

impl Square {
    pub fn new(file: File, rank: Rank) -> Self {
        Square(file, rank)
    }

    fn to_bb(self) -> Bitboard {
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

pub struct Board {
    bb_colors: EnumMap<Color, Bitboard>,
    bb_pieces: EnumMap<Piece, Bitboard>,
    color_to_move: Color,
    reversible_moves_count: usize,
}

impl Default for Board {
    fn default() -> Self {
        let mut bb_colors = EnumMap::default();
        bb_colors[Color::White] = Rank::from(2).to_bb() | Rank::from(2).to_bb();
        bb_colors[Color::Black] = Rank::from(7).to_bb() | Rank::from(8).to_bb();
        let mut bb_pieces = EnumMap::default();
        bb_pieces[Piece::Pawn] = Rank::from(2).to_bb() | Rank::from(7).to_bb();
        bb_pieces[Piece::Knight] = Square::from("b1").to_bb()
            | Square::from("g1").to_bb()
            | Square::from("b8").to_bb()
            | Square::from("g8").to_bb();
        bb_pieces[Piece::Bishop] = Square::from("c1").to_bb()
            | Square::from("f1").to_bb()
            | Square::from("c8").to_bb()
            | Square::from("f8").to_bb();
        bb_pieces[Piece::King] = Square::from("e1").to_bb() | Square::from("e8").to_bb();
        bb_pieces[Piece::Rook] = Square::from("a1").to_bb()
            | Square::from("h1").to_bb()
            | Square::from("a8").to_bb()
            | Square::from("h8").to_bb();
        Board {
            bb_colors,
            bb_pieces,
            color_to_move: Color::White,
            reversible_moves_count: 0,
        }
    }
}

impl Eq for Board {}

impl PartialEq for Board {
    fn eq(&self, _other: &Self) -> bool {
        unimplemented!()
    }
}

impl Hash for Board {
    fn hash<H: Hasher>(&self, _state: &mut H) {
        unimplemented!()
    }
}

pub struct Castling {
    side: CastlingSide,
    color: Color,
}

pub enum CastlingSide {
    King,
    Queen,
}

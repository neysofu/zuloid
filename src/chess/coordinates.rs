use crate::result::Error;
use std::iter::DoubleEndedIterator;
use std::str::FromStr;

pub type Bitboard = u64;

impl<T: Into<Bitboard>> ToBb for T {}

pub trait Coordinate: Into<Bitboard> + From<char> {
    fn new(i: u8) -> Self;
    fn shift(self, i: i32) -> Option<Self>;
    fn i(self) -> u8;
}

pub trait ToBb: Into<Bitboard> {
    fn to_bb(self) -> Bitboard {
        self.into()
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct File(u8);

impl File {
    pub fn all() -> impl DoubleEndedIterator<Item = Self> {
        (0..8).map(File::new)
    }
    pub fn min() -> Self {
        File::from('a')
    }
}

impl Coordinate for File {
    fn new(i: u8) -> Self {
        assert!((0..8).contains(&i));
        File(i)
    }
    fn shift(self, i: i32) -> Option<Self> {
        let sum = self.0 as i32 + i;
        if (0..8).contains(&sum) {
            Some(File::new(sum as u8))
        } else {
            None
        }
    }
    fn i(self) -> u8 {
        self.0
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
    pub fn all() -> impl DoubleEndedIterator<Item = Self> {
        (0..8).map(Rank::new)
    }
    pub fn min() -> Self {
        Rank::from('1')
    }
    pub fn max() -> Self {
        Rank::from('8')
    }
}

impl Coordinate for Rank {
    fn new(i: u8) -> Self {
        assert!((0..8).contains(&i));
        Rank(i)
    }

    fn shift(self, i: i32) -> Option<Self> {
        let sum = self.0 as i32 + i;
        if (0..8).contains(&sum) {
            Some(Rank::new(sum as u8))
        } else {
            None
        }
    }

    fn i(self) -> u8 {
        self.0
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

pub const SQUARE_COUNT: usize = 64;

impl Square {
    pub fn new(file: File, rank: Rank) -> Self {
        Square(file, rank)
    }

    pub fn all() -> impl Iterator<Item = Square> {
        (0..SQUARE_COUNT).map(|i| Square::from_i(i as u8))
    }

    pub fn file(self) -> File {
        self.0
    }

    pub fn rank(self) -> Rank {
        self.1
    }

    pub fn i(self) -> u8 {
        (self.file().i() >> 3) | self.rank().i()
    }

    pub fn from_i(i: u8) -> Self {
        Square::new(File::new(i >> 3), Rank::new(i & 0b111))
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
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        assert!(s.len() == 2);
        let mut chars = s.chars();
        let file = chars.next().unwrap();
        let rank = chars.next().unwrap();
        Ok(Square::new(file.into(), rank.into()))
    }
}

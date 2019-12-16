use crate::err::Error;
use super::Color;
use bitintr::Blsi;
use bitintr::Tzcnt;
use rand::Rng;
use std::convert::{TryFrom, TryInto};
use std::fmt;
use std::iter::DoubleEndedIterator;
use std::marker::PhantomData;
use std::ops;
use std::str::FromStr;

pub type BitBoard = u64;

pub trait BitBoardOps: ops::BitAnd<Output = Self> + Sized {
    fn squares(self) -> BitsCounter;
    fn north(self, n: usize) -> Self;
    fn south(self, n: usize) -> Self;
    fn east(self, n: usize) -> Self;
    fn west(self, n: usize) -> Self;
    fn rnd<R: Rng>(r: &mut R) -> Self;
    fn rnd_sparsely_populated<R: Rng>(r: &mut R) -> Self {
        Self::rnd(r) & Self::rnd(r) & Self::rnd(r)
    }
}

impl BitBoardOps for BitBoard {
    fn squares(self) -> BitsCounter {
        BitsCounter { bb: self }
    }

    fn north(self, n: usize) -> Self {
        self << n
    }

    fn south(self, n: usize) -> Self {
        self >> n
    }

    fn east(self, n: usize) -> Self {
        self << (8 * n)
    }

    fn west(self, n: usize) -> Self {
        self >> (8 * n)
    }

    fn rnd<R: Rng>(r: &mut R) -> Self {
        r.gen()
    }
}

pub struct BitsCounter {
    bb: BitBoard,
}

impl Iterator for BitsCounter {
    type Item = Square;

    fn next(&mut self) -> Option<Self::Item> {
        if self.bb == 0 {
            None
        } else {
            let i = self.bb.tzcnt() as u8;
            self.bb ^= self.bb.blsi();
            Some(Square::new(i))
        }
    }
}

pub trait Coordinate: Into<BitBoard> + Sized {
    const RANGE: ops::Range<u8>;

    fn new_unchecked(i: u8) -> Self;
    fn i(self) -> usize;

    fn new(i: u8) -> Self {
        assert!(Self::RANGE.contains(&i));
        Self::new_unchecked(i)
    }

    fn new_opt(i: i32) -> Option<Self> {
        let start = i32::from(Self::RANGE.start);
        let end = i32::from(Self::RANGE.end);
        let range = ops::Range { start, end };
        if range.contains(&i) {
            Some(Self::new_unchecked(i as u8))
        } else {
            None
        }
    }

    fn new_rnd<R: Rng>(r: &mut R) -> Self {
        let range = Self::RANGE;
        Self::new_unchecked(r.gen_range(range.start, range.end))
    }

    fn shift(self, rhs: i32) -> Option<Self> {
        Self::new_opt(self.i() as i32 + rhs)
    }

    fn min() -> Self {
        Self::new_unchecked(Self::RANGE.start)
    }

    fn max() -> Self {
        Self::new_unchecked(Self::RANGE.end - 1)
    }

    fn count() -> usize {
        (Self::RANGE.end - Self::RANGE.start) as usize
    }

    fn iter() -> CoordinateWalker<Self> {
        CoordinateWalker {
            phantom: PhantomData,
            range: Self::RANGE,
        }
    }
}

pub struct CoordinateWalker<T> {
    phantom: PhantomData<T>,
    range: ops::Range<u8>,
}

impl<T> CoordinateWalker<T> {
    fn is_over(&self) -> bool {
        self.range.start == self.range.end
    }
}

impl<S: Coordinate> Iterator for CoordinateWalker<S> {
    type Item = S;

    fn next(&mut self) -> Option<Self::Item> {
        if self.is_over() {
            None
        } else {
            let i = self.range.start;
            self.range.start += 1;
            Some(S::new_unchecked(i))
        }
    }
}

impl<S: Coordinate> DoubleEndedIterator for CoordinateWalker<S> {
    fn next_back(&mut self) -> Option<Self::Item> {
        if self.is_over() {
            None
        } else {
            self.range.end -= 1;
            Some(S::new_unchecked(self.range.end))
        }
    }
}

pub trait ToBb: Into<BitBoard> {
    fn to_bb(self) -> BitBoard {
        self.into()
    }
}

impl<T: Into<BitBoard>> ToBb for T {}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct File(u8);

impl File {
    pub const A: Self = File(0);
    pub const B: Self = File(1);
    pub const C: Self = File(2);
    pub const D: Self = File(3);
    pub const E: Self = File(4);
    pub const F: Self = File(5);
    pub const G: Self = File(6);
    pub const H: Self = File(7);
}

impl Coordinate for File {
    const RANGE: ops::Range<u8> = 0..8;

    fn new_unchecked(i: u8) -> Self {
        File(i)
    }

    fn i(self) -> usize {
        self.0 as usize
    }
}

impl Into<BitBoard> for File {
    fn into(self) -> BitBoard {
        0xff << (self.0 * 8)
    }
}

impl TryFrom<char> for File {
    type Error = Error;

    fn try_from(c: char) -> Result<Self, Self::Error> {
        let i = (c as u8) - b'a';
        Ok(File::new(i))
    }
}

impl From<File> for char {
    fn from(file: File) -> Self {
        (file.0 + b'a') as Self
    }
}

impl fmt::Display for File {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", char::from(*self))
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Rank(u8);

impl Rank {
    pub const FIRST: Rank = Rank(0);
    pub const SECOND: Rank = Rank(1);
    pub const THIRD: Rank = Rank(2);
    pub const FOURTH: Rank = Rank(3);
    pub const FIFTH: Rank = Rank(4);
    pub const SIXTH: Rank = Rank(5);
    pub const SEVENTH: Rank = Rank(6);
    pub const EIGHTH: Rank = Rank(7);

    pub fn symmetric(self) -> Self {
        Rank::new_unchecked(7 - self.i() as u8)
    }

    pub fn new_with_side(i: u8, side: Color) -> Self {
        let rank = Rank::new(i);
        match side {
            Color::White => rank,
            Color::Black => rank.symmetric(),
        }
    }
}

impl Coordinate for Rank {
    const RANGE: ops::Range<u8> = 0..8;

    fn new_unchecked(i: u8) -> Self {
        Rank(i)
    }

    fn i(self) -> usize {
        self.0 as usize
    }
}

impl Into<BitBoard> for Rank {
    fn into(self) -> BitBoard {
        0x0101_0101_0101_0101 << self.0
    }
}

impl TryFrom<char> for Rank {
    type Error = Error;

    fn try_from(c: char) -> Result<Self, Self::Error> {
        let i = c.to_digit(9).unwrap() - 1;
        Ok(Rank::new(i as u8))
    }
}

impl From<Rank> for char {
    fn from(rank: Rank) -> Self {
        (rank.0 + b'1') as Self
    }
}

impl fmt::Display for Rank {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", char::from(*self))
    }
}

#[derive(Copy, Clone, Debug, Hash, PartialEq, Eq)]
pub struct Square(u8);

impl Square {
    #![allow(dead_code)]

    pub const A1: Square = Square(0o0);
    pub const A2: Square = Square(0o1);
    pub const A3: Square = Square(0o2);
    pub const A4: Square = Square(0o3);
    pub const A5: Square = Square(0o4);
    pub const A6: Square = Square(0o5);
    pub const A7: Square = Square(0o6);
    pub const A8: Square = Square(0o7);

    pub const B1: Square = Square(0o10);
    pub const B2: Square = Square(0o11);
    pub const B3: Square = Square(0o12);
    pub const B4: Square = Square(0o13);
    pub const B5: Square = Square(0o14);
    pub const B6: Square = Square(0o15);
    pub const B7: Square = Square(0o16);
    pub const B8: Square = Square(0o17);

    pub const C1: Square = Square(0o20);
    pub const C2: Square = Square(0o21);
    pub const C3: Square = Square(0o22);
    pub const C4: Square = Square(0o23);
    pub const C5: Square = Square(0o24);
    pub const C6: Square = Square(0o25);
    pub const C7: Square = Square(0o26);
    pub const C8: Square = Square(0o27);

    pub const D1: Square = Square(0o30);
    pub const D2: Square = Square(0o31);
    pub const D3: Square = Square(0o32);
    pub const D4: Square = Square(0o33);
    pub const D5: Square = Square(0o34);
    pub const D6: Square = Square(0o35);
    pub const D7: Square = Square(0o36);
    pub const D8: Square = Square(0o37);

    pub const E1: Square = Square(0o40);
    pub const E2: Square = Square(0o41);
    pub const E3: Square = Square(0o42);
    pub const E4: Square = Square(0o43);
    pub const E5: Square = Square(0o44);
    pub const E6: Square = Square(0o45);
    pub const E7: Square = Square(0o46);
    pub const E8: Square = Square(0o47);

    pub const F1: Square = Square(0o50);
    pub const F2: Square = Square(0o51);
    pub const F3: Square = Square(0o52);
    pub const F4: Square = Square(0o53);
    pub const F5: Square = Square(0o54);
    pub const F6: Square = Square(0o55);
    pub const F7: Square = Square(0o56);
    pub const F8: Square = Square(0o57);

    pub const G1: Square = Square(0o60);
    pub const G2: Square = Square(0o61);
    pub const G3: Square = Square(0o62);
    pub const G4: Square = Square(0o63);
    pub const G5: Square = Square(0o64);
    pub const G6: Square = Square(0o65);
    pub const G7: Square = Square(0o66);
    pub const G8: Square = Square(0o67);

    pub const H1: Square = Square(0o70);
    pub const H2: Square = Square(0o71);
    pub const H3: Square = Square(0o72);
    pub const H4: Square = Square(0o73);
    pub const H5: Square = Square(0o74);
    pub const H6: Square = Square(0o75);
    pub const H7: Square = Square(0o76);
    pub const H8: Square = Square(0o77);

    pub const fn count() -> usize {
        64
    }

    pub const fn at(file: File, rank: Rank) -> Self {
        Square(((file.0 << 3) | rank.0) as u8)
    }

    pub fn file(self) -> File {
        File::new_unchecked(self.0 >> 3)
    }

    pub fn rank(self) -> Rank {
        Rank::new_unchecked(self.0 & 0b111)
    }

    pub fn diagonal_a1h8(self) -> BitBoard {
        let mut diagonal = 0x8040_2010_0804_0201;
        let delta = self.rank().i() as i32 - self.file().i() as i32;
        if delta >= 0 {
            diagonal <<= delta;
        } else {
            diagonal >>= delta;
        }
        diagonal
    }

    pub fn diagonal_h1a8(self) -> BitBoard {
        let mut diagonal = 0x102_0408_1020_4080;
        let delta = self.rank().i() as i32 + self.file().i() as i32 - 7;
        if delta >= 0 {
            diagonal <<= delta;
        } else {
            diagonal >>= delta;
        }
        diagonal
    }
}

impl Into<BitBoard> for Square {
    fn into(self) -> BitBoard {
        1u64 << self.0
    }
}

impl Coordinate for Square {
    const RANGE: ops::Range<u8> = 0..64;

    fn new_unchecked(i: u8) -> Self {
        Square(i)
    }

    fn i(self) -> usize {
        self.0 as usize
    }
}

impl FromStr for Square {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut chars = s.chars();
        Ok(Square::at(
            chars.next().ok_or(Error::InvalidSquare)?.try_into()?,
            chars.next().ok_or(Error::InvalidSquare)?.try_into()?,
        ))
    }
}

impl fmt::Display for Square {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", char::from(self.file()))?;
        write!(fmt, "{}", char::from(self.rank()))
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn file_boundaries() {
        assert_eq!(File::new_opt(8), None);
        assert_eq!(File::new_opt(-1), None);
        assert!(File::new_opt(0).is_some());
    }

    #[test]
    fn rank_boundaries() {
        assert_eq!(Rank::new_opt(8), None);
        assert_eq!(Rank::new_opt(-1), None);
        assert!(Rank::new_opt(0).is_some());
    }

    #[test]
    fn file_h_is_max() {
        assert_eq!(File::max(), File::H);
    }

    #[test]
    fn rank_8_is_max() {
        assert_eq!(Rank::max(), Rank::EIGHTH);
    }

    #[test]
    fn rank_max_i_is_7() {
        assert_eq!(Rank::EIGHTH.i(), 7);
    }

    #[test]
    fn square_file_and_rank() {
        let square = Square::new(14);
        assert_eq!(square.file(), File::new(1));
        assert_eq!(square.rank(), Rank::new(6));
    }

    #[test]
    fn square_at() {
        let file = File::new(3);
        let rank = Rank::max();
        let square = Square::at(file, rank);
        assert_eq!(square.file(), file);
        assert_eq!(square.rank(), rank);
    }

    #[test]
    fn square_i() {
        let file = File::new(3);
        let rank = Rank::max();
        let square = Square::at(file, rank);
        assert_eq!(square.i(), 31);
    }

    #[test]
    fn rank_new_rnd_i_is_in_range() {
        let rank = Rank::new_rnd(&mut rand::thread_rng());
        assert!(Rank::RANGE.contains(&(rank.i() as u8)));
    }
}

use crate::result::Error;
use std::iter::DoubleEndedIterator;
use std::marker::PhantomData;
use std::ops;
use std::str::FromStr;

pub type Bitboard = u64;

pub trait Coordinate
where
    Self: Into<Bitboard> + Sized,
{
    fn range() -> ops::Range<u8>;
    fn new_unchecked(i: u8) -> Self;
    fn i(self) -> u8;

    fn new(i: u8) -> Self {
        assert!(Self::range().contains(&i));
        Self::new_unchecked(i)
    }

    fn new_opt(i: i32) -> Option<Self> {
        let start = i32::from(Self::range().start);
        let end = i32::from(Self::range().end);
        let range = ops::Range { start, end };
        if range.contains(&i) {
            Some(Self::new_unchecked(i as u8))
        } else {
            None
        }
    }

    fn shift(self, rhs: i32) -> Option<Self> {
        Self::new_opt(rhs + i32::from(self.i()))
    }

    fn min() -> Self {
        Self::new_unchecked(Self::range().start)
    }

    fn max() -> Self {
        Self::new_unchecked(Self::range().end - 1)
    }

    fn all() -> CoordinateWalker<Self> {
        CoordinateWalker {
            phantom: PhantomData,
            range: Self::range(),
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

pub trait ToBb: Into<Bitboard> {
    fn to_bb(self) -> Bitboard {
        self.into()
    }
}

impl<T: Into<Bitboard>> ToBb for T {}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct File(u8);

impl Coordinate for File {
    fn range() -> ops::Range<u8> {
        (0..8)
    }

    fn new_unchecked(i: u8) -> Self {
        File(i)
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
        File::new(i)
    }
}

impl From<File> for char {
    fn from(file: File) -> Self {
        (file.0 + b'a') as Self
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Rank(u8);

impl Coordinate for Rank {
    fn range() -> ops::Range<u8> {
        (0..8)
    }

    fn new_unchecked(i: u8) -> Self {
        Rank(i)
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
        Rank::new(i as u8)
    }
}

impl From<Rank> for char {
    fn from(rank: Rank) -> Self {
        (rank.0 + b'1') as Self
    }
}

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Square(u8);

pub const SQUARE_COUNT: usize = 64;

impl Coordinate for Square {
    fn range() -> ops::Range<u8> {
        (0..(SQUARE_COUNT as u8))
    }

    fn new_unchecked(i: u8) -> Self {
        Square(i)
    }

    fn i(self) -> u8 {
        self.0
    }
}

impl Square {
    pub fn at(file: File, rank: Rank) -> Self {
        Square::new_unchecked((file.i() << 3) | rank.i())
    }

    pub fn file(self) -> File {
        File::new_unchecked(self.0 >> 3)
    }

    pub fn rank(self) -> Rank {
        Rank::new_unchecked(self.0 & 0b111)
    }
}

impl Into<Bitboard> for Square {
    fn into(self) -> Bitboard {
        1 << self.0
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
        Ok(Square::at(file.into(), rank.into()))
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
        assert_eq!(File::max(), File::from('h'));
    }

    #[test]
    fn rank_8_is_max() {
        assert_eq!(Rank::max(), Rank::from('8'));
    }

    #[test]
    fn rank_max_i_is_7() {
        assert_eq!(Rank::from('8').i(), 7);
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
}

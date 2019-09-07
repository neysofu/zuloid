use bitintr::Blsi;
use bitintr::Tzcnt;
use rand::Rng;
use std::fmt;
use std::iter::DoubleEndedIterator;
use std::marker::PhantomData;
use std::ops;
use std::str::FromStr;
use zorro_common::Error;

pub type Bitboard = u64;

pub struct BitboardFormatter(Bitboard);

impl fmt::Display for BitboardFormatter {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        // TODO: nice output with ranks and files.
        write!(fmt, "{:x}", self.0)
    }
}

pub trait BitboardOps: ops::BitAnd<Output = Self> + Sized {
    fn squares(self) -> BitsCounter;
    fn north(self, n: usize) -> Self;
    fn south(self, n: usize) -> Self;
    fn fmt_as_board(self) -> BitboardFormatter;
    fn rnd<R: Rng>(r: &mut R) -> Self;
    fn rnd_sparsely_populated<R: Rng>(r: &mut R) -> Self {
        Self::rnd(r) & Self::rnd(r) & Self::rnd(r)
    }
}

impl BitboardOps for Bitboard {
    fn squares(self) -> BitsCounter {
        BitsCounter { bb: self }
    }

    fn north(self, n: usize) -> Self {
        self << n
    }

    fn south(self, n: usize) -> Self {
        self >> n
    }

    fn fmt_as_board(self) -> BitboardFormatter {
        BitboardFormatter(self)
    }

    fn rnd<R: Rng>(r: &mut R) -> Self {
        r.gen()
    }
}

pub struct BitsCounter {
    bb: Bitboard,
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

pub trait Coordinate
where
    Self: Into<Bitboard> + Sized,
{
    fn range() -> ops::Range<u8>;
    fn new_unchecked(i: u8) -> Self;
    fn i(self) -> usize;

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

    fn new_rnd<R: Rng>(r: &mut R) -> Self {
        let range = Self::range();
        Self::new_unchecked(r.gen_range(range.start, range.end))
    }

    fn shift(self, rhs: i32) -> Option<Self> {
        Self::new_opt(self.i() as i32 + rhs)
    }

    fn min() -> Self {
        Self::new_unchecked(Self::range().start)
    }

    fn max() -> Self {
        Self::new_unchecked(Self::range().end - 1)
    }

    fn count() -> usize {
        (Self::range().end - Self::range().start) as usize
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

    fn i(self) -> usize {
        self.0 as usize
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

impl fmt::Display for File {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", char::from(*self))
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

    fn i(self) -> usize {
        self.0 as usize
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

impl fmt::Display for Rank {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", char::from(*self))
    }
}

#[derive(Copy, Clone, Debug, Hash, PartialEq, Eq)]
pub struct Square(u8);

pub const SQUARE_COUNT: usize = 64;

impl Coordinate for Square {
    fn range() -> ops::Range<u8> {
        (0..(SQUARE_COUNT as u8))
    }

    fn new_unchecked(i: u8) -> Self {
        Square(i)
    }

    fn i(self) -> usize {
        self.0 as usize
    }
}

impl Square {
    pub fn at(file: File, rank: Rank) -> Self {
        Square::new_unchecked(((file.i() << 3) | rank.i()) as u8)
    }

    pub fn file(self) -> File {
        File::new_unchecked(self.0 >> 3)
    }

    pub fn rank(self) -> Rank {
        Rank::new_unchecked(self.0 & 0b111)
    }

    pub fn diagonal_a1h8(self) -> Bitboard {
        let mut main_diagonal = 0x8040_2010_0804_0201;
        let delta = self.rank().i() as i32 - self.file().i() as i32;
        if delta >= 0 {
            main_diagonal <<= delta;
        } else {
            main_diagonal >>= delta;
        }
        main_diagonal
    }
    pub fn diagonal_h1a8(self) -> Bitboard {
        let mut main_diagonal = 0x102_0408_1020_4080;
        let delta = self.rank().i() as i32 + self.file().i() as i32 - 7;
        if delta >= 0 {
            main_diagonal <<= delta;
        } else {
            main_diagonal >>= delta;
        }
        main_diagonal
    }
}

impl Into<Bitboard> for Square {
    fn into(self) -> Bitboard {
        1u64 << self.0
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

    #[test]
    fn rank_new_rnd_i_is_in_range() {
        let rank = Rank::new_rnd(&mut rand::thread_rng());
        assert!(Rank::range().contains(&(rank.i() as u8)));
    }
}

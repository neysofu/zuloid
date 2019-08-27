use enum_map::EnumMap;
use enum_map_derive::Enum;
use std::hash::{Hash, Hasher};
use std::ops::{BitAnd, BitOr};

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

pub struct Bitboard {
    bb: u64,
}

impl Bitboard {
    pub const fn file(i: usize) -> Self {
        Bitboard { bb: 0xff << i }
    }

    pub const fn rank(i: usize) -> Self {
        Bitboard {
            bb: 0x0101_0101_0101_0101 << i,
        }
    }
}

impl Default for Bitboard {
    fn default() -> Self {
        Bitboard { bb: 0 }
    }
}

impl BitAnd for Bitboard {
    type Output = Self;

    fn bitand(self, other: Self) -> Self::Output {
        Bitboard {
            bb: self.bb & other.bb,
        }
    }
}

impl BitOr for Bitboard {
    type Output = Self;

    fn bitor(self, other: Self) -> Self::Output {
        Bitboard {
            bb: self.bb | other.bb,
        }
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
        bb_colors[Color::White] = RANK_1 | RANK_2;
        bb_colors[Color::Black] = RANK_7 | RANK_8;
        let mut bb_pieces = EnumMap::default();
        bb_pieces[Piece::Pawn] = RANK_2 | RANK_7;
        bb_pieces[Piece::Knight] = SQ_B1 | SQ_G1 | SQ_B8 | SQ_G8;
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
    fn eq(&self, other: &Self) -> bool {
        unimplemented!()
    }
}

impl Hash for Board {
    fn hash<H: Hasher>(&self, state: &mut H) {
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

pub const FILE_A: Bitboard = Bitboard::file(0);
pub const FILE_B: Bitboard = Bitboard::file(1);
pub const FILE_C: Bitboard = Bitboard::file(2);
pub const FILE_D: Bitboard = Bitboard::file(3);
pub const FILE_E: Bitboard = Bitboard::file(4);
pub const FILE_F: Bitboard = Bitboard::file(5);
pub const FILE_G: Bitboard = Bitboard::file(6);
pub const FILE_H: Bitboard = Bitboard::file(7);

pub const RANK_1: Bitboard = Bitboard::rank(0);
pub const RANK_2: Bitboard = Bitboard::rank(1);
pub const RANK_3: Bitboard = Bitboard::rank(2);
pub const RANK_4: Bitboard = Bitboard::rank(3);
pub const RANK_5: Bitboard = Bitboard::rank(4);
pub const RANK_6: Bitboard = Bitboard::rank(5);
pub const RANK_7: Bitboard = Bitboard::rank(6);
pub const RANK_8: Bitboard = Bitboard::rank(7);

pub const SQ_B1: Bitboard = Bitboard { bb: 1 << 8 };
pub const SQ_B8: Bitboard = Bitboard { bb: 1 << 15 };
pub const SQ_G1: Bitboard = Bitboard { bb: 1 << 47 };
pub const SQ_G8: Bitboard = Bitboard { bb: 1 << 55 };

//pub struct File(i32);
//
//impl File {
//    pub const fn a() -> Self {
//        File(0)
//    }
//
//    pub const fn b() -> Self {
//        File(1)
//    }
//
//    pub const fn c() -> Self {
//        File(2)
//    }
//
//    pub const fn d() -> Self {
//        File(3)
//    }
//
//    pub const fn e() -> Self {
//        File(4)
//    }
//
//    pub const fn f() -> Self {
//        File(5)
//    }
//
//    pub const fn g() -> Self {
//        File(6)
//    }
//
//    pub const fn h() -> Self {
//        File(7)
//    }
//
//    pub const fn left(&self) -> Self {
//        File::normalize(self.0 - 1)
//    }
//
//    pub const fn right(&self) -> Self {
//        File::normalize(self.0 + 1)
//    }
//
//    const fn normalize(i: i32) -> Self {
//        if i < 0 {
//            File(0)
//        } else if i > 7 {
//            File(7)
//        } else {
//            File(i)
//        }
//    }
//}
//
//impl BitOr
//
//pub struct Rank(i32);
//
//impl Rank {
//    pub fn new(i: u8) -> Self {
//        Rank::normalize(i as i32)
//    }
//
//    pub fn down(&self) -> Self {
//        Rank::normalize(self.0 - 1)
//    }
//
//    pub fn up(&self) -> Self {
//        Rank::normalize(self.0 + 1)
//    }
//
//    fn normalize(i: i32) -> Self {
//        if i < 0 {
//            Rank(0)
//        } else if i > 7 {
//            Rank(7)
//        } else {
//            Rank(i)
//        }
//    }
//}
//
//pub trait ToBitboard {
//    fn to_bb(&self) -> Bitboard;
//}
//
//impl ToBitboard for File {
//    fn to_bb(&self) -> Bitboard {
//        unimplemented!()
//    }
//}
//
//impl ToBitboard for Rank {
//    fn to_bb(&self) -> Bitboard {
//        unimplemented!()
//    }
//}
//

use super::*;
use lazy_static::lazy_static;
use std::iter::FromIterator;

#[derive(Debug, Copy, Clone)]
pub struct Magic {
    mask: BitBoard,
    multiplier: u64,
    right_shift: usize,
}

impl Magic {
    /// See [https://www.chessprogramming.org/Magic_BitBoards] for more information.
    pub fn magify(&self, bb: BitBoard) -> u64 {
        ((bb & self.mask) * self.multiplier) >> self.right_shift
    }

    fn by_file() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }

    fn by_rank() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }

    fn by_main_diagonal() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }

    fn by_anti_diagonal() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }
}

pub struct MagicsDb {
    by_file: [BitBoard; Square::count()],
    by_rank: [BitBoard; Square::count()],
    by_main_diagonal: [BitBoard; Square::count()],
    bb_anti_diagonal: [BitBoard; Square::count()],
}

impl SlidingPiecesMoveGen for MagicsDb {
    fn gen_rooks(&self, _buf: &mut AvailableMoves, _attackers: BitBoard, _all: BitBoard) -> usize {
        0
    }

    fn gen_bishops(
        &self,
        _buf: &mut AvailableMoves,
        _attackers: BitBoard,
        _all: BitBoard,
    ) -> usize {
        0
    }
}

impl Default for MagicsDb {
    fn default() -> Self {
        unimplemented!()
    }
}

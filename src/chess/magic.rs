use super::*;
use array_init::array_init;
use std::fmt;
use std::iter::FromIterator;

const ROOK_MAGICS: [Bitboard; Square::count()] = [
    0x0080004000608010,
    0x2240100040012002,
    0x008008a000841000,
    0x0100204900500004,
    0x020008200200100c,
    0x40800c0080020003,
    0x0080018002000100,
    0x4200042040820d04,
    0x10208008a8400480,
    0x4064402010024000,
    0x2181002000c10212,
    0x5101000850002100,
    0x0010800400080081,
    0x0012000200300815,
    0x060200080e002401,
    0x4282000420944201,
    0x1040208000400091,
    0x0010004040002008,
    0x0082020020804011,
    0x0005420010220208,
    0x8010510018010004,
    0x05050100088a1400,
    0x0009008080020001,
    0x2001060000408c01,
    0x0060400280008024,
    0x9810401180200382,
    0x0200201200420080,
    0x0280300100210048,
    0x0000080080800400,
    0x0002010200081004,
    0x8089000900040200,
    0x0040008200340047,
    0x0400884010800061,
    0xc202401000402000,
    0x0800401301002004,
    0x4c43502042000a00,
    0x0004a80082800400,
    0xd804040080800200,
    0x060200080e002401,
    0x0203216082000104,
    0x0000804000308000,
    0x004008100020a000,
    0x1001208042020012,
    0x0400220088420010,
    0x8010510018010004,
    0x8009000214010048,
    0x6445006200130004,
    0x000a008402460003,
    0x0080044014200240,
    0x0040012182411500,
    0x0003102001430100,
    0x4c43502042000a00,
    0x1008000400288080,
    0x0806003008040200,
    0x4200020801304400,
    0x8100640912804a00,
    0x300300a043168001,
    0x0106610218400081,
    0x008200c008108022,
    0x0201041861017001,
    0x00020010200884e2,
    0x0205000e18440001,
    0x202008104a08810c,
    0x800a208440230402,
];

const SHIFT_ROOK: [u8; Square::count()] = [
    52, 53, 53, 53, 53, 53, 53, 52,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    53, 54, 54, 54, 54, 54, 54, 53,
    52, 53, 53, 53, 53, 53, 53, 52,
];

static mut ROOK_MASK: [Bitboard; Square::count()] = [0; Square::count()];


pub static mut ATTACK_ROOK: [Bitboard; 0] = {
    []
};

unsafe fn bb_init() {
    use crate::chess::tables::rook_attacks;
    for square in Square::iter() {
        let borders = File::A.to_bb() | File::H.to_bb() | Rank::_1.to_bb() | Rank::_8.to_bb();
        ROOK_MASK[square.i()] = (square.file().to_bb() | square.rank().to_bb()) & !borders;
        let n = 1 << bitintr::Popcnt::popcnt(square.to_bb());
        for i in 0..n {
            let occupancies = 0;
            let bb = rook_attacks(square, occupancies);
            let index = ((occupancies * ROOK_MAGICS[square.i()]) >> SHIFT_ROOK[square.i()]) as usize;
            if ATTACK_ROOK[index] != 0 && ATTACK_ROOK[index] != bb {
                panic!();
            } else {
                ATTACK_ROOK[index] = bb;
            }
        }
    }
}

pub fn rook_attacks(from: Square, occupancy: Bitboard) -> Bitboard {
    let index = ((occupancy * ROOK_MAGICS[from.i()]) >> SHIFT_ROOK[from.i()]) as usize;
    unsafe {
        ATTACK_ROOK[index]
    }
}

#[derive(Debug, Copy, Clone)]
pub struct Magic {
    mask: Bitboard,
    multiplier: u64,
    right_shift: usize,
}

impl Magic {
    /// See [https://www.chessprogramming.org/Magic_Bitboards] for more information.
    pub fn hash(&self, bb: Bitboard) -> u64 {
        ((bb & self.mask) * self.multiplier) >> self.right_shift
    }

    pub fn by_file() -> Box<[Magic; Square::count()]> {
        Box::new(array_init(|i| {
            let sq = Square::new_unchecked(i as u8);
            let area = sq.file().i() * File::count();
            Magic {
                mask: 0x7f << area,
                multiplier: 1,
                right_shift: area + 1,
            }
        }))
    }

    pub fn by_rank() -> Box<[Magic; Square::count()]> {
        // FIXME
        Box::new(array_init(|i| {
            let sq = Square::new_unchecked(i as u8);
            Magic {
                mask: 0x7e << (8 * sq.file().i()),
                multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
                right_shift: 58,
            }
        }))
    }

    pub fn by_main_diagonal() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }

    pub fn by_anti_diagonal() -> [Magic; Square::count()] {
        //Vec::from_iter(Square::iter()
        //    .map(|sq| Magic {
        //        mask: 0x7e << (8 * sq.file().i()),
        //        multiplier: 0x2 * (0x100 * (8 - sq.file().i())) as u64,
        //        right_shift: 58,
        //    }))[..];
        unimplemented!()
    }
}

impl fmt::Display for Magic {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(
            f,
            "(BB & 0x{:x}) * 0x{:x} >> 0o{:o}",
            self.mask, self.multiplier, self.right_shift
        )
    }
}

pub struct MagicsDb {
    by_file: [Bitboard; Square::count()],
    by_rank: [Bitboard; Square::count()],
    by_main_diagonal: [Bitboard; Square::count()],
    bb_anti_diagonal: [Bitboard; Square::count()],
}

impl Default for MagicsDb {
    fn default() -> Self {
        unimplemented!()
    }
}

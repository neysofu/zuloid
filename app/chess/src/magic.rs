use super::*;
use lazy_static::lazy_static;

pub struct Magic {
    mask: Bitboard,
    multiplier: u64,
    right_shift: usize,
}

impl Magic {
    pub fn magify(&self, bb: Bitboard) -> usize {
        (((bb & self.mask) * self.multiplier) >> self.right_shift) as usize
    }
}

lazy_static! {
    pub static ref BOARD_FRAME: Bitboard = File::from('a').to_bb()
        | File::from('h').to_bb()
        | Rank::from('1').to_bb()
        | Rank::from('8').to_bb();
    pub static ref FILE_MAGICS: [Magic; 8] = {
        fn magic(file_i: u64) -> Magic {
            Magic {
                mask: 0x7e << (8 * file_i),
                multiplier: 0x2 * (0x100 * (8 - file_i)),
                right_shift: 58,
            }
        }
        [
            magic(0),
            magic(1),
            magic(2),
            magic(3),
            magic(4),
            magic(5),
            magic(6),
            magic(7),
        ]
    };
    pub static ref RANK_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref DIAGONAL_A1H8_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref DIAGONAL_H1A8_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref FILE_ATTACKS: [Bitboard; 4096] = { unimplemented!() };
}

pub fn rook_magics(square: Square, occupancy: Bitboard) -> Bitboard {
    unimplemented!()
}

pub fn bishop_magics(square: Square, occupancy: Bitboard) -> Bitboard {
    unimplemented!()
}

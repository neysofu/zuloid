use lazy_static::lazy_static;
use zorro_chess::SlidingPiecesMoveGen;
use zorro_chess::*;

#[derive(Debug, Copy, Clone)]
pub struct Magic {
    mask: Bitboard,
    multiplier: u64,
    right_shift: usize,
}

impl Magic {
    /// See [https://www.chessprogramming.org/Magic_Bitboards] for more information.
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
        fn file_magic(file_i: u64) -> Magic {
            Magic {
                mask: 0x7e << (8 * file_i),
                multiplier: 0x2 * (0x100 * (8 - file_i)),
                right_shift: 58,
            }
        }
        [
            file_magic(0),
            file_magic(1),
            file_magic(2),
            file_magic(3),
            file_magic(4),
            file_magic(5),
            file_magic(6),
            file_magic(7),
        ]
    };
    pub static ref RANK_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref DIAGONAL_A1H8_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref DIAGONAL_H1A8_MAGICS: [Magic; SQUARE_COUNT] = { unimplemented!() };
    pub static ref FILE_ATTACKS: [Bitboard; 4096] = { unimplemented!() };
}

pub fn rook_magics(_square: Square, _occupancy: Bitboard) -> Bitboard {
    unimplemented!()
}

pub fn bishop_magics(_square: Square, _occupancy: Bitboard) -> Bitboard {
    unimplemented!()
}

pub fn find_magics() {
    //    // FIXME
    //    loop {
    //        // Try a random magic.
    //        source_mask = randrange(1, 0x40) << 1
    //        source_mask <<= file_i * 8
    //        # print('Attempting the source mask {}'.format(bin(source_mask)))
    //        target_mask = randrange(1, 0x40) << 58
    //        # print('The target mask is {}'.format(bin(target_mask)))
    //        if target_mask % source_mask == 0:
    //            magic = int(target_mask / source_mask)
    //            if bin(source_mask).count('1') != bin(source_mask *
    // magic).count('1'):                # print('The magic candidate doesn\'t
    // have same bitcnt')                pass
    //            else:
    //                # print('Found a possible magic value: {}'.format(hex(magic)))
    //                found = True
    //                # Check with every possible source/target pair.
    //                for source_mask in range(0, 0x40):
    //                    source_mask <<= 1
    //                    source_mask <<= file_i * 8
    //                    target_mask_s = bin(source_mask * magic)
    //                    # print('Testing compatibility with
    // {}'.format(hex(source_mask)))                    if
    // bin(source_mask).count('1') != target_mask_s.count('1'):
    // found = False                        break
    //                if found and (0x7e << (file_i * 8)) * magic ==
    // 0xfc00000000000000:                    # print('Tests ok')
    //                    return magic
    //    }
}

pub struct SlidingAttacksDatabase {
    // FIXME: exact buffer sizes.
    bb_rooks: [Bitboard; 86000],
    bb_bishopts: [Bitboard; 86000],
}

impl SlidingPiecesMoveGen for SlidingAttacksDatabase {
    fn gen_rooks(&self, buf: &mut [Move], attackers: Bitboard, all: Bitboard) -> usize {
        0
    }

    fn gen_bishops(&self, buf: &mut [Move], attackers: Bitboard, all: Bitboard) -> usize {
        0
    }
}

impl Default for SlidingAttacksDatabase {
    fn default() -> Self {
        // Initialize all bitboards based on the magics.
        SlidingAttacksDatabase {
            bb_rooks: [0; 86000],
            bb_bishopts: [0; 86000],
        }
    }
}

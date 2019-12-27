mod king;
mod knight;

pub use king::KING_ATTACKS;
pub use knight::KNIGHT_ATTACKS;

use crate::chess::*;
use array_init;

pub fn boxed_king_attacks() -> Box<[BitBoard; Square::count()]> {
    Box::new(array_init::array_init(|i| {
        king_attacks(Square::new(i as u8))
    }))
}

pub fn boxed_knight_attacks() -> Box<[BitBoard; Square::count()]> {
    Box::new(array_init::array_init(|i| {
        knight_attacks(Square::new(i as u8))
    }))
}

fn knight_attacks(from: Square) -> BitBoard {
    let shifts = [
        (-1, 2),
        (1, 2),
        (2, 1),
        (2, -1),
        (1, -2),
        (-1, -2),
        (-2, -1),
        (-2, 1),
    ];
    let mut bb = 0;
    for shift in shifts.iter() {
        if let (Some(file), Some(rank)) = (from.file().shift(shift.0), from.rank().shift(shift.1)) {
            bb |= Square::at(file, rank).to_bb();
        }
    }
    bb
}

fn king_attacks(from: Square) -> BitBoard {
    let shifts = [-1, 1];
    let mut file_bb = from.file().to_bb();
    let mut rank_bb = from.rank().to_bb();
    for shift in shifts.iter() {
        if let Some(file) = from.file().shift(*shift) {
            file_bb |= file.to_bb();
        }
        if let Some(rank) = from.rank().shift(*shift) {
            rank_bb |= rank.to_bb();
        }
    }
    (file_bb & rank_bb) ^ from.to_bb()
}

pub fn rook_attacks(from: Square, occupancy: BitBoard) -> BitBoard {
    let mut bb = 0;
    let shifts = [-1, 1, 8, -8];
    for shift in shifts.iter() {
        let mut to = from;
        while let Some(to_next) = Square::new_opt(to.i() as i32 + shift) {
            if to_next.file() != to.file() && to_next.rank() != to.rank() {
                break;
            }
            to = to_next;
            bb |= to.to_bb();
            if to.to_bb() & occupancy != 0 {
                break;
            }
        }
    }
    bb
}

pub fn bishop_attacks(from: Square, occupancy: BitBoard) -> BitBoard {
    let mut bb = 0;
    let shifts = [-9, -7, 9, 7];
    for shift in shifts.iter() {
        let mut to = from;
        while let Some(to_next) = Square::new_opt(to.i() as i32 + shift) {
            if (to_next.file().i() as i32 - to.file().i() as i32).abs() != 1
                && (to_next.rank().i() as i32 - to.rank().i() as i32).abs() != 1
            {
                break;
            }
            to = to_next;
            bb |= to.to_bb();
            if to.to_bb() & occupancy != 0 {
                break;
            }
        }
    }
    bb
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn gen_king_attacks() {
        let expected = *boxed_king_attacks();
        let actual = king::KING_ATTACKS;
        assert!(expected.iter().zip(actual.iter()).all(|(a, b)| a == b));
    }

    #[test]
    fn gen_knight_attacks() {
        let expected = *boxed_knight_attacks();
        let actual = knight::KNIGHT_ATTACKS;
        assert!(expected.iter().zip(actual.iter()).all(|(a, b)| a == b));
    }

    #[test]
    fn free_rook_in_a1() {
        let expected = File::A.to_bb() ^ Rank::_1.to_bb();
        let actual = rook_attacks(Square::A1, 0);
        assert_eq!(expected, actual);
    }

    #[test]
    fn trapped_rook_in_e4() {
        let expected = 8882127112200;
        let actual = rook_attacks(Square::E4, 8881992368128);
        assert_eq!(expected, actual);
    }
}

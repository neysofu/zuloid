use super::*;
use array_init::array_init;
use lazy_static::lazy_static;
use std::fmt;
use std::iter::FromIterator;

pub struct Mover {
    by_file: [BitBoard; Square::count()],
    by_rank: [BitBoard; Square::count()],
    by_main_diagonal: [BitBoard; Square::count()],
    bb_anti_diagonal: [BitBoard; Square::count()],
}

impl Default for Mover {
    fn default() -> Self {
        unimplemented!()
    }
}

/// A pre-initialized sliding pieces attack database.
impl Mover {
    pub fn list_legals(&self, board: &Board, move_list: &mut AvailableMoves) {
        self.gen_pawns(board, move_list);
        self.gen_knights(board, move_list);
        self.gen_king(board, move_list);
        self.gen_sliding_pieces(board, move_list);
    }

    fn gen_pawns(&self, board: &Board, move_list: &mut AvailableMoves) {
        let bb_all = board.bb_all();
        let attackers = board.attackers();
        let defenders = board.defenders();
        fn push(attackers: BitBoard, all: BitBoard, mover: Color) -> BitBoard {
            !all & match mover {
                Color::White => attackers.north(1),
                Color::Black => attackers.south(1),
            }
        }
        let single_pushes = push(
            board.attackers_with_role(Role::Pawn),
            bb_all,
            board.color_to_move,
        );
        let double_pushes = push(single_pushes, bb_all, board.color_to_move);
        let mut captures_east = attackers & !File::H.to_bb();
        let mut captures_west = attackers & !File::A.to_bb();
        let shifts: [i32; 4];
        match board.color_to_move {
            Color::White => {
                captures_east <<= 9;
                captures_west >>= 7;
                shifts = [-1, -2, -9, 7];
            }
            Color::Black => {
                captures_east <<= 7;
                captures_west >>= 9;
                shifts = [1, 2, -7, 9];
            }
        }
        captures_east &= defenders;
        captures_west &= defenders;
        let sources = [single_pushes, double_pushes, captures_east, captures_west];
        for (i, src) in sources.iter().enumerate() {
            for square in src.squares() {
                move_list.push(Move {
                    from: square.shift(shifts[i]).unwrap(),
                    to: square,
                    promotion: None,
                });
            }
        }
    }

    fn gen_knights(&self, board: &Board, move_list: &mut AvailableMoves) {
        for from in board.attackers_with_role(Role::Knight).squares() {
            let possible_targets = KNIGHT[from.i() as usize] & !board.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                });
            }
        }
    }

    fn gen_king(&self, board: &Board, move_list: &mut AvailableMoves) {
        for from in board.attackers_with_role(Role::King).squares() {
            let possible_targets = KING[from.i() as usize] & !board.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                });
            }
        }
    }

    fn gen_sliding_pieces(&self, board: &Board, move_list: &mut AvailableMoves) {
        //let bb_all = self.bb_colors[Color::White] |
        // self.bb_colors[Color::Black]; MAGICS.gen_bishops(
        //    move_list,
        //    self.attackers_with_role(Role::Bishop),
        //    bb_all,
        //);
        //MAGICS.gen_rooks(
        //    move_list,
        //    self.attackers_with_role(Role::Rook),
        //    bb_all,
        //);
    }
}

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

lazy_static! {
    pub static ref KNIGHT: [BitBoard; Square::count()] = {
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
        let mut bitboards = [0; Square::count()];
        for square in Square::iter() {
            let mut bb = 0;
            for shift in shifts.iter() {
                if let (Some(file), Some(rank)) =
                    (square.file().shift(shift.0), square.rank().shift(shift.1))
                {
                    bb |= Square::at(file, rank).to_bb();
                }
            }
            bitboards[square.i() as usize] = bb;
        }
        bitboards
    };
    pub static ref KING: [BitBoard; Square::count()] = {
        let shifts = [-1, 1];
        let mut bitboards = [0; Square::count()];
        for square in Square::iter() {
            let mut File = square.file().to_bb();
            let mut Rank = square.rank().to_bb();
            for shift in shifts.iter() {
                if let Some(file) = square.file().shift(*shift) {
                    File |= file.to_bb();
                }
                if let Some(rank) = square.rank().shift(*shift) {
                    Rank |= rank.to_bb();
                }
            }
            bitboards[square.i() as usize] = (File & Rank) ^ square.to_bb();
        }
        bitboards
    };
}

use super::magic::MAGICS;
use super::*;

use lazy_static::lazy_static;

/// A pre-initialized sliding pieces attack database.
pub trait SlidingPiecesMoveGen: Default + Sized {
    fn gen_rooks(&self, buf: &mut AvailableMoves, rooks: BitBoard, all: BitBoard) -> usize;
    fn gen_bishops(&self, buf: &mut AvailableMoves, bishops: BitBoard, all: BitBoard) -> usize;
}

impl Board {
    pub fn list_legals(&self, move_list: &mut AvailableMoves) {
        let mut count = 0;
        self.gen_pawns(move_list);
        self.gen_knights(move_list);
        self.gen_king(move_list);
        self.gen_sliding_pieces(move_list);
    }

    fn gen_pawns(&self, move_list: &mut AvailableMoves) {
        let bb_all = self.bb_all();
        let attackers = self.attackers();
        let defenders = self.defenders();
        fn push(attackers: BitBoard, all: BitBoard, mover: Color) -> BitBoard {
            !all & match mover {
                Color::White => attackers.north(1),
                Color::Black => attackers.south(1),
            }
        }
        let single_pushes = push(
            self.attackers_with_role(Role::Pawn),
            bb_all,
            self.color_to_move,
        );
        let double_pushes = push(single_pushes, bb_all, self.color_to_move);
        let mut captures_east = attackers & !File::new(7).to_bb();
        let mut captures_west = attackers & !File::new(0).to_bb();
        let shifts: [i32; 4];
        match self.color_to_move {
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

    fn gen_knights(&self, move_list: &mut AvailableMoves) {
        for from in self.attackers_with_role(Role::Knight).squares() {
            let possible_targets = KNIGHT[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                });
            }
        }
    }

    fn gen_king(&self, move_list: &mut AvailableMoves) {
        for from in self.attackers_with_role(Role::King).squares() {
            let possible_targets = KING[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                });
            }
        }
    }

    fn gen_sliding_pieces(&self, move_list: &mut AvailableMoves) {
        let bb_all = self.bb_colors[Color::White] | self.bb_colors[Color::Black];
        MAGICS.gen_bishops(
            move_list,
            self.attackers_with_role(Role::Bishop),
            bb_all,
        );
        MAGICS.gen_rooks(
            move_list,
            self.attackers_with_role(Role::Rook),
            bb_all,
        );
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

#[cfg(test)]
mod test {
    use super::*;
    use std::str::FromStr;

    #[test]
    fn knight_attacks_a2() {
        let attacker = Square::A2;
        let attacks = Square::C1.to_bb()
            | Square::C3.to_bb()
            | Square::B4.to_bb();
        assert_eq!(KNIGHT[attacker.i() as usize], attacks);
    }

    #[test]
    fn king_attacks_f8() {
        let attacker = Square::F8;
        assert_eq!(KING[attacker.i() as usize], 0xc0_40c0_0000_0000);
    }

    #[test]
    fn initial_board_has_16_pawn_moves() {
        let board = Board::default();
        let mut move_list = AvailableMoves::default();
        board.gen_pawns(&mut move_list);
        assert_eq!(move_list.as_slice().len(), 16);
    }

    #[test]
    fn initial_board_has_4_knight_moves() {
        let board = Board::default();
        let mut move_list = AvailableMoves::default();
        board.gen_knights(&mut move_list);
        assert_eq!(move_list.as_slice().len(), 4);
    }

    #[test]
    fn initial_board_has_0_king_moves() {
        let board = Board::default();
        let mut move_list = AvailableMoves::default();
        board.gen_king(&mut move_list);
        assert_eq!(move_list.as_slice().len(), 0);
    }
}

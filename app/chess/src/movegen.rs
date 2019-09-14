use super::*;
use lazy_static::lazy_static;

/// A pre-initialized sliding pieces attack database.
pub trait SlidingPiecesMoveGen: Default + Sized {
    fn gen_rooks(&self, buf: &mut [Move], rooks: BitBoard, all: BitBoard) -> usize;
    fn gen_bishops(&self, buf: &mut [Move], bishops: BitBoard, all: BitBoard) -> usize;
}

/// Some terminology:
///
/// > attackers
///     All pieces of the moving color.
/// > defenders
///     All pieces of the non-moving color.
/// > <plural role name>
///     All attackers with a certain role. Note that it does *not* include
/// defenders. > all
///     All pieces on the board.
impl Board {
    pub fn list_legals<'t, M: SlidingPiecesMoveGen>(
        &self,
        buf: &'t mut [Move],
        magic_mover: &M,
    ) -> impl Iterator<Item = Move> + 't {
        let _mover = self.color_to_move;
        let mut count = 0;
        let bb_all = self.bb_colors[Color::White] | self.bb_colors[Color::Black];
        count += self.gen_pawns(&mut buf[count..]);
        count += self.gen_knights(&mut buf[count..]);
        count += self.gen_king(&mut buf[count..]);
        // Sliding pieces.
        count += magic_mover.gen_bishops(
            &mut buf[count..],
            self.attackers_with_role(Role::Bishop),
            bb_all,
        );
        count += magic_mover.gen_rooks(
            &mut buf[count..],
            self.attackers_with_role(Role::Rook),
            bb_all,
        );
        // Note that queen moves are already included in bishops' and rooks'.
        (0..count).map(move |i| buf[i])
    }

    fn gen_pawns(&self, buf: &mut [Move]) -> usize {
        let bb_all = self.bb_all();
        let attackers = self.attackers();
        let defenders = self.defenders();
        fn push(attackers: BitBoard, all: BitBoard, mover: Color) -> BitBoard {
            !all & match mover {
                Color::White => attackers.north(1),
                Color::Black => attackers.south(1),
            }
        }
        let mut count = 0;
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
                buf[count] = Move {
                    from: square.shift(shifts[i]).unwrap(),
                    to: square,
                    promotion: None,
                };
                count += 1;
            }
        }
        count
    }

    fn gen_knights(&self, buf: &mut [Move]) -> usize {
        let mut count = 0;
        for from in self.attackers_with_role(Role::Knight).squares() {
            let possible_targets = KNIGHT[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                let mv = Move {
                    from,
                    to,
                    promotion: None,
                };
                buf[count] = mv;
                count += 1;
            }
        }
        count
    }

    fn gen_king(&self, buf: &mut [Move]) -> usize {
        let mut count = 0;
        for from in self.attackers_with_role(Role::King).squares() {
            let possible_targets = KING[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                let mv = Move {
                    from,
                    to,
                    promotion: None,
                };
                buf[count] = mv;
                count += 1;
            }
        }
        count
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
        let attacker = Square::from_str("a2").unwrap();
        let attacks = Square::from_str("c1").unwrap().to_bb()
            | Square::from_str("c3").unwrap().to_bb()
            | Square::from_str("b4").unwrap().to_bb();
        assert_eq!(KNIGHT[attacker.i() as usize], attacks);
    }

    #[test]
    fn king_attacks_f8() {
        let attacker = Square::from_str("f8").unwrap();
        assert_eq!(KING[attacker.i() as usize], 0xc0_40c0_0000_0000);
    }

    #[test]
    fn initial_board_has_16_pawn_moves() {
        let board = Board::default();
        let mut buf = [Move::new_garbage(); 256];
        assert_eq!(board.gen_pawns(&mut buf), 16);
    }

    #[test]
    fn initial_board_has_4_knight_moves() {
        let board = Board::default();
        let mut buf = [Move::new_garbage(); 256];
        assert_eq!(board.gen_knights(&mut buf), 4);
    }

    #[test]
    fn initial_board_has_0_king_moves() {
        let board = Board::default();
        let mut buf = [Move::new_garbage(); 256];
        assert_eq!(board.gen_king(&mut buf), 0);
    }
}

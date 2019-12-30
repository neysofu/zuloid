use super::tables;
use super::*;
use array_init::array_init;
use std::fmt;

/// A pre-initialized sliding pieces attack database.
impl Board {
    pub fn list_pseudolegals(&mut self, move_list: &mut AvailableMoves) {
        self.gen_pawns(move_list);
        self.gen_knights(move_list);
        self.gen_king(move_list);
        self.gen_sliding_pieces(move_list);
    }

    pub fn list_legals(&mut self, move_list: &mut AvailableMoves) {
        let mut pseudolegals = AvailableMoves::default();
        self.list_pseudolegals(&mut pseudolegals);
        for m in (&pseudolegals).into_iter() {
            let mut temp = AvailableMoves::default();
            let captured = self.do_move(m);
            self.list_pseudolegals(&mut temp);
            let mut is_legal = true;
            for m in (&temp).into_iter() {
                let captured = self.do_move(m);
                if !self.has_both_kings() {
                    is_legal = false;
                    self.undo_move(m, captured);
                    break;
                }
                self.undo_move(m, captured);
            }
            if is_legal {
                move_list.push(m);
            }
            self.undo_move(m, captured)
        }
    }

    fn gen_pawns(&self, move_list: &mut AvailableMoves) {
        let bb_all = self.bb_all();
        let bb_pawns = self.attackers_with_role(Role::Pawn);
        let bb_defenders = self.defenders();
        let push = |bb: BitBoard| {
            !bb_all
                & match self.color_to_move {
                    Color::W => bb.north(1),
                    Color::B => bb.south(1),
                }
        };
        let bb_push_1 = push(bb_pawns);
        let bb_push_2 = push(
            bb_push_1 & Rank::new_with_side(2, self.color_to_move).to_bb(),
        );
        let mut bb_captures_east = bb_pawns & !File::H.to_bb();
        let mut bb_captures_west = bb_pawns & !File::A.to_bb();
        let shifts: [i32; 4];
        match self.color_to_move {
            Color::W => {
                bb_captures_east <<= 9;
                bb_captures_west >>= 7;
                shifts = [-1, -2, -9, 7];
            }
            Color::B => {
                bb_captures_east <<= 7;
                bb_captures_west >>= 9;
                shifts = [1, 2, -7, 9];
            }
        }
        bb_captures_east &= bb_defenders;
        bb_captures_west &= bb_defenders;
        let sources =
            [bb_push_1, bb_push_2, bb_captures_east, bb_captures_west];
        for (i, src) in sources.iter().enumerate() {
            for square in src.squares() {
                move_list.push(Move {
                    from: square.shift(shifts[i]).unwrap(),
                    to: square,
                    promotion: None,
                    capture: self.piece_opt_at(square).map(|p| p.role),
                });
            }
        }
    }

    fn gen_knights(&self, move_list: &mut AvailableMoves) {
        for from in self.attackers_with_role(Role::Knight).squares() {
            let possible_targets =
                tables::KNIGHT_ATTACKS[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                    capture: None,
                });
            }
        }
    }

    fn gen_king(&self, move_list: &mut AvailableMoves) {
        for from in self.attackers_with_role(Role::King).squares() {
            let possible_targets =
                tables::KING_ATTACKS[from.i() as usize] & !self.attackers();
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                    capture: None,
                });
            }
        }
    }

    fn gen_sliding_pieces(&self, move_list: &mut AvailableMoves) {
        for from in self.attackers_with_role(Role::Rook).squares() {
            let possible_targets = tables::rook_attacks(from, self.bb_all())
                & !self.bb_colors[self.color_to_move];
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                    capture: None,
                });
            }
        }
        for from in self.attackers_with_role(Role::Bishop).squares() {
            let possible_targets = tables::bishop_attacks(from, self.bb_all())
                & !self.bb_colors[self.color_to_move];
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                    capture: None,
                });
            }
        }
        for from in self.attackers_with_role(Role::Queen).squares() {
            let possible_targets = (tables::rook_attacks(from, self.bb_all())
                | tables::bishop_attacks(from, self.bb_all()))
                & !self.bb_colors[self.color_to_move];
            for to in possible_targets.squares() {
                move_list.push(Move {
                    from,
                    to,
                    promotion: None,
                    capture: None,
                });
            }
        }
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

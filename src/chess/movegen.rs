use super::coordinates::{Bitboard, Square};
use super::{Board, Move, Role};
use bitintr::Blsi;
use bitintr::Tzcnt;
use std::str::FromStr;

impl Board {
    pub fn gen_moves<'t>(&'t self) -> impl Iterator<Item = Move> + 't {
        MoveGenerator {
            board: self,
            moves: [Move::from_str("a1a1").unwrap(); MAX_MOVES],
            moves_count: 0,
        }
    }
}

const MAX_MOVES: usize = 225;

struct MoveGenerator<'t> {
    board: &'t Board,
    moves: [Move; MAX_MOVES],
    moves_count: usize,
}

impl<'t> Iterator for MoveGenerator<'t> {
    type Item = Move;

    fn next(&mut self) -> Option<Self::Item> {
        self.moves_count += self.gen_pawns();
        None
    }
}

impl<'t> MoveGenerator<'t> {
    fn gen_pawns(&self) -> usize {
        unimplemented!()
    }

    fn gen_knights(&mut self, attackers: Bitboard, mask: Bitboard) -> usize {
        let initial_moves_count = self.moves_count;
        for attacker in attackers.squares() {
            let attacks = Role::Knight.attacks(attacker) & mask;
            for attack in attacks.squares() {
                let mv = Move {
                    from: attacker,
                    to: attack,
                    promotion: None,
                };
                self.moves[self.moves_count] = mv;
                self.moves_count += 1;
            }
        }
        initial_moves_count - self.moves_count
    }
}

pub trait BitboardUtils {
    fn squares(self) -> BitsCounter;
}

impl BitboardUtils for Bitboard {
    fn squares(self) -> BitsCounter {
        BitsCounter { bb: self }
    }
}

pub struct BitsCounter {
    bb: Bitboard,
}

impl Iterator for BitsCounter {
    type Item = Square;

    fn next(&mut self) -> Option<Self::Item> {
        if self.bb == 0 {
            None
        } else {
            let i = self.bb.tzcnt() as u8;
            let square_bb = self.bb.blsi();
            self.bb &= square_bb;
            Some(Square::from_i(i))
        }
    }
}

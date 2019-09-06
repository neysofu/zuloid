use super::piece::*;
use super::*;

impl Board {
    pub fn list_legals<'t>(&self, buf: &'t mut [Move]) -> impl Iterator<Item = Move> + 't {
        let mover = self.color_to_move;
        let mut count = 0;
        count += self.gen_pawns(
            &mut buf[count..],
            self.bb_colors[mover] & self.bb_roles[Role::Pawn],
            self.bb_colors[!mover],
            mover,
        );
        count += self.gen_knights(
            &mut buf[count..],
            self.bb_colors[mover] & self.bb_roles[Role::Knight],
        );
        count += self.gen_king(
            &mut buf[count..],
            self.bb_colors[mover] & self.bb_roles[Role::King],
        );
        (0..count).map(move |i| buf[i])
    }

    fn gen_pawns(
        &self,
        buf: &mut [Move],
        attackers: Bitboard,
        defenders: Bitboard,
        mover: Color,
    ) -> usize {
        let all = attackers | defenders;
        fn push(attackers: Bitboard, all: Bitboard, mover: Color) -> Bitboard {
            !all & match mover {
                Color::White => attackers.north(1),
                Color::Black => attackers.south(1),
            }
        }
        let mut count = 0;
        let single_pushes = push(attackers, all, mover);
        let double_pushes = push(single_pushes, all, mover);
        let mut captures_east = attackers & !File::new(7).to_bb();
        let mut captures_west = attackers & !File::new(0).to_bb();
        let shifts: [i32; 4];
        match mover {
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
        for square in single_pushes.squares() {
            buf[count] = Move {
                from: square.shift(shifts[0]).unwrap(),
                to: square,
                promotion: None,
            };
            count += 1;
        }
        for square in double_pushes.squares() {
            buf[count] = Move {
                from: square.shift(shifts[1]).unwrap(),
                to: square,
                promotion: None,
            };
            count += 1;
        }
        for square in captures_east.squares() {
            buf[count] = Move {
                from: square.shift(shifts[2]).unwrap(),
                to: square,
                promotion: None,
            };
            count += 1;
        }
        for square in captures_west.squares() {
            buf[count] = Move {
                from: square.shift(shifts[3]).unwrap(),
                to: square,
                promotion: None,
            };
            count += 1;
        }
        count
    }

    fn gen_knights(&self, buf: &mut [Move], attackers: Bitboard) -> usize {
        let mut count = 0;
        for attacker in attackers.squares() {
            let attacks = KNIGHT[attacker.i() as usize];
            for attack in attacks.squares() {
                let mv = Move {
                    from: attacker,
                    to: attack,
                    promotion: None,
                };
                buf[count] = mv;
                count += 1;
            }
        }
        count
    }

    fn gen_rooks(&self, _buf: &mut [Move], _attackers: Bitboard, _all: Bitboard) -> usize {
        let count = 0;
        // TODO
        count
    }

    fn gen_king(&self, buf: &mut [Move], attackers: Bitboard) -> usize {
        let mut count = 0;
        for attacker in attackers.squares() {
            let attacks = KING[attacker.i() as usize];
            for attack in attacks.squares() {
                let mv = Move {
                    from: attacker,
                    to: attack,
                    promotion: None,
                };
                buf[count] = mv;
                count += 1;
            }
        }
        count
    }
}

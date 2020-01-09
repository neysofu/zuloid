use crate::chess::*;
use bitintr::Popcnt;
use std::fmt;

// EXTERNAL INTERFACE
// ------------------

pub struct Eval {
    pub score: i32,
    pub sharpness: i32,
    pub best_move: Move,
    pub mate_in: Option<u32>,
}

pub struct Detailed {
    pub eval: Eval,
}

impl Eval {
    pub fn new(board: &Board) -> Self {
        let mut eval = Eval::default();
        eval.score = count_materials(board);
        let pawns_count = board.bb_roles[Role::Pawn].popcnt() as usize;
        let knights_count = board.bb_roles[Role::Knight].popcnt() as usize;
        let rooks_count = board.bb_roles[Role::Rook].popcnt() as usize;
        eval.score += KNIGHT_ADJ[knights_count];
        eval.score += ROOK_ADJ[rooks_count];
        eval
    }
}

const KNIGHT_ADJ: [i32; 9] = [-200, -160, -120, -80, -40, 0, 40, 80, 120];
const ROOK_ADJ: [i32; 9] = [150, 120, 90, 60, 30, 0, -30, -60, -90];

#[derive(Copy, Clone, Default)]
struct EvalDetails {
    game_phase: i32,
    mg_mob: i32,
    eg_mob: i32,
    attack_count: i32,
    attack_weight: i32,
    king_shield: i32,
    material_adj: i32,
    blockages: i32,
    positional_themes: i32,
}

#[derive(Copy, Clone)]
pub struct ConciseEval;

fn piece_value(role: Role) -> i32 {
    match role {
        Role::Pawn => 1000,
        Role::Knight => 2800,
        Role::Bishop => 3200,
        Role::Rook => 4790,
        Role::Queen => 9290,
        Role::King => 613370,
    }
}

pub fn count_materials(board: &Board) -> i32 {
    let mut balance = 0;
    for sq in Square::iter() {
        if let Some(piece) = board.piece_opt_at(sq) {
            match piece.color {
                Color::W => balance += piece_value(piece.role),
                Color::B => balance -= piece_value(piece.role),
            }
        }
    }
    balance
}

impl Default for Eval {
    fn default() -> Self {
        Eval {
            score: 0,
            sharpness: 0,
            best_move: Move::ID,
            mate_in: None,
        }
    }
}

impl fmt::Display for Eval {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        unimplemented!()
    }
}

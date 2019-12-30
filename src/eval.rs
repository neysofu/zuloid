use crate::chess::*;

pub struct Eval {
    pub score: f32,
    pub sharpness: f32,
    pub best_move: Move,
    pub mate_in: Option<u32>,
}

impl Eval {
    pub fn new(board: &Board) -> Self {
        let mut eval = Eval::default();
        eval.score = count_materials(board);
        eval
    }
}

fn piece_value(role: Role) -> f32 {
    match role {
        Role::Pawn => 100.0,
        Role::Knight => 280.,
        Role::Bishop => 320.0,
        Role::Rook => 479.0,
        Role::Queen => 929.0,
        Role::King => 61337.0,
    }
}

fn count_materials(board: &Board) -> f32 {
    let mut balance = 0.0;
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
            score: 0.0,
            sharpness: 0.5,
            best_move: Move::new_garbage(),
            mate_in: None,
        }
    }
}

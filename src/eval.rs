use crate::chess::*;

pub struct Eval {
    pub score: f32,
    pub sharpness: f32,
    pub best_move: Move,
    pub mate_in: Option<u32>,
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

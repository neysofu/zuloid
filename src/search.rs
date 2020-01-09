//! Iterative deepening depth-first search. It implements a flexible search
//! algorithm.
//!
//! This is arguably the most important code in Zorro, ELO-wise.
//!
//! Resources:
//!   - https://en.wikipedia.org/wiki/Iterative_deepening_depth-first_search
//!   - https://www.chessprogramming.org/Iterative_Deepening
//!   - https://www.chessprogramming.org/Depth-First
//!   - https://stackoverflow.com/a/5278751/5148606

use crate::chess::*;
use crate::core::*;
use crate::eval::count_materials;
use crate::eval::Eval;
use std::vec;

struct Stack {
    board: Board,
    levels: Vec<Level>,
}

struct Level {
    generator: Move,
    capture: Option<Piece>,
    children: vec::IntoIter<Move>,
    best_move: Move,
    best_score: i32,
}

impl Stack {
    fn new(zorro: &Zorro) -> Self {
        let mut board = zorro.board.clone();
        let levels = vec![{
            let mut children = AvailableMoves::default();
            board.list_legals(&mut children);
            Level {
                generator: Move::ID,
                capture: None,
                children: children.into_iter(),
                best_move: Move::ID,
                best_score: std::i32::MIN,
            }
        }];
        Stack { board, levels }
    }

    fn push(&mut self, mv: Move) {
        let capture = self.board.do_move(mv);
        let mut children = AvailableMoves::default();
        self.board.list_legals(&mut children);
        self.levels.push(Level {
            generator: mv,
            capture,
            children: children.into_iter(),
            best_move: Move::ID,
            best_score: std::i32::MIN,
        });
    }

    fn pop(&mut self) {
        let last_level = self.levels.pop().unwrap();
        self.board
            .undo_move(last_level.generator, last_level.capture);
    }

    fn top_mut(&mut self) -> &mut Level {
        self.levels.last_mut().unwrap()
    }

    fn depth(&self) -> usize {
        self.levels.len()
    }

    fn candidate_move(&mut self, mv: Move, score: i32) {
        let top = self.top_mut();
        if top.best_score < score {
            top.best_move = mv;
            top.best_score = score;
        }
    }

    fn winner_move(&mut self) -> Move {
        unimplemented!()
    }
}

fn minimax_sign(color: Color) -> i32 {
    match color {
        Color::W => 1,
        Color::B => -1,
    }
}

/// See:
///   - https://en.wikipedia.org/wiki/Minimax#Pseudocode
pub fn iter_search(zorro: &mut Zorro) -> Eval {
    let mut eval = Eval::new(&zorro.board);
    let mut stack = Stack::new(&zorro);
    while stack.depth() != 0 {
        if let Some(mv) = stack.top_mut().children.next() {
            let score = 0;
            stack.candidate_move(mv, score);
            // TODO: reduce horizon effect.
            if stack.depth() == 4 {
                stack.pop();
            } else {
                stack.push(mv);
            }
        } else {
            stack.pop();
        }
    }
    eval.best_move = stack.winner_move();
    eval
}

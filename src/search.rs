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
use crossbeam_channel::unbounded;
use std::vec;

struct Stack {
    zorro: Zorro,
    best_move: Move,
    buffer: Vec<StackItem>,
}

type StackItem = (vec::IntoIter<Move>, Move, Option<Piece>);

impl Stack {
    fn new(zorro: &Zorro) -> Self {
        let mut root = AvailableMoves::default();
        zorro.board.clone().list_legals(&mut root);
        Stack {
            zorro: (*zorro).clone(),
            best_move: Move::ID,
            buffer: vec![(root.into_iter(), Move::ID, None)],
        }
    }

    fn depth(&self) -> usize {
        self.buffer.len()
    }

    fn push(&mut self, mv: Move, legals: AvailableMoves) {
        self.buffer.push((legals.into_iter(), mv, None));
    }

    fn pop(&mut self) {
        self.buffer.pop();
    }

    fn top_mut(&mut self) -> Option<&mut StackItem> {
        self.buffer.last_mut()
    }

    fn line(&self) -> impl Iterator<Item = Move> {
        self.buffer.clone().into_iter().map(|x| x.1)
    }
}

fn minimax_sign(color: Color) -> i32 {
    match color {
        Color::W => 1,
        Color::B => -1,
    }
}

pub fn iter_search(zorro: &mut Zorro) -> Eval {
    let mut eval = Eval::new(&zorro.board);
    let mut line_best: Vec<Move> = vec![];
    let mut line_best_value = std::i32::MIN;
    let mut stack = Stack::new(&zorro);
    while let Some(last) = stack.top_mut() {
        if let Some(mv) = last.0.next() {
            let mut buffer = AvailableMoves::default();
            let mv_capture = zorro.board.do_move(mv);
            let score = Eval::new(&zorro.board).score
                * minimax_sign(zorro.board.color_to_move);
            zorro.board.list_legals(&mut buffer);
            if stack.depth() < 2 {
                stack.push(mv, buffer);
            } else {
                if score > line_best_value {
                    line_best = vec![];
                    line_best_value = score;
                    for mv in stack.line() {
                        line_best.push(mv);
                    }
                }
                zorro.board.undo_move(mv, mv_capture);
            }
        } else {
            zorro.board.undo_move(last.1, last.2);
            stack.pop();
        }
    }
    eval.best_move = line_best[1];
    eval
}

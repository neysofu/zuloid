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
use crate::eval::Eval;
use std::vec;

struct Stack {
    board: Board,
    tree: Vec<vec::IntoIter<Move>>,
    line: Vec<Move>,
}

impl Stack {
    fn new(board: &Board) -> Self {
        let mut legal_moves = AvailableMoves::default();
        board.clone().list_legals(&mut legal_moves);
        Stack {
            board: board.clone(),
            tree: vec![legal_moves.into_iter()],
            line: vec![],
        }
    }

    fn backtrace(&mut self) {
        while let Some(last) = self.tree.last_mut() {
            if last.next().is_none() {
                // All nodes have been visited already.
                self.tree.pop();
            } else {
                // TODO: the result of '.next()' should be returned.
                return;
            }
        }
    }

    fn is_empty(&self) -> bool {
        self.tree.is_empty()
    }
}

pub fn search(zorro: &mut Zorro, depth: usize) -> Eval {
    let mut stack = Stack::new(&zorro.board);
    while !stack.is_empty() {
        // The branching condition determines wheather to keep exploring the
        // tree or to backtrace its way up.
        if stack.tree.len() < depth {
            let mut legal_moves = AvailableMoves::default();
            stack.board.list_legals(&mut legal_moves);
            stack.tree.push(legal_moves.into_iter());
        } else {
            println!("this is depth {}", stack.tree.len());
            stack.backtrace();
        }
    }
    let eval = Eval::default();
    //eval.best_move = best.unwrap();
    eval
}

pub struct Perft {
    board: Board,
    line: Vec<(Move, Option<Piece>)>,
    depth: usize,
    nodes_count: usize,
}

impl Visitor<usize> for Perft {
    fn new(board: &Board, config: &Config) -> Self {
        Perft {
            board: board.clone(),
            line: vec![],
            depth: config.max_depth.unwrap_or(2),
            nodes_count: 0,
        }
    }

    fn visit(&mut self, m: Move) -> bool {
        if self.line.len() > self.depth {
            false
        } else {
            self.line.push((m, self.board.do_move(m)));
            true
        }
    }

    fn backtrace(&mut self, depth: usize) {
        for (m, capture) in self.line.iter().rev().take(depth) {
            self.board.undo_move(*m, *capture);
        }
    }

    fn wrap_it_up(&mut self) -> usize {
        self.nodes_count
    }
}

pub trait Visitor<T> {
    fn new(board: &Board, config: &Config) -> Self;
    fn visit(&mut self, m: Move) -> bool;
    fn backtrace(&mut self, depth: usize);
    fn wrap_it_up(&mut self) -> T;
}

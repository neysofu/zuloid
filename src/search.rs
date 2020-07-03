//! Heuristic-driven depth-first search (HDDFS) is a novel search strategy
//! that was developed specifically for Zorro. HDDFS does not fight the horizon
//! effect by itself; instead, it employs a heuristic provided by the
//! evaluation function and time management system. Thus the game tree is only
//! searched up to a point where branchs are deemed interesting by the
//! evaluation function.

use crate::chess::*;
use crate::core::*;
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
    score: i32,
}

fn default_score(color: Color) -> i32 {
    (std::i32::MIN + 1) * minimax_sign(color)
}

impl Stack {
    /// Creates a new empty stack for the given board configuration.
    fn new(zorro: &Zorro) -> Self {
        let mut board = zorro.board.clone();
        let levels = vec![{
            let mut children = AvailableMoves::default();
            board.list_legals(&mut children);
            Level {
                generator: Move::IDENTITY,
                capture: None,
                children: children.into_iter(),
                best_move: Move::IDENTITY,
                score: default_score(board.color_to_move),
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
            best_move: Move::IDENTITY,
            score: default_score(self.board.color_to_move),
        });
    }

    fn pop(&mut self) {
        let last_level = self.levels.pop().unwrap();
        self.board
            .undo_move(last_level.generator, last_level.capture);
        let parent_level = self.levels.last_mut().unwrap();
        if (self.board.color_to_move == Color::B) ^ (last_level.score > parent_level.score) {
            parent_level.score = last_level.score;
            parent_level.best_move = last_level.generator;
        }
        println!("{}", Move::to_string(&last_level.generator));
    }

    fn visit_next(&mut self) -> Option<Move> {
        self.top_mut().children.next()
    }

    fn top_mut(&mut self) -> &mut Level {
        self.levels.last_mut().unwrap()
    }

    fn depth(&self) -> usize {
        self.levels.len()
    }

    fn heuristic(&self) -> i32 {
        Eval::new(&self.board).score
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
    let mut stack = Stack::new(&zorro);
    loop {
        if stack.depth() == 5 {
            // Terminal nodes have heuristic score assigned to them.
            stack.top_mut().score = stack.heuristic();
            stack.pop();
        } else if let Some(mv) = stack.visit_next() {
            stack.push(mv);
        } else if stack.depth() == 1 {
            eval.best_move = stack.levels[0].best_move;
            break;
        } else {
            stack.pop();
        }
    }
    eval
}

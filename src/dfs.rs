use crate::chess::*;
use crate::core::*;
use crate::eval::Eval;
use std::vec;

struct Search {
    board: Board,
    stack: Vec<Node>,
}

struct Node {
    parent: usize,
    generator: Move,
    capture: Option<Piece>,
    best_score: i32,
}

impl Search {

    pub fn new(zorro: &Zorro) -> Self {
        let mut board = zorro.board.clone();
        Search {
            board, stack: vec![Node {
                parent: 0,
                generator: Move::IDENTITY,
                capture: None,
                best_score: std::i32::MIN,
            }],
        }
    }

    pub fn push(&self, mv: Move) {
        self.stack.push(mv);
    }

    pub fn top(&self) -> Option<&Node> {
        self.stack.last()
    }

    pub fn pop(&mut self) {
        let last_level = self.levels.pop().unwrap();
        let parent_level = self.levels.last_mut().unwrap();
        if last_level.best_score > parent_level.best_score {
            parent_level.best_move = last_level.generator;
            parent_level.best_score = last_level.best_score;
        }
        self.board
            .undo_move(last_level.generator, last_level.capture);
    }
}

pub fn search(zorro: &mut Zorro) -> Eval {
    let mut eval = Eval::new(&zorro.board);
    let mut search = Search::new(&zorro);
    while !search.is_done() {
        search.visit();
        if stack.depth() == 4 {
            search.pop();
        } else {
            search.push(mv);
        }
    }
    eval.best_move = search.levels[0].best_move;
    eval
}

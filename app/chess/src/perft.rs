use crate::*;
use std::fmt;

#[derive(Clone)]
struct Level {
    buf: [Move; 256],
    i: usize,
    count: usize,
}

impl Level {
    fn init(&mut self, board: &Board) {
        self.count = board.list_legals(&mut self.buf[..]).count();
        self.i = 0;
    }
}

impl Default for Level {
    fn default() -> Self {
        Level {
            buf: [Move::new_garbage(); 256],
            i: 0,
            count: 0,
        }
    }
}

struct State {
    tree: Vec<Level>,
    current_depth: usize,
    desired_depth: usize,
    report: Report,
}

impl State {
    fn new(depth: usize, board: &Board) -> Self {
        let mut tree = vec![Level::default(); depth];
        tree[0].init(board);
        State {
            tree,
            report: Report::new(depth),
            current_depth: 0,
            desired_depth: depth,
        }
    }

    fn run(mut self, board: &mut Board) -> Report {
        while self.tree[0].i < self.tree[0].count {
            while self.tree[self.current_depth].i < self.tree[self.current_depth].count {
                let current_move =
                    self.tree[self.current_depth].buf[self.tree[self.current_depth].i];
                if self.current_depth == self.desired_depth {
                    self.report.nodes += board
                        .list_legals(&mut self.tree[self.current_depth].buf[..])
                        .count();
                } else {
                    board.do_move(current_move);
                    self.current_depth += 1;
                }
                if self.current_depth == self.desired_depth {
                    self.current_depth -= 1;
                }
                self.tree[self.current_depth].i += 1;
            }
        }
        self.report.clone()
    }
}

impl Board {
    pub fn perft(&mut self, depth: usize) -> Report {
        if depth == 0 {
            Report::new(0)
        } else if depth == 1 {
            let mut moves = [Move::new_garbage(); 256];
            let count = self.list_legals(&mut moves[..]).count();
            Report {
                depth,
                nodes: count,
                overview: moves.to_vec(),
            }
        } else {
            let mut total_moves_count = 0;
            let mut moves = [Move::new_garbage(); 256];
            let moves_count = self.list_legals(&mut moves[..]).count();
            for m in moves[..moves_count].iter() {
                self.do_move(*m);
                total_moves_count += self.perft(depth - 1).nodes;
                self.undo_move(*m);
            }
            Report {
                depth,
                nodes: total_moves_count,
                overview: moves.to_vec(),
            }
        }
    }
}

#[derive(Clone)]
pub struct Report {
    depth: usize,
    nodes: usize,
    overview: Vec<Move>,
}

impl Report {
    fn new(depth: usize) -> Self {
        Report {
            depth,
            nodes: 0,
            overview: Vec::default(),
        }
    }
}

impl fmt::Display for Report {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        Ok(())
    }
}

use super::*;
use std::fmt;

#[derive(Clone, Default)]
struct Level {
    buf: AvailableMoves,
    i: usize,
}

struct Perft {
    tree: Vec<Level>,
    current_depth: usize,
    desired_depth: usize,
    report: Report,
}

impl Perft {
    fn new(depth: usize, board: &Board) -> Self {
        unimplemented!()
        //let mut tree = vec![Level::default(); depth];
        //tree[0] = Level::default();
        //self.count = board.list_legals(&mut self.buf[..]).count();
        //self.i = 0;
        //Perft {
        //    tree,
        //    report: Report::new(depth),
        //    current_depth: 0,
        //    desired_depth: depth,
        //}
    }

    fn run(mut self, board: &mut Board) -> Report {
        unimplemented!()
        //while self.tree[0].i < self.tree[0].count {
        //    while self.tree[self.current_depth].i < self.tree[self.current_depth].count {
        //        let current_move =
        //            self.tree[self.current_depth].buf[self.tree[self.current_depth].i];
        //        if self.current_depth == self.desired_depth {
        //            self.report.nodes += board
        //                .list_legals(&mut self.tree[self.current_depth].buf[..])
        //                .count();
        //        } else {
        //            board.do_move(current_move);
        //            self.current_depth += 1;
        //        }
        //        if self.current_depth == self.desired_depth {
        //            self.current_depth -= 1;
        //        }
        //        self.tree[self.current_depth].i += 1;
        //    }
        //}
        //self.report.clone()
    }
}

impl Board {
    pub fn perft(&mut self, depth: usize) -> Report {
        let mut report = Report::new(depth);
        if depth == 0 {
            report.nodes_count = 1;
            return report;
        } else if depth == 1 {
            let mut moves = AvailableMoves::default();
            self.list_legals(&mut moves);
            Report {
                depth,
                nodes_count: moves.as_slice().len(),
                overview: moves.as_slice().to_vec(),
            }
        } else {
            let mut total_moves_count = 0;
            let mut moves = AvailableMoves::default();
            self.list_legals(&mut moves);
            for m in moves.as_slice().iter() {
                self.do_move(*m);
                total_moves_count += self.perft(depth - 1).nodes_count;
                self.undo_move(*m);
            }
            Report {
                depth,
                nodes_count: total_moves_count,
                overview: moves.as_slice().to_vec(),
            }
        }
    }
}

#[derive(Clone)]
pub struct Report {
    depth: usize,
    nodes_count: usize,
    overview: Vec<Move>,
}

impl Report {
    fn new(depth: usize) -> Self {
        Report {
            depth,
            nodes_count: 0,
            overview: vec![],
        }
    }
}

impl fmt::Display for Report {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        Ok(())
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn perft_0_is_1() {
        let report = Board::default().perft(0);
        assert_eq!(report.nodes_count, 1);
    }
}

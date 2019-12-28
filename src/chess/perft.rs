use super::*;
use std::fmt;
use std::time::{Duration, Instant};

impl Board {
    pub fn perft(&mut self, depth: usize) -> Report {
        let start = Instant::now();
        let mut report = Report::new(depth);
        if depth == 0 {
            report.nodes_count = 1;
        } else {
            let mut moves = AvailableMoves::default();
            self.list_legals(&mut moves);
            for m in moves.as_slice().iter() {
                let capture = self.do_move(*m);
                let count = self.perft(depth - 1).nodes_count;
                report.nodes_count += count;
                report.overview.push((*m, count));
                self.undo_move(*m, capture);
            }
        }
        report.duration = start.elapsed();
        report
    }

    pub fn backtrace_perft(&mut self, depth: usize) -> (usize, usize) {
        let report = self.perft(depth);
        if depth == 1 {
            return (report.nodes_count, self.shakmaty_perft(1));
        }
        for (m, count) in report.overview {
            let captured = self.do_move(m);
            let expected_count = self.shakmaty_perft(depth - 1);
            if count != expected_count {
                return self.backtrace_perft(depth - 1);
            }
            self.undo_move(m, captured);
        }
        (0, 0)
    }

    fn shakmaty_perft(&self, depth: usize) -> usize {
        use shakmaty::{fen::Fen, perft, Chess};
        let fen: Fen = self.fmt_fen(' ').to_string().parse().unwrap();
        let position: Chess = fen.position().unwrap();
        perft(&position, depth as u32) as usize
    }
}

#[derive(Clone)]
pub struct Report {
    depth: usize,
    nodes_count: usize,
    overview: Vec<(Move, usize)>,
    duration: Duration,
}

impl Report {
    fn new(depth: usize) -> Self {
        Report {
            depth,
            nodes_count: 0,
            overview: vec![],
            duration: Duration::default(),
        }
    }

    fn nodes_per_second(&self) -> usize {
        (self.nodes_count as f64 / (self.duration.as_nanos() as f64 / 10E9)).round() as usize
    }
}

impl fmt::Display for Report {
    fn fmt(&self, w: &mut fmt::Formatter) -> fmt::Result {
        writeln!(w, "\nPosition: 1/1")?;
        for m in self.overview.iter() {
            writeln!(w, "{}: {}", m.0, m.1)?;
        }
        writeln!(w, "\n===========================")?;
        writeln!(w, "Total time (ms) : {}", self.duration.as_millis())?;
        writeln!(w, "Nodes searched  : {}", self.nodes_count)?;
        writeln!(w, "Nodes/second    : {}", self.nodes_per_second())?;
        Ok(())
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn depth_0() {
        let board = Board::default();
        assert_eq!(board.clone().perft(0).nodes_count, 1);
    }

    #[test]
    fn depth_1() {
        let board = Board::default();
        assert_eq!(board.clone().perft(1).nodes_count, 20);
    }

    #[test]
    fn depth_2() {
        let board = Board::default();
        assert_eq!(board.clone().perft(2).nodes_count, 400);
    }

    #[test]
    fn depth_of_report_is_consistent_with_call() {
        let board = Board::default();
        assert_eq!(board.clone().perft(1).depth, 1);
        assert_eq!(board.clone().perft(2).depth, 2);
    }

    #[test]
    fn depth_3() {
        let board = Board::default();
        assert_eq!(board.clone().perft(3).nodes_count, 8902);
    }

    #[test]
    fn depth_3_doesnt_alter_the_board() {
        let mut board = Board::default();
        board.perft(3);
        assert_eq!(board, Board::default());
    }

    #[test]
    fn depth_4() {
        let board = Board::default();
        assert_eq!(board.clone().perft(4).nodes_count, 197281);
    }
}

use super::*;
use std::fmt;
use std::time::{Duration, Instant};

pub fn bare_perft(board: &mut Board, depth: usize) -> usize {
    if depth == 0 {
        return 1;
    }
    let mut current_depth = 1;
    let mut nodes_count = 0;
    let mut buffer = AvailableMoves::default();
    board.list_legals(&mut buffer);
    let mut stack = vec![(buffer.into_iter(), Move::ID, None)];
    while let Some(last) = stack.last_mut() {
        if let Some(next_move) = last.0.next() {
            if current_depth >= depth as i32 {
                nodes_count += 1;
            } else {
                current_depth += 1;
                let next_capture = board.do_move(next_move);
                let mut buffer = AvailableMoves::default();
                board.list_legals(&mut buffer);
                stack.push((buffer.into_iter(), next_move, next_capture));
            }
        } else if current_depth == 1 {
            break;
        } else {
            current_depth -= 1;
            board.undo_move(last.1, last.2);
            stack.pop().unwrap();
        }
    }
    nodes_count
}

fn shakmaty_perft(board: &Board, depth: usize) -> usize {
    use shakmaty::{fen::Fen, perft, Chess};
    let fen: Fen = board.fmt_fen(' ').to_string().parse().unwrap();
    let position: Chess = fen.position().unwrap();
    perft(&position, depth as u32) as usize
}

pub fn perft(board: &mut Board, depth: usize) -> Report {
    let mut report = Report::new(depth);
    let start = Instant::now();
    if depth == 0 {
        report.nodes_count = 1;
    } else {
        let mut legal_moves = AvailableMoves::default();
        board.list_legals(&mut legal_moves);
        for m in legal_moves.into_iter() {
            let capture = board.do_move(m);
            let result = bare_perft(board, depth - 1);
            report.overview.push((m, result));
            report.nodes_count += result;
            board.undo_move(m, capture);
        }
    }
    report.duration += start.elapsed();
    report
}

impl Board {
    pub fn backtrace_perft(&mut self, depth: usize) -> (usize, usize) {
        let report = perft(self, depth);
        if depth == 1 {
            return (report.nodes_count, shakmaty_perft(self, 1));
        }
        for (m, count) in report.overview {
            let captured = self.do_move(m);
            let expected_count = shakmaty_perft(self, depth - 1);
            if count != expected_count {
                return self.backtrace_perft(depth - 1);
            }
            self.undo_move(m, captured);
        }
        (0, 0)
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
            duration: Duration::from_millis(1),
        }
    }

    fn nodes_per_second(&self) -> usize {
        let duration_as_secs = self.duration.as_secs_f64();
        (self.nodes_count as f64 / duration_as_secs).round() as usize
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
        assert_eq!(perft(&mut Board::default(), 0).nodes_count, 1);
    }

    #[test]
    fn depth_1() {
        assert_eq!(perft(&mut Board::default(), 1).nodes_count, 20);
    }

    #[test]
    fn depth_1_doesnt_alter_the_board() {
        let mut board = Board::default();
        perft(&mut board, 1);
        assert_eq!(board, Board::default());
    }

    #[test]
    fn depth_2() {
        assert_eq!(perft(&mut Board::default(), 2).nodes_count, 400);
    }

    #[test]
    fn depth_of_report_is_consistent_with_call() {
        assert_eq!(perft(&mut Board::default(), 1).depth, 1);
        assert_eq!(perft(&mut Board::default(), 2).depth, 2);
    }

    #[test]
    fn depth_3() {
        assert_eq!(perft(&mut Board::default(), 3).nodes_count, 8902);
    }

    #[test]
    fn depth_3_doesnt_alter_the_board() {
        let mut board = Board::default();
        perft(&mut board, 3);
        assert_eq!(board, Board::default());
    }

    #[test]
    fn depth_4() {
        assert_eq!(perft(&mut Board::default(), 4).nodes_count, 197_281);
    }

    //#[test]
    //fn depth_5() {
    //    assert_eq!(Board::default().perft(5).nodes_count, 4_865_609);
    //}
}

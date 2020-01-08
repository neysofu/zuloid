use super::*;
use crate::chess::Board;
use array_init::array_init;
use std::fmt;
use std::time::{Duration, Instant};
use std::vec;

pub fn perft_with_generator(
    board: &mut Board,
    generator: Move,
    depth: usize,
) -> usize {
    if depth == 0 {
        1
    } else {
        let mut stack = Stack::new(board.clone(), generator);
        let mut total = 0;
        while stack.depth > 0 {
            if let Some(mv) = stack.top_mut().unwrap().children.next() {
                debug_assert!(stack.depth <= depth);
                if stack.depth == depth {
                    total += stack.pop();
                } else {
                    stack.push(mv);
                }
            } else {
                stack.pop();
            }
        }
        total
    }
}

pub fn perft(board: &mut Board, depth: usize) -> Report {
    let start = Instant::now();
    let mut report = Report::new(depth);
    if depth == 0 {
        report.nodes_count = 1;
    } else {
        let mut legal_moves = AvailableMoves::default();
        board.list_legals(&mut legal_moves);
        for m in legal_moves.into_iter() {
            let result = perft_with_generator(board, m, depth - 1);
            report.overview.push((m, result));
            report.nodes_count += result;
        }
    }
    report.duration += start.elapsed();
    report
}

struct Stack {
    board: Board,
    levels: [Level; 20],
    depth: usize,
}

struct Level {
    generator: Move,
    capture: Option<Piece>,
    children: vec::IntoIter<Move>,
}

impl Stack {
    fn new(mut board: Board, generator: Move) -> Self {
        let capture = board.do_move(generator);
        let levels = array_init(|_| {
            let mut children = AvailableMoves::default();
            board.list_legals(&mut children);
            Level {
                generator,
                capture,
                children: children.into_iter(),
            }
        });
        Stack {
            board,
            levels,
            depth: 1,
        }
    }

    fn push(&mut self, mv: Move) {
        let capture = self.board.do_move(mv);
        let mut children = AvailableMoves::default();
        self.board.list_legals(&mut children);
        self.levels[self.depth] = Level {
            generator: mv,
            capture,
            children: children.into_iter(),
        };
        self.depth += 1;
    }

    fn pop(&mut self) -> usize {
        self.depth -= 1;
        self.board.undo_move(
            self.levels[self.depth].generator,
            self.levels[self.depth].capture,
        );
        self.levels[self.depth].children.clone().count()
    }

    fn top_mut(&mut self) -> Option<&mut Level> {
        if self.depth == 0 {
            None
        } else {
            Some(&mut self.levels[self.depth - 1])
        }
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

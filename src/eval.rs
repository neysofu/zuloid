use crate::chess::Board;

struct Eval {
    score: i32,
}

impl From<Board> for Eval {
    fn from(board: Board) -> Self {
        unimplemented!()
    }
}

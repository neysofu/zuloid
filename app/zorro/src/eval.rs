use zorro_chess::board::Board;

struct Eval {
    score: i32,
}

impl From<Board> for Eval {
    fn from(_board: Board) -> Self {
        unimplemented!()
    }
}

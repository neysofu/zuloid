use crate::chess::moves::AvailableMovesIter;
use crate::chess::*;
use crate::core::*;
use crate::eval::Eval;
use enum_map::{enum_map, EnumMap};

struct State<I: Iterator<Item = Move>> {
    tree: Vec<I>,
    depth: usize,
}

impl<'a> State<AvailableMovesIter<'a>> {
    fn new(board: &Board) -> Self {
        unimplemented!()
        //State {
        //    tree: vec![AvailableMoves::default().into_iter()],
        //    depth: 1,
        //}
    }

    fn next(&mut self) -> Option<Move> {
        unimplemented!()
    }
}

pub fn search(zorro: &mut Zorro) -> Eval {
    let role_values: EnumMap<Role, u32> = enum_map! {
        Role::Pawn => 1,
        Role::Knight => 3,
        Role::Bishop => 3,
        Role::Rook => 5,
        Role::Queen => 9,
        Role::King => 10000,
    };
    let mut tree = vec![AvailableMoves::default()];
    let mut moves = AvailableMoves::default();
    zorro.board.list_legals(&mut moves);
    let mut best = None;
    let mut val = 0;
    for m in moves.into_iter() {
        let capture = zorro.board.do_move(m);
        if best.is_none() {
            best = Some(m);
        }
        if let Some(piece) = capture {
            if role_values[piece.role] >= val {
                best = Some(m);
                val = role_values[piece.role];
            }
        }
        zorro.board.undo_move(m, capture);
    }
    let mut eval = Eval::default();
    eval.best_move = best.unwrap();
    eval
}

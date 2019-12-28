use crate::chess::*;
use crate::core::*;

pub fn search(zorro: &mut Zorro) -> Move {
    let mut moves = AvailableMoves::default();
    zorro.board.list_legals(&mut moves);
    let mut best = None;
    let mut role = Role::Pawn;
    for m in moves.into_iter() {
        let capture = zorro.board.do_move(m);
        if best.is_none() {
            best = Some(m);
        }
        if let Some(piece) = capture {
            if piece.role as u8 > role as u8 {
                best = Some(m);
                role = piece.role;
            }
        }
        zorro.board.undo_move(m, capture);
    }
    best.unwrap()
}

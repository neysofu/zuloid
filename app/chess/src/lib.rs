pub(crate) mod board;
pub(crate) mod color;
pub(crate) mod coordinates;
pub(crate) mod fen;
pub(crate) mod game;
pub(crate) mod magic;
pub(crate) mod movegen;
pub(crate) mod moving;
pub(crate) mod perft;
pub(crate) mod piece;

pub use board::Board;
pub use color::Color;
pub use coordinates::*;
pub use movegen::SlidingPiecesMoveGen;
pub use moving::Move;
pub use piece::{Piece, Role};

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    InvalidFen,
    InvalidColor,
    InvalidSquare,
}

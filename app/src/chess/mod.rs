pub mod board;
pub mod color;
pub mod coordinates;
pub mod fen;
pub mod game;
pub mod magic;
pub mod movegen;
pub mod moving;
pub mod perft;
pub mod piece;

pub use board::Board;
pub use color::Color;
pub use coordinates::*;
pub use movegen::SlidingPiecesMoveGen;
pub use moving::{Move, MoveList};
pub use piece::{Piece, Role};

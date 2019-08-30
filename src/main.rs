mod cache;
mod chess;
mod core;
mod eval;
mod protocols;
mod result;
mod time;
mod utils;
mod version;

use crate::core::Zorro;
use crate::protocols::uci::Uci;
use crate::protocols::Protocol;

fn main() {
    Uci::init(Zorro::default())
}

mod cache;
mod chess;
mod core;
mod err;
mod eval;
mod protocols;
mod time;
mod version;

use crate::core::Zorro;
use crate::protocols::{Protocol, Uci};
use std::io;

fn main() -> io::Result<()> {
    Uci::init(Zorro::default(), io::stdin().lock(), io::stdout())
}

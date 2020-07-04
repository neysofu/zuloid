#![allow(clippy::unreadable_literal)]

mod cache;
mod chess;
mod core;
mod elo;
mod err;
mod eval;
mod protocols;
mod search;
mod time;
mod utils;
mod version;

use crate::core::Zorro;
use std::io;

fn main() -> io::Result<()> {
    protocols::uci(&mut Zorro::default(), io::stdin().lock(), io::stdout())
}

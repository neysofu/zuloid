mod cache;
mod core;
mod eval;
mod protocols;
mod time;
mod version;

use crate::core::Zorro;
use crate::protocols::{uci::Uci, Protocol};
use std::io;

fn main() -> io::Result<()> {
    Uci::init(Zorro::default(), io::stdin().lock(), io::stdout())
}

mod cache;
mod chess;
mod core;
mod eval;
mod magics;
mod protocols;
mod result;
mod time;
mod utils;
mod version;

use crate::core::Zorro;
use crate::protocols::uci::Uci;
use crate::protocols::Protocol;
use std::process;

fn main() {
    /* A welcome message for human users. The number sign ensures minimal
     * possibility of accidental evaluation and it just happens to use the
     * same syntax as CECP's debug messages. */
    println!("# Zorro {}", version::VERSION);
    println!("# Process ID: {}", process::id());
    let zorro = Zorro::default();
    Uci::init(zorro)
}

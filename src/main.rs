mod chess;
mod core;
mod fen;
mod globals;
mod cache;
mod time;

use crate::core::Zorro;
use std::process;

fn main() {
    /* Now printing a welcome message for terminal users...
     * The number sign ensures minimal possibility of accidental evaluation and
     * it just happens to use the same syntax as CECP's debug messages. */
    println!("# Zorro {}", globals::VERSION);
    println!("# Process ID: {}", process::id());
    let zorro = Zorro::default();
    zorro.uci()
}

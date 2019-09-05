mod cache;
mod core;
mod eval;
mod protocols;
mod time;
mod utils;
mod version;

use crate::core::Zorro;
use crate::protocols::{uci::Uci, Protocol};

fn main() {
    Uci::init(Zorro::default())
}

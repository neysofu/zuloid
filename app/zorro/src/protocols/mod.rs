pub mod uci;

use crate::core::Zorro;
use std::io;

pub trait Protocol {
    fn init<R, W>(zorro: Zorro, input: R, output: W) -> io::Result<()>
    where
        R: io::BufRead,
        W: io::Write;
}

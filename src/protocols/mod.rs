pub mod uci;

pub use uci::Uci;

use crate::core::Zorro;

pub trait Protocol {
    fn init(zorro: Zorro);
}

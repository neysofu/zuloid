pub mod uci;

use crate::core::Zorro;

pub trait Protocol {
    fn init(zorro: Zorro);
}

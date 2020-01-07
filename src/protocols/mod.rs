//! Implementations of communication protocols between Zorro and compliant UIs.
//!
//! Only UCI is supported at the moment. CECP (XBoard) is currently not on the
//! roadmap.

mod uci;

pub use uci::{handle_line, uci};

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    InvalidFen,
    InvalidColor,
    InvalidSquare,
}

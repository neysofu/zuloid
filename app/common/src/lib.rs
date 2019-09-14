use std::fmt;

#[derive(Debug, PartialEq, Eq)]
pub enum Error {
    UnexpectedToken(String),
    UnexpectedEndOfCommand,
    UnknownCommand(String),
    InvalidFen,
    InvalidColor,
    InvalidSquare,
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::UnexpectedToken(s) => writeln!(f, "[ERROR] Expected token '{}'", s),
            Error::UnexpectedEndOfCommand => {
                writeln!(f, "[ERROR] End of command, a token was expected")
            }
            Error::UnknownCommand(s) => writeln!(f, "[ERROR] Unknown command '{}'", s),
            Error::InvalidFen => writeln!(f, "[ERROR] Invalid FEN string"),
            Error::InvalidColor => writeln!(f, "[ERROR] Invalid color string"),
            Error::InvalidSquare => writeln!(f, "[ERROR] Invalid square string"),
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

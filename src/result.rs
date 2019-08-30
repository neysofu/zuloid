use std::fmt;

#[derive(Debug)]
pub enum Error {
    UnexpectedToken(String),
    UnexpectedEndOfCommand,
    UnknownCommand(String),
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::UnexpectedToken(s) => writeln!(f, "[ERROR] Expected token '{}'", s),
            Error::UnexpectedEndOfCommand => {
                writeln!(f, "[ERROR] End of command, a token was expected")
            }
            Error::UnknownCommand(s) => writeln!(f, "[ERROR] Unknown command '{}'", s),
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

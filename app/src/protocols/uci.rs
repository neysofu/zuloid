use super::Protocol;
use crate::chess::{Board, Coordinate, Move, Square};
use crate::core::Zorro;
use crate::err::Error as ChessErr;
use crate::version::VERSION;
use bytesize::ByteSize;
use std::fmt;
use std::io;
use std::str::FromStr;

enum State {
    Alive,
    Shutdown,
}

pub struct Uci;

impl Protocol for Uci {
    fn init<R, W>(mut zorro: Zorro, input: R, mut output: W) -> io::Result<()>
    where
        R: io::BufRead,
        W: io::Write,
    {
        writeln!(output, "# Zorro {}", VERSION)?;
        writeln!(output, "# Process ID: {}", std::process::id())?;
        for line in input.lines() {
            match Uci::handle_line(&mut zorro, line?, &mut output) {
                Ok(State::Alive) => (),
                Ok(State::Shutdown) => return Ok(()),
                Err(err) => writeln!(output, "{}", err)?,
            }
        }
        Ok(())
    }
}

impl Uci {
    fn handle_line(
        zorro: &mut Zorro,
        line: impl AsRef<str>,
        mut output: impl io::Write,
    ) -> Result<State> {
        let mut tokens = line.as_ref().split_whitespace();
        match tokens.next() {
            // Standard UCI commands.
            Some("uci") => cmd::uci(output)?,
            Some("isready") => writeln!(output, "readyok")?,
            Some("setoption") => cmd::set_option(zorro, tokens)?,
            Some("ucinewgame") => zorro.cache.clear(),
            Some("position") => cmd::position(zorro, tokens)?,
            Some("quit") => return Ok(State::Shutdown),
            // Non-standard but useful nonetheless.
            Some("cleart") => writeln!(output, "{}[2J", 27 as char)?,
            Some("d") => cmd::d(zorro, tokens, output)?,
            Some("perft") => cmd::perft(zorro, tokens, output)?,
            Some("magic") => cmd::magic(tokens, output)?,
            Some("listmagics") => cmd::list_magics(output)?,
            Some("gentables") => cmd::gen_tables(output)?,
            Some(unknown) => return Err(Error::UnknownCommand(unknown.to_string())),
            None => (),
        }
        Ok(State::Alive)
    }
}

mod cmd {
    use super::*;

    pub fn uci(mut output: impl io::Write) -> Result<()> {
        writeln!(output, "id name Zorro {}", VERSION)?;
        writeln!(output, "id author Filippo Costa")?;
        writeln!(output, "option name Clear Hash type button")?;
        writeln!(
            output,
            "option name Contempt type spin default 20 min -100 max 100"
        )?;
        writeln!(
            output,
            "option name Hash type spin default 64 min 0 max 131072"
        )?;
        writeln!(
            output,
            "option name Minimum Thinking Time type spin default 20 min 0 max 5000"
        )?;
        writeln!(
            output,
            "option name nodestime type spin default 0 min 0 max 10000"
        )?;
        writeln!(output, "option name Ponder type check default false")?;
        writeln!(
            output,
            "option name Skill Level type spin default 20 min 0 max 20"
        )?;
        // See http://www.talkchess.com/forum3/viewtopic.php?start=0&t=4230?;
        writeln!(
            output,
            "option name Slow Mover type spin default 84 min 10 max 1000"
        )?;
        writeln!(
            output,
            "option name Threads type spin default 1 min 1 max 512"
        )?;
        writeln!(
            output,
            "option name Move Overhead type spin default 30 min 0 max 60000"
        )?;
        writeln!(output, "uciok")?;
        Ok(())
    }

    pub fn d<'s>(
        zorro: &Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        match tokens.next() {
            Some("lichess") => {
                let url = format!(
                    "https://lichess.org/analysis/standard/{}",
                    zorro.board.fmt_fen('_')
                );
                webbrowser::open(url.as_str()).ok();
            }
            Some(s) => return Err(Error::UnexpectedToken(s.to_string())),
            None => writeln!(output, "{}", zorro.board)?,
        }
        Ok(())
    }

    pub fn gen_tables<W: io::Write>(mut output: W) -> Result<()> {
        use crate::chess::tables;
        writeln!(output, "KING ATTACKS")?;
        for bb in (*tables::boxed_king_attacks()).iter() {
            writeln!(output, "0x{:016x}", bb)?;
        }
        writeln!(output, "KNIGHT ATTACKS")?;
        for bb in (*tables::boxed_knight_attacks()).iter() {
            writeln!(output, "0x{:016x}", bb)?;
        }
        Ok(())
    }

    pub fn list_magics(mut output: impl io::Write) -> Result<()> {
        use crate::chess::Magic;
        for magic in Magic::by_file().iter() {
            writeln!(output, "{}", magic)?;
        }
        Ok(())
    }

    pub fn magic<'s>(
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        use crate::chess::Magic;
        let square = Square::from_str(tokens.next().unwrap()).unwrap();
        let kind = tokens.next().unwrap();
        let mut bb = tokens.next().unwrap().parse().unwrap();
        match kind {
            "file" => {
                bb = (*Magic::by_file())[square.i()].magify(bb);
                writeln!(output, "0x{:x}", bb)?;
            }
            _ => {}
        };
        Ok(())
    }

    pub fn perft<'s>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        let depth = if let Some(s) = tokens.next() {
            match str::parse::<usize>(s) {
                Ok(depth) => depth,
                Err(_) => {
                    return Err(Error::UnexpectedToken(s.to_string()));
                }
            }
        } else {
            1
        };
        writeln!(output, "{}", zorro.board.perft(depth))?;
        Ok(())
    }

    pub fn position<'s>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
    ) -> Result<()> {
        match tokens.next() {
            Some("960") => unimplemented!(),
            Some("current") => (),
            Some("fen") => zorro.board = Board::from_fen(&mut tokens)?,
            Some("startpos") => zorro.board = Board::default(),
            Some(token) => return Err(Error::UnexpectedToken(token.to_string())),
            None => return Err(Error::UnexpectedEndOfCommand),
        }
        for token in tokens.skip_while(|s| *s == "moves") {
            match Move::from_str(token) {
                Ok(m) => {
                    zorro.board.do_move(m);
                }
                Err(err) => return Err(err.into()),
            }
        }
        Ok(())
    }

    pub fn set_option<'s>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
    ) -> Result<()> {
        assert_eq!(tokens.next(), Some("name"));
        let mut option_name = String::new();
        while let Some(token) = tokens.next() {
            if token == "value" {
                break;
            } else {
                option_name.push_str(token);
            }
        }
        let mut option_value = String::new();
        for token in tokens {
            // From the UCI protocol specification (April 2004):
            // > The name of the option should not be case sensitive and can inludes spaces
            // > like also the value.
            option_value.push_str(token.to_ascii_lowercase().as_str());
        }
        // Option support is quite hairy and messy. I don't want to break pre-existing
        // scripts and configs originally written for other engines.
        //
        // Please see:
        //  - https://komodochess.com/Komodo-11-README.html
        //  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
        //
        // No worries in case the links above die, just search for a list of UCI
        // settings for popular chess engines. I don't commit to 100% feature
        // parity with any engine; I just try and use my better judgement.
        match option_name.as_str() {
            "hash" => {
                let cache_size = ByteSize::mib(option_value.parse().unwrap());
                zorro.config.cache_size = cache_size;
            }
            "ponder" => {
                zorro.config.ponder = match option_value.chars().next() {
                    Some('f') => false,
                    Some('n') => false,
                    Some('0') => false,
                    _ => true,
                };
            }
            _ => (),
        };
        Ok(())
    }
}

#[derive(Debug)]
pub enum Error {
    UnexpectedToken(String),
    UnexpectedEndOfCommand,
    UnknownCommand(String),
    Chess(crate::err::Error),
    Io(io::Error),
}

impl From<io::Error> for Error {
    fn from(err: io::Error) -> Self {
        Error::Io(err)
    }
}

impl From<ChessErr> for Error {
    fn from(err: ChessErr) -> Self {
        Error::Chess(err)
    }
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::UnexpectedToken(s) => write!(f, "[ERROR] Unexpected token '{}'", s),
            Error::UnexpectedEndOfCommand => {
                write!(f, "[ERROR] End of command, a token was expected")
            }
            Error::UnknownCommand(s) => write!(f, "[ERROR] Unknown command '{}'", s),
            Error::Chess(ChessErr::InvalidFen) => write!(f, "[ERROR] Invalid FEN string"),
            Error::Chess(ChessErr::InvalidColor) => write!(f, "[ERROR] Invalid color string"),
            Error::Chess(ChessErr::InvalidSquare) => write!(f, "[ERROR] Invalid square string"),
            Error::Io(err) => write!(f, "[ERROR] Fatal I/O condition ({})", err),
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

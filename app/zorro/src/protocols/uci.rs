use super::Protocol;
use crate::core::Zorro;
use crate::version::VERSION;
use bytesize::ByteSize;
use std::fmt;
use std::io;
use std::str::FromStr;
use zorro_chess::Error as ChessErr;
use zorro_chess::{Board, Move};

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
        writeln!(&mut output, "# Zorro {}", VERSION)?;
        writeln!(&mut output, "# Process ID: {}", std::process::id())?;
        for line in input.lines() {
            match Uci::handle_line(&mut zorro, line?, &mut output) {
                Ok(State::Alive) => (),
                Ok(State::Shutdown) => return Ok(()),
                Err(err) => writeln!(&mut output, "{}", err)?,
            }
        }
        Ok(())
    }
}

impl Uci {
    fn handle_line<S, W>(zorro: &mut Zorro, line: S, mut output: W) -> Result<State>
    where
        S: AsRef<str>,
        W: io::Write,
    {
        let mut tokens = line.as_ref().split_whitespace();
        match tokens.next() {
            // Standard UCI commands.
            Some("isready") => writeln!(output, "readyok")?,
            Some("position") => CmdPosition::run(zorro, tokens, output)?,
            Some("quit") => return Ok(State::Shutdown),
            Some("setoption") => CmdSetOption::run(zorro, tokens, output)?,
            Some("uci") => print_uci_message(&mut output)?,
            Some("ucinewgame") => zorro.cache.clear(),
            // Non-standard but useful nonetheless.
            Some("cleart") => writeln!(output, "{}[2J", 27 as char)?,
            Some("d") => writeln!(output, "{}", zorro.board)?,
            Some("open") => CmdOpen::run(zorro, tokens, output)?,
            Some("perft") => CmdPerft::run(zorro, tokens, output)?,
            Some(unknown) => return Err(Error::UnknownCommand(unknown.to_string())),
            None => (),
        }
        Ok(State::Alive)
    }
}

trait Command {
    fn run<'s, W>(
        zorro: &mut Zorro,
        tokens: impl Iterator<Item = &'s str>,
        output: W,
    ) -> Result<()>
    where
        W: io::Write;
}

struct CmdOpen;
struct CmdPerft;
struct CmdPosition;
struct CmdSetOption;

impl Command for CmdOpen {
    fn run<'s, W>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        _output: W,
    ) -> Result<()>
    where
        W: io::Write,
    {
        match tokens.next() {
            Some("lichess") => {
                let url = format!(
                    "https://lichess.org/analysis/standard/{}",
                    zorro.board.fmt_fen('_')
                );
                webbrowser::open(url.as_str()).ok();
                Ok(())
            }
            Some(s) => Err(Error::UnexpectedToken(s.to_string())),
            None => Err(Error::UnexpectedEndOfCommand),
        }
    }
}

impl Command for CmdPerft {
    fn run<'s, W>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: W,
    ) -> Result<()>
    where
        W: io::Write,
    {
        let _depth = if let Some(s) = tokens.next() {
            match str::parse::<usize>(s) {
                Ok(depth) => depth,
                Err(_) => {
                    return Err(Error::UnexpectedToken(s.to_string()));
                }
            }
        } else {
            1
        };
        let mut buf = [Move::new_garbage(); 256];
        let legal_moves_count = zorro.board.list_legals(&mut buf[..], &zorro.magics).count();
        writeln!(&mut output, "N.{} legal moves:", legal_moves_count)?;
        for m in &buf[..legal_moves_count] {
            writeln!(&mut output, "{}", m)?;
        }
        Ok(())
    }
}

impl Command for CmdPosition {
    fn run<'s, W>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        output: W,
    ) -> Result<()>
    where
        W: io::Write,
    {
        match tokens.next() {
            Some("960") => unimplemented!(),
            Some("current") => (),
            Some("fen") => zorro.board = Board::from_fen(&mut tokens)?,
            Some("startpos") => zorro.board = Board::default(),
            Some(token) => return Err(Error::UnexpectedToken(token.to_string())),
            None => return Err(Error::UnexpectedEndOfCommand),
        }
        for token in tokens {
            match Move::from_str(token) {
                Ok(mv) => zorro.board.do_move(mv),
                Err(err) => return Err(err.into()),
            }
        }
        Ok(())
    }
}

impl Command for CmdSetOption {
    fn run<'s, W>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        _output: W,
    ) -> Result<()>
    where
        W: io::Write,
    {
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

fn print_uci_message<W>(output: &mut W) -> io::Result<()>
where
    W: io::Write,
{
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

#[derive(Debug)]
pub enum Error {
    UnexpectedToken(String),
    UnexpectedEndOfCommand,
    UnknownCommand(String),
    Chess(zorro_chess::Error),
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

use crate::chess::{Board, Coordinate, Move, Square};
use crate::core::Zorro;
use crate::err::Error as ChessErr;
use crate::version::VERSION;
use bytesize::ByteSize;
use std::fmt;
use std::io;
use std::str::FromStr;

#[derive(Debug, PartialEq)]
enum State {
    Alive,
    Shutdown,
}

pub fn uci(
    zorro: &mut Zorro,
    input: impl io::BufRead,
    mut output: impl io::Write,
) -> io::Result<()> {
    writeln!(output, "# Zorro {}", VERSION)?;
    writeln!(output, "# Process ID: {}", std::process::id())?;
    for line in input.lines() {
        match handle_line(zorro, line?, &mut output) {
            Ok(State::Alive) => (),
            Ok(State::Shutdown) => break,
            // Don't exit on error.
            Err(err) => writeln!(output, "{}", err)?,
        }
    }
    Ok(())
}

fn handle_line(
    zorro: &mut Zorro,
    line: impl AsRef<str>,
    mut output: impl io::Write,
) -> Result<State> {
    let mut tokens = line.as_ref().split_whitespace();
    match tokens.next() {
        // Standard UCI commands.
        Some("debug") => (),
        Some("go") => cmd::go(zorro, tokens, output)?,
        Some("isready") => writeln!(output, "readyok")?,
        Some("position") => cmd::position(zorro, tokens)?,
        Some("quit") | Some("stop") => return Ok(State::Shutdown),
        Some("setoption") => cmd::set_option(zorro, tokens)?,
        Some("uci") => cmd::uci(output)?,
        Some("ucinewgame") => zorro.cache.clear(),
        // Non-standard but useful nonetheless.
        Some("cleart") => writeln!(output, "{}[2J", 27 as char)?,
        Some("d") => cmd::d(zorro, tokens, output)?,
        Some("gentables") => cmd::gen_tables(output)?,
        Some("listmagics") => cmd::list_magics(output)?,
        Some("magic") => cmd::magic(tokens, output)?,
        Some("perft") => cmd::perft(zorro, tokens, output)?,
        Some(unknown) => return Err(Error::UnknownCommand(unknown.to_string())),
        // Simply ignore empty commands.
        None => (),
    }
    Ok(State::Alive)
}

mod cmd {
    use super::*;

    pub fn uci(mut output: impl io::Write) -> Result<()> {
        // See http://www.talkchess.com/forum3/viewtopic.php?start=0&t=4230?;
        writeln!(
            output,
            "id name Zorro {}\n\
             id author Filippo Costa\n\
             option name Clear Hash type button\n\
             option name Contempt type spin default 20 min -100 max 100\n\
             option name Hash type spin default 64 min 0 max 131072\n\
             option name Minimum Thinking Time type spin default 20 min 0 max 5000\n\
             option name nodestime type spin default 0 min 0 max 10000\n\
             option name Skill Level type spin default 20 min 0 max 20\n\
             option name Slow Mover type spin default 84 min 10 max 1000\n\
             option name Threads type spin default 1 min 1 max 512\n\
             option name Move Overhead type spin default 30 min 0 max 60000\n\
             uciok",
            VERSION
        )?;
        Ok(())
    }

    pub fn d<'s>(
        zorro: &Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        match tokens.next() {
            Some("lichess") => {
                webbrowser::open(zorro.board.lichess_url().as_str()).ok();
            }
            Some("fen") => writeln!(output, "{}", zorro.board.fmt_fen(' '))?,
            Some(_) => return Err(Error::Syntax),
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

    pub fn go<'s>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        let mut config = zorro.config.clone();
        while let Some(token) = tokens.next() {
            let next = tokens.next().ok_or(Error::Syntax);
            match token {
                "wtime" => (),
                "btime" => (),
                "winc" => (),
                "binc" => (),
                "movestogo" => config.moves_to_go = Some(str::parse(next?)?),
                "depth" => config.max_depth = Some(str::parse(next?)?),
                "nodes" => config.max_nodes = Some(str::parse(next?)?),
                "infinite" => (),
                "ponder" => config.ponder = true,
                "perft" => return perft(zorro, tokens, output),
                _ => return Err(Error::Syntax),
            }
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
        use crate::chess::tables;
        use crate::chess::Magic;
        let square = Square::from_str(tokens.next().unwrap()).unwrap();
        let kind = tokens.next().unwrap();
        let mut bb = tokens.next().unwrap_or("0").parse().unwrap();
        match kind {
            "file" => {
                bb = (*Magic::by_file())[square.i()].magify(bb);
                writeln!(output, "0x{:x}", bb)?;
            }
            "knight" => {
                bb = tables::KNIGHT_ATTACKS[square.i()];
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
        let token = tokens.next().unwrap_or("1");
        let depth = str::parse::<usize>(token)?;
        writeln!(output, "{}", zorro.board.perft(depth))?;
        if let Some("bt") = tokens.next() {
            let (actual, expected) = zorro.board.backtrace_perft(depth);
            if actual == expected {
                writeln!(output, "Backtrace FEN   : none")?;
            } else {
                writeln!(output, "Backtrace FEN   : {}", zorro.board.fmt_fen(' '))?;
            }
            writeln!(output, "Final searched (expected) : {}", expected)?;
            writeln!(output, "Final searched (actual)   : {}", actual)?;
        }
        Ok(())
    }

    pub fn position<'s>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'s str>,
    ) -> Result<()> {
        match tokens.next() {
            Some("startpos") => zorro.board = Board::default(),
            Some("fen") => zorro.board = Board::from_fen(&mut tokens)?,
            Some("960") => unimplemented!(),
            Some("current") => (),
            _ => return Err(Error::Syntax),
        }
        for s in tokens.skip(1) {
            zorro.board.do_move(Move::from_str(s)?);
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
    Syntax,
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

impl From<std::num::ParseIntError> for Error {
    fn from(_err: std::num::ParseIntError) -> Self {
        Error::Syntax
    }
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::Syntax => write!(f, "[ERROR] Invalid command syntax"),
            Error::UnknownCommand(s) => write!(f, "[ERROR] Unknown command '{}'", s),
            Error::Chess(ChessErr::InvalidFen) => write!(f, "[ERROR] Invalid FEN string"),
            Error::Chess(ChessErr::InvalidColor) => write!(f, "[ERROR] Invalid color string"),
            Error::Chess(ChessErr::InvalidSquare) => write!(f, "[ERROR] Invalid square string"),
            Error::Io(err) => write!(f, "[ERROR] Fatal I/O condition ({})", err),
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn stop_cmd_triggers_shutdown() {
        match handle_line(&mut Zorro::default(), "stop", io::sink()) {
            Ok(State::Shutdown) => true,
            _ => false,
        };
    }

    #[test]
    fn quit_cmd_triggers_shutdown() {
        match handle_line(&mut Zorro::default(), "quit", io::sink()) {
            Ok(State::Shutdown) => true,
            _ => false,
        };
    }

    #[test]
    fn readyok_always_follows_isready() {
        let zorro = &mut Zorro::default();
        let mut output = vec![];
        assert!(handle_line(zorro, "isready", &mut output).is_ok());
        output.retain(|c| !(*c as char).is_whitespace());
        assert_eq!(output, b"readyok");
    }
}

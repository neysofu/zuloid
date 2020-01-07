//! Universal Chess Interface implementation for Zorro.
//! Documentation is available at
//! <https://wbec-ridderkerk.nl/html/UCIProtocol.html>.
//!
//! We aim for full compliance. Stockfish-like debugging features are also
//! present.

use crate::chess::{Board, Color, Coordinate, Move, Square};
use crate::core::Zorro;
use crate::err::Error as ChessErr;
use crate::search::*;
use crate::time::TimeControl;
use crate::version::VERSION;
use std::fmt;
use std::io;
use std::str::FromStr;
use std::time::Duration;
use strum::IntoEnumIterator;

#[derive(Debug, PartialEq)]
pub enum State {
    Alive,
    Shutdown,
}

pub fn uci(
    zorro: &mut Zorro,
    input: impl io::BufRead,
    mut output: impl io::Write,
) -> io::Result<()> {
    // Greet the user with some information about the engine.
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
    // Ave, Caesar, moriturus te salutat.
    Ok(())
}

pub fn handle_line(
    zorro: &mut Zorro,
    line: impl AsRef<str>,
    mut output: impl io::Write,
) -> Result<State> {
    let mut tokens = line.as_ref().split_whitespace();
    match tokens.next() {
        // Standard UCI commands.
        Some("debug") => cmd::debug(zorro, tokens)?,
        Some("go") => cmd::go(zorro, tokens, output)?,
        Some("isready") => writeln!(output, "readyok")?,
        Some("position") => cmd::position(zorro, tokens)?,
        Some("quit") | Some("stop") => return Ok(State::Shutdown),
        Some("setoption") => cmd::set_option(zorro, tokens)?,
        Some("uci") => cmd::uci(output)?,
        Some("ucinewgame") => zorro.cache.clear(),
        // Non-standard but useful nonetheless.
        Some("cleart") => writeln!(output, "{}[2J", 27 as char)?,
        Some("d") => cmd::d(&zorro.board, tokens, output)?,
        Some("gentables") => cmd::gen_tables(output)?,
        Some("listmagics") => cmd::list_magics(output)?,
        Some("magic") => cmd::magic(tokens, output)?,
        Some("perft") => cmd::perft(zorro, tokens, output)?,
        // Easter eggs.
        Some("detroit") => writeln!(output, "JESUS CHRIST CONNOR")?,
        Some("kara") => writeln!(output, "I'm cold")?,
        Some("meow") => writeln!(output, "Wowwwww, you meow like a cat! That means you are one, right? Shut the fuck up. If you really want to be put on a leash and treated like a domestic animal then that’s called a fetish, not “quirky” or “cute”. What part of you seriously thinks that any part of acting like a feline establishes a reputation of appreciation? Is it your lack of any defining aspect of personality that urges you to resort to shitty representations of cats to create an illusion of meaning in your worthless life? Wearing “cat ears” in the shape of headbands further notes the complete absence of human attribution to your false sense of personality, such as intelligence or charisma in any form or shape. Where do you think this mindset’s gonna lead you? You think you’re funny, random, quirky even? What makes you think that acting like a fucking cat will make a goddamn hyena laugh? I, personally, feel extremely sympathetic towards you as your only escape from the worthless thing you call your existence is to pretend to be an animal. But it’s not a worthy choice to assert this horrifying fact as a dominant trait, mainly because personality traits require an initial personality to lay their foundation on. You’re not worthy of anybody’s time, so go fuck off, “cat-girl”.")?,
        Some(s) => return Err(Error::UnknownCommand(s.to_string())),
        // Skip empty lines.
        None => (),
    }
    Ok(State::Alive)
}

/// UCI commands handlers.
mod cmd {
    use super::*;

    pub fn debug<'a>(
        zorro: &mut Zorro,
        mut tokens: impl Iterator<Item = &'a str>,
    ) -> Result<()> {
        zorro.config.debug = match tokens.next() {
            Some("on") => true,
            Some("off") => false,
            _ => return Err(Error::Syntax),
        };
        Ok(())
    }

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
            VERSION,
        )?;
        Ok(())
    }

    pub fn d<'s>(
        board: &Board,
        mut tokens: impl Iterator<Item = &'s str>,
        mut output: impl io::Write,
    ) -> Result<()> {
        match tokens.next() {
            None => write!(output, "{}", board)?,
            Some("fen") => writeln!(output, "{}", board.fmt_fen(' '))?,
            Some("lichess") => {
                let url = board.lichess_url();
                writeln!(output, "{}", url)?;
                webbrowser::open(url.as_str()).map_err(|_| Error::Other)?;
            }
            _ => return Err(Error::Syntax),
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
            let mut next = || tokens.next().ok_or(Error::Syntax);
            match token {
                "searchmoves" => {
                    // A for loop will cause ownership issues. FIXME?
                    while let Some(s) = tokens.next() {
                        config.restrict_search.push(Move::from_str(s)?);
                    }
                }
                "wtime" | "btime" | "winc" | "binc" => {
                    let color = Color::from_str(token)?;
                    let time_control = &mut zorro.time_controls[color];
                    let dur = Duration::from_millis(str::parse(next()?)?);
                    if &token[1..] == "time" {
                        time_control.time_limit = dur;
                    } else {
                        time_control.increment = dur;
                    }
                }
                "movestogo" => config.moves_to_go = Some(str::parse(next()?)?),
                "depth" => config.max_depth = Some(str::parse(next()?)?),
                "nodes" => config.max_nodes = Some(str::parse(next()?)?),
                "mate" => (),
                "movetime" => {
                    let color = zorro.board.color_to_move;
                    let time_control = &mut zorro.time_controls[color];
                    let dur = Duration::from_millis(str::parse(next()?)?);
                    time_control.time_limit = dur;
                    time_control.delay = Duration::default();
                }
                "infinite" => {
                    for c in Color::iter() {
                        zorro.time_controls[c] = TimeControl::infinite();
                    }
                }
                "ponder" => config.ponder = true,
                "perft" => return perft(zorro, tokens, output),
                _ => return Err(Error::Syntax),
            }
        }
        writeln!(output, "bestmove {}", iter_search(zorro).best_move)?;
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
        write!(
            output,
            "{}",
            crate::chess::perft::perft(&mut zorro.board, depth)
        )?;
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
        // '.skip(1)' intuitively makes more sense, but that would happily skip
        // a valid move in case you forget the "moves" token.
        for s in tokens.skip_while(|s| *s == "moves") {
            zorro.board.do_move(Move::from_str(s)?);
        }
        Ok(())
    }

    pub fn set_option<'s>(
        _zorro: &mut Zorro,
        _tokens: impl Iterator<Item = &'s str>,
    ) -> Result<()> {
        //assert_eq!(tokens.next(), Some("name"));
        //let mut option_name = String::new();
        //while let Some(token) = tokens.next() {
        //    if token == "value" {
        //        break;
        //    } else {
        //        option_name.push_str(token);
        //    }
        //}
        //let mut option_value = String::new();
        //for token in tokens {
        //    // From the UCI protocol specification (April 2004):
        //    // > The name of the option should not be case sensitive and can
        // inludes spaces    // > like also the value.
        //    option_value.push_str(token.to_ascii_lowercase().as_str());
        //}
        //// Option support is quite hairy and messy. I don't want to break
        //// pre-existing scripts and configs originally written for
        //// other engines.
        ////
        //// Please see:
        ////  - https://komodochess.com/Komodo-11-README.html
        ////  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
        ////
        //// No worries in case the links above die, just search for a list of
        //// UCI settings for popular chess engines. I don't commit to
        //// 100% feature parity with any engine; I just try and use my
        //// better judgement.
        //match option_name.as_str() {
        //    "hash" => {
        //        let cache_size =
        // ByteSize::mib(option_value.parse().unwrap());        zorro.
        // config.cache_size = cache_size;    }
        //    "ponder" => {
        //        zorro.config.ponder = match option_value.chars().next() {
        //            Some('f') => false,
        //            Some('n') => false,
        //            Some('0') => false,
        //            _ => true,
        //        };
        //    }
        //    _ => (),
        //};
        Ok(())
    }
}

#[derive(Debug)]
pub enum Error {
    Syntax,
    UnknownCommand(String),
    Chess(crate::err::Error),
    Io(io::Error),
    Other,
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
            Error::Other => {
                write!(f, "[ERROR] An unspecified error has happened")
            }
            Error::UnknownCommand(s) => {
                write!(f, "[ERROR] Unknown command '{}'", s)
            }
            Error::Chess(ChessErr::InvalidFen) => {
                write!(f, "[ERROR] Invalid FEN string")
            }
            Error::Chess(ChessErr::InvalidColor) => {
                write!(f, "[ERROR] Invalid color string")
            }
            Error::Chess(ChessErr::InvalidSquare) => {
                write!(f, "[ERROR] Invalid square string")
            }
            Error::Io(err) => {
                write!(f, "[ERROR] Fatal I/O condition ({})", err)
            }
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

#[cfg(test)]
mod test {
    use super::*;
    use crate::utils::buf_to_str;

    #[test]
    fn d_fen_prints_fen() {
        let zorro = &mut Zorro::default();
        let mut output = vec![];
        handle_line(zorro, "uci", io::sink()).unwrap();
        handle_line(zorro, "position current moves b1c3", io::sink()).unwrap();
        let fen = zorro.board.fmt_fen(' ').to_string();
        handle_line(zorro, "d fen", &mut output).unwrap();
        assert_eq!(buf_to_str(&mut output), fen);
    }

    #[test]
    fn stop_cmd_triggers_shutdown() {
        assert_eq!(
            handle_line(&mut Zorro::default(), "stop", io::sink()).unwrap(),
            State::Shutdown
        );
    }

    #[test]
    fn quit_cmd_triggers_shutdown() {
        assert_eq!(
            handle_line(&mut Zorro::default(), "quit", io::sink()).unwrap(),
            State::Shutdown
        );
    }

    #[test]
    fn readyok_always_follows_isready() {
        let zorro = &mut Zorro::default();
        let mut output = vec![];
        handle_line(zorro, "uci", io::sink()).unwrap();
        handle_line(zorro, "isready", &mut output).unwrap();
        assert_eq!(buf_to_str(&mut output), "readyok");
    }
}

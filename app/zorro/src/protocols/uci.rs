use super::Protocol;
use crate::core::Zorro;
use crate::version::VERSION;
use bytesize::ByteSize;
use std::io::{self, BufRead};
use std::str::FromStr;
use zorro_chess::{Board, Move};
use zorro_common::{Error, Result};

enum State {
    Alive,
    Shutdown,
}

pub struct Uci;

impl Uci {
    fn handle_line<S: AsRef<str>>(zorro: &mut Zorro, line: S) -> Result<State> {
        let mut tokens = line.as_ref().split_whitespace();
        match tokens.next() {
            // Standard UCI commands.
            Some("isready") => println!("readyok"),
            Some("position") => CmdPosition::run(zorro, tokens)?,
            Some("quit") => return Ok(State::Shutdown),
            Some("setoption") => CmdSetOption::run(zorro, tokens)?,
            Some("uci") => print_uci_message(),
            Some("ucinewgame") => zorro.cache.clear(),
            // Non-standard but useful nonetheless.
            Some("cleart") => println!("{}[2J", 27 as char),
            Some("d") => println!("{}", zorro.board),
            Some("open") => CmdOpen::run(zorro, tokens)?,
            Some("perft") => CmdPerft::run(zorro, tokens)?,
            Some(unknown) => return Err(Error::UnknownCommand(unknown.to_string())),
            None => (),
        }
        Ok(State::Alive)
    }
}

impl Protocol for Uci {
    fn init(mut zorro: Zorro) {
        println!("# Zorro {}", VERSION);
        println!("# Process ID: {}", std::process::id());
        for line in io::stdin().lock().lines() {
            if let Err(e) = Uci::handle_line(&mut zorro, line.unwrap()) {
                print_err(e)
            }
        }
    }
}

trait Command {
    fn run<'s>(zorro: &mut Zorro, tokens: impl Iterator<Item = &'s str>) -> Result<()>;
}

struct CmdOpen;
struct CmdPerft;
struct CmdPosition;
struct CmdSetOption;

impl Command for CmdOpen {
    fn run<'s>(zorro: &mut Zorro, mut tokens: impl Iterator<Item = &'s str>) -> Result<()> {
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
    fn run<'s>(zorro: &mut Zorro, mut tokens: impl Iterator<Item = &'s str>) -> Result<()> {
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
        for mv in zorro.board.list_legals(&mut buf[..], &zorro.magics) {
            println!("{}", mv);
        }
        Ok(())
    }
}

impl Command for CmdPosition {
    fn run<'s>(zorro: &mut Zorro, mut tokens: impl Iterator<Item = &'s str>) -> Result<()> {
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
                Err(e) => print_err(e),
            }
        }
        Ok(())
    }
}

impl Command for CmdSetOption {
    fn run<'s>(zorro: &mut Zorro, mut tokens: impl Iterator<Item = &'s str>) -> Result<()> {
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

fn print_uci_message() {
    println!("id name Zorro {}", VERSION);
    println!("id author Filippo Costa");
    println!("option name Clear Hash type button");
    println!("option name Contempt type spin default 20 min -100 max 100");
    println!("option name Hash type spin default 64 min 0 max 131072");
    println!("option name Minimum Thinking Time type spin default 20 min 0 max 5000");
    println!("option name nodestime type spin default 0 min 0 max 10000");
    println!("option name Ponder type check default false");
    println!("option name Skill Level type spin default 20 min 0 max 20");
    // See http://www.talkchess.com/forum3/viewtopic.php?start=0&t=42308
    println!("option name Slow Mover type spin default 84 min 10 max 1000");
    println!("option name Threads type spin default 1 min 1 max 512");
    println!("option name Move Overhead type spin default 30 min 0 max 60000");
    println!("uciok");
}

fn print_err(err: Error) {
    print!("{}", err)
}

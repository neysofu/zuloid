use crate::cache::Cache;
use crate::chess::*;
use crate::globals::VERSION;
use crate::time::TimeControl;
use bytesize::ByteSize;
use enum_map::EnumMap;
use std::io::{self, BufRead};
use std::str::FromStr;

#[derive(Default)]
pub struct Zorro {
    config: Config,
    cache: Cache,
    board: Board,
    history: Vec<Board>,
    time_controls: EnumMap<Color, TimeControl>,
}

impl Zorro {
    pub fn uci(mut self) {
        let stdin = io::stdin();
        for line in stdin.lock().lines() {
            let line = line.unwrap();
            let mut tokens = line.split_whitespace();
            match tokens.next() {
                Some("cleart") => clear_screen(),
                Some("d") => println!("{}", self.board),
                Some("go") => self.search(),
                Some("isready") => println!("readyok"),
                Some("position") => self.uci_position(tokens),
                Some("quit") => break,
                Some("setoption") => self.uci_set_option(tokens),
                Some("uci") => print_uci_message(),
                Some("ucinewgame") => self.cache.clear(),
                Some(unknown) => self.warn_unknown_command(unknown),
                None => (),
            }
        }
    }

    fn warn_unknown_command<S: AsRef<str>>(&self, cmd_name: S) {
        println!("[ERROR] Unknown command '{}'", cmd_name.as_ref());
    }

    fn warn_expected_token(&self) {
        println!("[ERROR] End of command, a token was expected");
    }

    fn warn_unexpected_token<S: AsRef<str>>(&self, token: S) {
        println!("[ERROR] Unexpected token '{}'", token.as_ref());
    }

    fn uci_position<'s>(&mut self, mut tokens: impl Iterator<Item = &'s str>) {
        match tokens.next() {
            Some("960") => unimplemented!(),
            Some("current") => (),
            Some("fen") => self.board = Board::from_fen(&mut tokens),
            Some("startpos") => self.board = Board::default(),
            Some(token) => self.warn_unexpected_token(token),
            None => self.warn_expected_token(),
        }
        for token in tokens {
            match Move::from_str(token) {
                Ok(mv) => self.board.do_move(&mv),
                Err(_) => (),
            }
        }
    }

    fn uci_set_option<'s>(&mut self, mut tokens: impl Iterator<Item = &'s str>) {
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
        // Option support is quite hairy and messy. I don't want to break pre-existing scripts
        // and configs originally written for other engines.
        //
        // Please see:
        //  - https://komodochess.com/Komodo-11-README.html
        //  - http://www.rybkachess.com/index.php?auswahl=Engine+parameters
        //
        // No worries in case the links above die, just search for a list of UCI settings for
        // popular chess engines. I don't commit to 100% feature parity with any engine; I just
        // try and use my better judgement.
        match option_name.as_str() {
            "hash" => {
                let cache_size = ByteSize::mib(option_value.parse().unwrap());
                self.config.cache_size = cache_size;
            }
            "ponder" => {
                self.config.ponder = match option_value.chars().next() {
                    Some('f') => false,
                    Some('n') => false,
                    Some('0') => false,
                    _ => true,
                };
            }
            _ => (),
        }
    }

    fn search(&mut self) {}
}

pub struct Config {
    cache_size: ByteSize,
    contempt: f32,
    debug: bool,
    max_depth: Option<usize>,
    max_nodes: Option<usize>,
    ponder: bool,
    selectivity: f32,
}

impl Default for Config {
    fn default() -> Self {
        Config {
            cache_size: ByteSize::mb(8),
            contempt: 0.5,
            debug: false,
            max_depth: None,
            max_nodes: None,
            ponder: false,
            selectivity: 0.5,
        }
    }
}

fn print_uci_message() {
    println!("id name Zorro {}", VERSION);
    println!("id author Filippo Costa");
    println!("option name Clear Hash type button");
    // TODO: also implement Komodo's Drawscore option.
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

fn clear_screen() {
    println!("{}[2J", 27 as char);
}

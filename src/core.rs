use crate::cache::Cache;
use crate::chess::*;
use crate::globals::VERSION;
use crate::time::TimeControl;
use enum_map::EnumMap;
use std::io::{self, BufRead};

#[derive(Default)]
pub struct Zorro {
    config: Config,
    cache: Cache,
    board: Board,
    time_controls: EnumMap<Color, TimeControl>,
}

impl Zorro {
    pub fn uci(mut self) {
        let stdin = io::stdin();
        for line in stdin.lock().lines() {
            let line = line.unwrap();
            let mut tokens = line.split_whitespace();
            match tokens.next() {
                Some("isready") => println!("readyok"),
                Some("quit") => {
                    break;
                }
                Some("uci") => print_uci_message(),
                Some("ucinewgame") => {
                    self.cache.clear();
                }
                Some(unknown) => println!("Unknown command: {}", unknown),
                None => (),
            }
        }
    }
}

pub struct Config {
    debug: bool,
    contempt: f32,
    selectivity: f32,
    max_depth: Option<usize>,
    max_nodes: Option<usize>,
}

impl Default for Config {
    fn default() -> Self {
        Config {
            debug: false,
            contempt: 0.5,
            selectivity: 0.5,
            max_depth: None,
            max_nodes: None,
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

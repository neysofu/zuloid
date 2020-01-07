use crate::cache::Cache;
use crate::chess::{Board, Color, Move};
use crate::time::TimeControl;
use bytesize::ByteSize;
use enum_map::EnumMap;

#[derive(Clone, Default)]
pub struct Zorro {
    pub config: Config,
    pub cache: Cache,
    pub board: Board,
    pub board_buffer: Board,
    pub time_controls: EnumMap<Color, TimeControl>,
}

#[derive(Clone)]
pub struct Config {
    pub cache_size: ByteSize,
    pub restrict_search: Vec<Move>,
    pub contempt: f32,
    pub debug: bool,
    pub max_depth: Option<usize>,
    pub max_nodes: Option<usize>,
    pub moves_to_go: Option<usize>,
    pub ponder: bool,
    pub selectivity: f32,
}

impl Default for Config {
    fn default() -> Self {
        Config {
            cache_size: ByteSize::mb(32),
            contempt: 0.5,
            debug: false,
            restrict_search: Vec::with_capacity(0),
            moves_to_go: None,
            max_depth: None,
            max_nodes: None,
            ponder: false,
            selectivity: 0.5,
        }
    }
}

use crate::cache::Cache;
use crate::chess::{Board, Color};
use crate::time::TimeControl;
use bytesize::ByteSize;
use enum_map::EnumMap;

#[derive(Default)]
pub struct Zorro {
    pub config: Config,
    pub cache: Cache,
    pub board: Board,
    //pub move_logic: MoveLogic,
    pub time_controls: EnumMap<Color, TimeControl>,
}

pub struct Config {
    pub cache_size: ByteSize,
    pub contempt: f32,
    pub debug: bool,
    pub max_depth: Option<usize>,
    pub max_nodes: Option<usize>,
    pub ponder: bool,
    pub selectivity: f32,
}

impl Default for Config {
    fn default() -> Self {
        Config {
            cache_size: ByteSize::mb(32),
            contempt: 0.5,
            debug: false,
            max_depth: None,
            max_nodes: None,
            ponder: false,
            selectivity: 0.5,
        }
    }
}

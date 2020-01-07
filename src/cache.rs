use crate::chess::board::Board;
use crate::eval::ConciseEval;
use bytesize::ByteSize;
use std::collections::HashMap;

#[derive(Clone)]
pub struct Cache {
    hashmap: HashMap<Board, ConciseEval>,
}

impl Cache {
    pub fn new_with_capacity(size: ByteSize) -> Self {
        Cache {
            hashmap: HashMap::with_capacity(size.as_u64() as usize),
        }
    }

    pub fn clear(&mut self) {
        self.hashmap.clear();
    }
}

impl Default for Cache {
    fn default() -> Self {
        Cache::new_with_capacity(ByteSize::mib(32))
    }
}

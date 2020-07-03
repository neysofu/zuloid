use crate::chess::board::Board;
use crate::eval::Eval;
use lru::LruCache;
use bytesize::ByteSize;
use std::collections::HashMap;

pub struct Cache {
    hashmap: HashMap<Board, i32>,
    lru: LruCache<Board, i32>,
}

impl Cache {
    pub fn new_with_capacity(size: ByteSize) -> Self {
        Cache {
            hashmap: HashMap::with_capacity(size.as_u64() as usize),
            lru: LruCache::new(8192),
        }
    }

    pub fn clear(&mut self) {
        self.hashmap.clear();
        self.lru.clear();
    }

    pub fn hit(&mut self, board: &Board) -> Option<i32> {
        let opt_result = self.lru.get(board);
        match opt_result {
            Some(r) => Some(*r),
            None => {
                self.lru.put(board.clone(), Eval::new(board).score);
                None
            }
        }
    }
}

impl Default for Cache {
    fn default() -> Self {
        Cache::new_with_capacity(ByteSize::mib(32))
    }
}

use crate::chess::board::Board;
use crate::eval::Eval;
use bytesize::ByteSize;
use lru::LruCache;

struct Node {
    score: i32,
    quiescence: u8,
}

pub struct Cache {
    lru: LruCache<Board, Node>,
}

impl Cache {
    pub fn new_with_capacity(size: ByteSize) -> Self {
        Cache {
            lru: LruCache::new(8192),
        }
    }

    pub fn clear(&mut self) {
        self.lru.clear();
    }

    pub fn hit(&mut self, board: &Board) -> Option<i32> {
        let opt_result = self.lru.get(board);
        match opt_result {
            Some(r) => Some((*r).score),
            None => {
                let node = Node {
                    score: Eval::new(board).score,
                    quiescence: 2,
                };
                self.lru.put(board.clone(), node);
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

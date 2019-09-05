use std::collections::HashMap;
use zorro_chess::board::Board;

struct Evaluation;

#[derive(Default)]
pub struct Cache {
    hashmap: HashMap<Board, Evaluation>,
}

impl Cache {
    pub fn clear(&mut self) {
        self.hashmap.clear();
    }
}

use chess::{ALL_SQUARES, BitBoard, File, Rank, Square};

fn rnd_sparse_u64() -> u64 {
    rand::random::<u64>() & rand::random::<u64>() & rand::random::<u64>()
}

struct Hasher {
    premask: u64,
    magic: u64,
    shift: u8,
    postmask: u64,
}

struct U64SubsetIter {
    current: u64,
    val: u64,
}

impl U64SubsetIter {
    fn new(val: u64) -> Self {
        U64SubsetIter {
            current: 0,
            val
        }
    }
}

impl Iterator for U64SubsetIter {
    type Item = u64;

    fn next(&mut self) -> Option<Self::Item> {
        self.current = (self.current - self.val) & self.val;
        if self.current > 0 {
            Some(self.current)
        } else {
            None
        }
    }
}

fn main() {
    for square in ALL_SQUARES.iter() {
        let hasher = Hasher {
            premask: 0,
            magic: rnd_sparse_u64(),
            shift: 0,
            postmask: 0,
        };
        let obstacles = BitBoard::new(hasher.premask);
    }
    println!("Hello, world!");
}

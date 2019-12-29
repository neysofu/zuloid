extern crate zorro;

use std::io;
use zorro::core::*;
use zorro::protocols::*;
use zorro::utils::buf_to_str;

const TACTICS: &[(&str, &str)] = &[
    (
        "r1bqkbnr/ppp2ppp/2np4/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 0 4",
        "f3f7",
    ),
    (
        "rnbqkbnr/pppp1ppp/8/4p3/6P1/5P2/PPPPP2P/RNBQKBNR b KQkq - 0 2",
        "d8h4",
    ),
];

#[test]
fn tactics() {
    let zorro = &mut Zorro::default();
    for (fen, best_move) in TACTICS.iter() {
        let mut buffer = vec![];
        handle_line(zorro, format!("position fen {}", fen), io::sink())
            .unwrap();
        handle_line(zorro, "go wtime 1 btime 1", &mut buffer).unwrap();
        assert_eq!(buf_to_str(&mut buffer), format!("bestmove {}", best_move));
    }
}

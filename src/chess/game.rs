use super::*;
use enum_map::EnumMap;

pub struct GameResult {
    score: EnumMap<Color, Score>,
    kind: GameResultKind,
}

pub enum Score {
    One,
    Half,
    Zero,
}

/// What was the result of this game?
#[derive(Copy, Clone, PartialEq, PartialOrd, Eq, Ord, Debug, Hash)]
pub enum GameResultKind {
    Checkmate,
    Resign,
    Stalemate,
    DrawAccepted,
    DrawDeclared,
}

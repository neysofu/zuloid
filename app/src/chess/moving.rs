use super::{Coordinate, Role, Square};
use crate::err::Error;
use std::fmt;
use std::iter::IntoIterator;
use std::str::FromStr;

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Move {
    pub from: Square,
    pub to: Square,
    pub promotion: Option<Role>,
}

impl Move {
    pub fn new_garbage() -> Self {
        Move {
            from: Square::new(0),
            to: Square::new(0),
            promotion: None,
        }
    }
}

impl FromStr for Move {
    type Err = Error;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        assert!(s.len() >= 4);
        let from = Square::from_str(&s[0..2])?;
        let to = Square::from_str(&s[2..4])?;
        let promotion = s.chars().nth(5).map(Role::from);
        Ok(Move {
            from,
            to,
            promotion,
        })
    }
}

impl fmt::Display for Move {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(fmt, "{}", self.from)?;
        write!(fmt, "{}", self.to)
    }
}

#[derive(Clone)]
pub struct AvailableMoves {
    buf: Vec<Move>,
}

impl AvailableMoves {
    pub fn as_slice(&self) -> &[Move] {
        &self.buf[..self.buf.len()]
    }

    pub fn push(&mut self, m: Move) {
        self.buf.push(m);
    }
}

impl Default for AvailableMoves {
    fn default() -> Self {
        AvailableMoves {
            buf: Vec::with_capacity(140),
        }
    }
}

impl<'a> IntoIterator for &'a AvailableMoves {
    type Item = Move;
    type IntoIter = AvailableMovesIter<'a>;

    fn into_iter(self) -> Self::IntoIter {
        AvailableMovesIter {
            available_moves: self,
            index: self.buf.len(),
        }
    }
}

pub struct AvailableMovesIter<'a> {
    available_moves: &'a AvailableMoves,
    index: usize,
}

impl<'a> Iterator for AvailableMovesIter<'a> {
    type Item = Move;

    fn next(&mut self) -> Option<Self::Item> {
        if self.index == 0 {
            None
        } else {
            self.index -= 1;
            Some(self.available_moves.buf[self.index])
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;
    use std::str::FromStr;

    // Move validity is not relevant to parsing.
    #[test]
    fn move_parse_a1c7() {
        let expected_move = Move {
            from: Square::A1,
            to: Square::C7,
            promotion: None,
        };
        assert!(match Move::from_str("a1c7") {
            Ok(actual_move) => actual_move == expected_move,
            _ => false,
        });
    }

    #[test]
    fn move_e2e4_to_string_then_from_str() {
        let expected_move = Move {
            from: Square::from_str("e2").unwrap(),
            to: Square::from_str("e4").unwrap(),
            promotion: None,
        };
        assert!(match Move::from_str(expected_move.to_string().as_str()) {
            Ok(actual_move) => actual_move == expected_move,
            _ => false,
        });
    }
}

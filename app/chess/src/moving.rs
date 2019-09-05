use super::*;
use std::fmt;
use std::str::FromStr;
use zorro_common::result::Error;

#[derive(Copy, Clone, Debug, PartialEq)]
pub struct Move {
    pub from: Square,
    pub to: Square,
    pub promotion: Option<Role>,
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

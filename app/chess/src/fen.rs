use super::board::*;
use super::color::*;
use super::coordinates::*;
use super::piece::*;
use std::fmt;
use std::str::FromStr;

pub struct Fen<'b> {
    board: &'b Board,
    filler: char,
}

impl<'b> fmt::Display for Fen<'b> {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        for rank in Rank::all().rev() {
            let mut previous_emtpy_squares = 0;
            for file in File::all() {
                let square = Square::at(file, rank);
                match self.board.piece_opt_at(square) {
                    Some(piece) => {
                        if previous_emtpy_squares > 0 {
                            write!(fmt, "{}", previous_emtpy_squares)?;
                            previous_emtpy_squares = 0;
                        }
                        write!(fmt, "{}", char::from(piece))?;
                    }
                    None => previous_emtpy_squares += 1,
                }
            }
            if previous_emtpy_squares > 0 {
                write!(fmt, "{}", previous_emtpy_squares)?;
            }
            if rank != Rank::min() {
                write!(fmt, "/")?;
            }
        }
        write!(fmt, "{}", self.filler)?;
        write!(fmt, "{}", char::from(self.board.color_to_move))?;
        write!(fmt, "{}", self.filler)?;
        write!(fmt, "{}", self.board.castling_rights)?;
        write!(fmt, "{}", self.filler)?;
        if let Some(square) = self.board.en_passant_target_square {
            write!(fmt, "{}", square)?;
        } else {
            write!(fmt, "-")?;
        }
        Ok(())
    }
}

impl Board {
    pub fn fmt_fen<'s>(&'s self, filler: char) -> impl fmt::Display + 's {
        Fen {
            board: self,
            filler,
        }
    }

    pub fn from_fen<S: AsRef<str>>(fields: &mut impl Iterator<Item = S>) -> Self {
        let mut board = Board::empty();
        {
            let piece_map_str = fields.next().unwrap();
            let piece_map_by_rank = piece_map_str.as_ref().split('/');
            let mut rank = Rank::max();
            for rank_piece_map in piece_map_by_rank {
                let mut file = File::min();
                for c in rank_piece_map.chars() {
                    if let Some(digit) = c.to_digit(9) {
                        for _ in 0..digit {
                            let square = Square::at(file, rank);
                            board.set_at_square(square, None);
                            file = file.shift(1).unwrap();
                        }
                    } else {
                        let square = Square::at(file, rank);
                        let piece = Piece::from(c);
                        board.set_at_square(square, Some(piece));
                        file = file.shift(1).unwrap();
                    }
                }
                if let Some(rank_below) = rank.shift(1) {
                    rank = rank_below;
                } else {
                    break;
                }
            }
        }
        let color_to_move_str = fields.next().unwrap();
        board.color_to_move = Color::from(color_to_move_str.as_ref().chars().next().unwrap());
        board.castling_rights = CastlingRights::from_str(fields.next().unwrap().as_ref()).unwrap();
        board
    }
}

use super::board::*;
use super::color::*;
use super::coordinates::*;
use super::piece::*;
use crate::err::Error;
use std::fmt;
use std::str::FromStr;

pub struct Fen<'b> {
    board: &'b Board,
    filler: char,
}

impl<'b> fmt::Display for Fen<'b> {
    fn fmt(&self, fmt: &mut fmt::Formatter<'_>) -> fmt::Result {
        for rank in Rank::iter().rev() {
            let mut previous_emtpy_squares = 0;
            for file in File::iter() {
                let square = Square::at(file, rank);
                if let Some(piece) = self.board.piece_opt_at(square) {
                    if previous_emtpy_squares > 0 {
                        write!(fmt, "{}", previous_emtpy_squares)?;
                        previous_emtpy_squares = 0;
                    }
                    write!(fmt, "{}", char::from(piece))?;
                } else {
                    previous_emtpy_squares += 1;
                }
            }
            if previous_emtpy_squares > 0 {
                write!(fmt, "{}", previous_emtpy_squares)?;
            }
            if rank != Rank::min() {
                write!(fmt, "/")?;
            }
        }
        write!(
            fmt,
            "{}{}{0}{}{0}",
            self.filler,
            char::from(self.board.color_to_move),
            self.board.castling_rights,
        )?;
        if let Some(square) = self.board.en_passant_target_square {
            write!(fmt, "{}", square.to_string())?;
        } else {
            write!(fmt, "-")?;
        }
        write!(
            fmt,
            "{0}{1}{0}{2}",
            self.filler, self.board.half_moves_counter, self.board.full_moves_counter
        )?;
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

    pub fn from_fen<S: AsRef<str>>(fields: &mut impl Iterator<Item = S>) -> Result<Board, Error> {
        fn update_coordinates(file: &mut File, rank: &mut Rank) -> Option<()> {
            if let Some(shifted_file) = file.shift(1) {
                *file = shifted_file;
                Some(())
            } else if let Some(rank_below) = rank.shift(-1) {
                *file = File::min();
                *rank = rank_below;
                Some(())
            } else {
                None
            }
        }
        let mut board = Board::empty();
        let mut field = fields.next().ok_or(Error::InvalidFen)?;
        let mut file = File::min();
        let mut rank = Rank::max();
        for c in field.as_ref().chars().filter(|c| c.is_ascii_alphanumeric()) {
            if let Some(digit) = c.to_digit(10) {
                for _ in 0..digit {
                    let square = Square::at(file, rank);
                    board.set_at_square(square, None);
                    if update_coordinates(&mut file, &mut rank).is_none() {
                        break;
                    }
                }
            } else {
                let square = Square::at(file, rank);
                let piece = Piece::from(c);
                board.set_at_square(square, Some(piece));
                if update_coordinates(&mut file, &mut rank).is_none() {
                    break;
                }
            }
        }
        field = fields.next().ok_or(Error::InvalidFen)?;
        board.color_to_move = Color::from_str(field.as_ref())?;
        field = fields.next().ok_or(Error::InvalidFen)?;
        board.castling_rights = CastlingRights::from_str(field.as_ref())?;
        field = fields.next().ok_or(Error::InvalidFen)?;
        board.en_passant_target_square = if field.as_ref().starts_with('-') {
            None
        } else {
            Some(Square::from_str(field.as_ref())?)
        };
        field = fields.next().ok_or(Error::InvalidFen)?;
        board.half_moves_counter =
            str::parse::<usize>(field.as_ref()).map_err(|_| Error::InvalidFen)?;
        field = fields.next().ok_or(Error::InvalidFen)?;
        board.full_moves_counter =
            str::parse::<usize>(field.as_ref()).map_err(|_| Error::InvalidFen)?;
        Ok(board)
    }
}

#[cfg(test)]
mod test {
    use super::*;

    const TEST_CASES: &[&str] = &[
        "r3qbk1/pbp3pp/8/2p5/1rNnPP2/1PQPB3/2P3PP/R4R1K b - - 1 16",
        "4r1k1/1bq1rp1p/p2p2p1/1p2b3/3BP3/P1P5/1P3QPP/1B2RR1K w - - 0 26",
        "r4bk1/pp1b3r/3p1q2/P2Pp1n1/1P2PnP1/2N2B2/3B1NP1/2RQ1RK1 b - - 0 35",
        "r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 30 42",
        "r4rk1/1p3p2/3p3p/1PqPb3/p1P2pb1/3B1Pp1/P1Q3PP/2RNR2K b - - 72 0",
        "rnbqk1nr/ppppppbp/6p1/8/2PP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 1 1",
        "bnrqkrnb/pppppppp/8/8/8/6P1/PPPPPP1P/BNRQKRNB b KQkq - 0 1",
        "2rq2k1/1p1b1rpp/5b2/pN3p2/Q1Nnp3/P2P2P1/1P2PPBP/2R1R1K1 w - - 1 20",
        "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d3 0 1",
        "rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq d4 0 1",
        "R2K1B1R/PPP2QPP/5N1N/1n2q3/8/1p3n2/pbpp3p/1kr4r w KQkq - 0 1",
        "8/3p4/1P2R1p1/7p/pP5R/1p4r1/2p3p1/k1K2B2 w - - 0 0",
        "1KB2R1R/2P1B3/1P1Q4/1n3N2/qpp2n1P/8/6pp/r3rbk1 w - - 0 0",
        "2kr4/K1pp4/1p6/8/8/8/7Q/3R4 w - - 2 23",
        "r6r/ppp2Bpp/2n1b3/2R5/3k4/B7/P1P1KPPP/8 w - - 0 13",
        "r2q1rk1/pb1nbppp/1pp1pn2/3pN3/2PPP3/2N3P1/PP3PBP/R1BQ1RK1 w - - 0 1",
        "4R1K1/P5P1/3P1PP1/3BR3/5b2/p1p2p2/1Q3qpp/r3rk2 b - - 0 1",
        "1KR4R/PPPB1P1P/4Q1Pq/4P1b1/1B1Nn3/3p3p/ppp2pp1/1kr2bnr w - - 0 1",
    ];

    #[test]
    fn back_and_forth_samples() {
        for fen in TEST_CASES.iter() {
            let board = Board::from_fen(&mut fen.split_whitespace()).unwrap();
            assert_eq!(fen, &board.fmt_fen(' ').to_string().as_str());
        }
    }

    #[test]
    fn back_and_forth_init() {
        let default_board_fen = Board::default().fmt_fen(' ').to_string();
        assert_eq!(
            Board::from_fen(&mut default_board_fen.split_whitespace()),
            Ok(Board::default())
        );
    }
}

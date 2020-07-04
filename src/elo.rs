pub type Elo = u16;

pub const OWN_ELO: Elo = 1400;

pub fn expected_score(own: Elo, other: Elo) -> f32 {
    1.0 / (1.0 + 10f32.powf((other as f32 - own as f32) / 400.0))
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn contempt_2000_vs_1400_is_quite_high() {
        assert!(expected_score(2122, 1401) > 0.9);
    }

    #[test]
    fn contempt_3200_vs_3200_is_neutral() {
        let contempt = expected_score(3200, 3200);
        assert!(contempt > 0.49 && contempt < 0.51);
    }
}

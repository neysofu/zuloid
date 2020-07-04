/// Chess clock abstractions.
///
/// Note, this is NOT time management. The following code simply provides
/// the abstractions needed to reason about time during a chess game.
///
/// Resources:
///   - https://en.wikipedia.org/wiki/Chess_clock
use std::rc::Rc;
use std::time::{Duration, Instant};

/// An enforceable limit to thinking time. It supports several advanced
/// features, like:
///   - Fischer increments;
///   - Bronstein delays;
///   - overtime;
///   - sudden death.
///
/// You can model a wide range of real-life time controls using this
/// abstraction, including many unorthodox ones, such as byo-yomi and Canadian
/// overtime. Under the hood, time controls are modelled by breaking them into
/// multiple "periods". After time depletion of each period, two things can
/// happen:
///   - Sudden death.
///   - Switch to a custom "overtime" period.
///
/// Periods can also expire when a certain move number is reached, e.g.:
///
///   "90 minutes for the first 40 moves followed by ..."
///
/// These two mechanisms (custom behavior on overtime and move number limit)
/// allow for modelling even complex time controls.
///
/// TODO:
///   - Implement a basic parser for time control notation
///   (https://chess.stackexchange.com/questions/8283/time-control-notation-explained).
///
/// Resources:
///   - https://en.wikipedia.org/wiki/Time_control
#[derive(Clone, Default)]
pub struct TimeControl {
    // Time bank available to the played at the beginning of the period.
    pub time_limit: Duration,
    /// Time added to the time bank after every move.
    pub increment: Duration,
    /// Delay after which the clock starts ticking at the beginning of every
    /// turn. It can be negative to account for network latency.
    pub delay: Duration,
    // Upper limit on the number of moves that can be played inside this
    // period.
    pub length: Option<usize>,
    pub next: Option<Rc<TimeControl>>,
    /// If absent, will result in sudden death.
    pub overtime: Option<Rc<TimeControl>>,
}

impl TimeControl {
    pub fn infinite() -> Self {
        let year = Duration::from_secs(60 * 60 * 24 * 365);
        TimeControl {
            time_limit: year,
            increment: year,
            ..Default::default()
        }
    }

    pub fn new_with_increment_secs(
        time_limit: usize,
        increment: usize,
    ) -> Self {
        TimeControl {
            time_limit: Duration::from_secs(time_limit as u64),
            increment: Duration::from_secs(increment as u64),
            ..Default::default()
        }
    }
}

/// This clock abstraction mutably implements a time control. In others words,
/// time controls are blueprints for creating clocks.
pub struct Clock {
    time_left: Duration,
    ticking_since: Option<Instant>,
    length_left: Option<usize>,
    // We keep the original time control for reference.
    time_control: TimeControl,
}

impl Clock {
    /// A note about increments: technically, FIDENTITYE includes them in the
    /// ticking time right from the first move. This clock behavior is,
    /// however, really uncommon and unexpected - so we don't implement it.
    ///
    /// How much time does the current player have to play a move?
    fn cumulative_time_limit(&self) -> Duration {
        let mut time_left = self.time_left;
        let mut tc = &self.time_control;
        while let Some(overtime) = &tc.overtime {
            time_left += overtime.time_limit;
            tc = &*overtime;
        }
        time_left
    }

    /// Starts the clock countdown and returns the time available to the
    /// current player.
    pub fn tick(&mut self) -> Duration {
        assert!(self.ticking_since.is_none());
        self.ticking_since = Some(Instant::now());
        self.cumulative_time_limit()
    }

    /// Stops the ticking clock. It returns the time spent ticking if the clock
    /// hasn't flagged, None otherwise.
    ///
    /// # Assumptions
    /// This function assumes that the current clock is already ticking and
    /// will panic otherwise.
    pub fn tock(&mut self) -> Option<Duration> {
        let ticking_since = self.ticking_since.unwrap();
        let now = Instant::now();
        if let Some(tick_duration) = self
            .time_left
            .checked_sub(now.duration_since(ticking_since))
        {
            self.time_left -= now.duration_since(ticking_since);
            self.time_left += self.time_control.increment;
            Some(tick_duration)
        } else {
            self.time_left = Duration::default();
            None
        }
    }
}

impl From<TimeControl> for Clock {
    fn from(tc: TimeControl) -> Self {
        Clock {
            time_left: tc.time_limit,
            ticking_since: None,
            length_left: tc.length,
            time_control: tc,
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn cumulative_time_limit_without_increment_is_time_limit() {
        let time_control = TimeControl::new_with_increment_secs(60, 0);
        assert_eq!(
            Clock::from(time_control).cumulative_time_limit(),
            Duration::from_secs(60)
        );
    }

    #[test]
    fn available_time_on_tick_equals_cumulative_time_limit() {
        let time_control = TimeControl::new_with_increment_secs(60, 0);
        let mut clock = Clock::from(time_control);
        assert_eq!(clock.cumulative_time_limit(), clock.tick());
    }

    #[test]
    fn clock_can_flag() {
        let time_control = TimeControl {
            time_limit: Duration::from_nanos(1337),
            increment: Duration::default(),
            ..Default::default()
        };
        let mut clock = Clock::from(time_control);
        clock.tick();
        std::thread::sleep(Duration::from_millis(1));
        assert!(clock.tock().is_none());
    }

    #[test]
    fn clock_doesnt_flag() {
        let time_control = TimeControl::default();
        let mut clock = Clock::from(time_control);
        clock.tick();
        std::thread::sleep(Duration::from_millis(1));
        assert!(clock.tock().is_some());
    }
}

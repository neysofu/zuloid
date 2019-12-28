use std::boxed::Box;
use std::time::{Duration, Instant};

pub struct TimeControl {
    time_limit: Duration,
    increment: Duration,
    delay: Duration,
    length: Option<usize>,
    next: Option<Box<TimeControl>>,
    /// After main time has expired, this optional time control will kick in
    /// immediately.
    ///
    /// This overtime hack supports many unorthodox time controls, such as
    /// byo-yomi and Canadian overtime.
    overtime: Option<Box<TimeControl>>,
}

impl TimeControl {
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

impl Default for TimeControl {
    fn default() -> Self {
        TimeControl {
            time_limit: Duration::from_secs(3 * 60),
            increment: Duration::from_secs(2),
            delay: Duration::default(),
            length: None,
            next: None,
            overtime: None,
        }
    }
}

pub struct Clock {
    time_left: Duration,
    ticking_since: Option<Instant>,
    length_left: Option<usize>,
    time_control: TimeControl,
}

impl Clock {
    /// A note about increments: technically, FIDE includes them in the ticking
    /// time right from the first move. This clock behavior is, however, really
    /// uncommon and unexpected - so we don't implement it.
    fn cumulative_time_limit(&self) -> Duration {
        let mut time_left = self.time_left;
        let mut time_control = &self.time_control;
        while let Some(overtime) = &time_control.overtime {
            time_left += overtime.time_limit;
            time_control = &*overtime;
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

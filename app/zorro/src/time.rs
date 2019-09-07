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
    pub fn new_with_increment_secs(time_limit: usize, increment: usize) -> Self {
        TimeControl {
            time_limit: Duration::from_secs(time_limit as u64),
            increment: Duration::from_secs(increment as u64),
            ..Default::default()
        }
    }

    fn cumulative_time_limit(&self) -> Duration {
        self.time_limit // TODO
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
    /// Starts the clock countdown and returns the time available to the current
    /// player.
    pub fn tick(&mut self) -> Duration {
        assert!(self.ticking_since.is_none());
        self.ticking_since = Some(Instant::now());
        self.time_control.cumulative_time_limit()
    }

    /// Stops the ticking clock
    ///
    /// # Assumptions
    /// This function assumes that the current clock is already ticking and will
    /// panic otherwise.
    pub fn tock(&mut self) {
        let ticking_since = self.ticking_since.unwrap();
        let now = Instant::now();
        self.time_left -= now.duration_since(ticking_since);
        self.time_left += self.time_control.increment;
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
    // TODO
}

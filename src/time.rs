use std::boxed::Box;
use std::time::Duration;

pub struct TimeControl {
    pub time_limit: Duration,
    pub increment: Duration,
    pub delay: Duration,
    pub length: Option<usize>,
    pub next: Option<Box<TimeControl>>,
    /// After main time is expired, this optional time control will kick in. It
    /// will kick in as soon as the current time is finished.
    ///
    /// I'm aware of many unorthodox time controls and this overtime method
    /// supports quite a few of them, including byo-yomi and Canadian overtime.
    pub overtime: Option<Box<TimeControl>>,
}

impl TimeControl {
    pub fn new_with_increment_secs(time_limit: usize, increment: usize) -> Self {
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

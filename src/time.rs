use std::boxed::Box;
use std::time::Duration;

#[derive(Default)]
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
    /// 1+0
    pub fn new_bullet() -> Self {
        TimeControl::new_with_increment_secs(3 * 60, 0)
    }

    /// 3+2
    pub fn new_blitz() -> Self {
        TimeControl::new_with_increment_secs(3 * 60, 2)
    }

    /// 15+10
    pub fn new_rapid() -> Self {
        TimeControl::new_with_increment_secs(15 * 60, 10)
    }

    /// 90+30 plus 30 extra minutes after move 40.
    fn new_classical() -> Self {
        let next = TimeControl::new_with_increment_secs(30 * 60, 0);
        TimeControl {
            time_limit: Duration::from_secs(90 * 60),
            increment: Duration::from_secs(30),
            length: Some(40),
            next: Some(Box::new(next)),
            ..TimeControl::default()
        }
    }

    pub fn new_with_increment_secs(time_limit: usize, increment: usize) -> Self {
        TimeControl {
            time_limit: Duration::from_secs(time_limit as u64),
            increment: Duration::from_secs(increment as u64),
            ..Default::default()
        }
    }
}

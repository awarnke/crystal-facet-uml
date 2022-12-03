use std::ops::Add;
use std::ops::AddAssign;

#[derive(Debug, Copy, Clone, PartialEq)]
pub struct TestResult {
    pub failed: u32,
    pub total: u32,
}

impl Add for TestResult {
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            failed: self.failed + other.failed,
            total: self.total + other.total,
        }
    }
}

impl AddAssign for TestResult {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            failed: self.failed + other.failed,
            total: self.total + other.total,
        }
    }
}

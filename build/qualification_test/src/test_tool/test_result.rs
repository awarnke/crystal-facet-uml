//! This module implements a TestResult to collect statistics on a performed
//! test run
use std::ops::Add;
use std::ops::AddAssign;

/// A TestResult is a set of statistical data on a performed test run
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct TestResult {
    /// The number of failed test cases in a test run
    pub failed: u32,
    /// The number of total test cases in a test run
    pub total: u32,
}

/// The Add trait on a TestResult allows to combine the test results of
/// several sets of performed test cases
impl Add for TestResult {
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            failed: self.failed + other.failed,
            total: self.total + other.total,
        }
    }
}

/// The AddAssign trait on a TestResult allows to combine the test results of
/// several sets of performed test cases
impl AddAssign for TestResult {
    fn add_assign(&mut self, other: Self) {
        *self = Self {
            failed: self.failed + other.failed,
            total: self.total + other.total,
        }
    }
}

/// The AddAssign trait on a TestResult and a Result allows to add the result
/// of a single performed test case
impl AddAssign<Result<(), ()>> for TestResult {
    fn add_assign(&mut self, other: Result<(), ()>) {
        let other_failed: u32 = match other {
            Ok(_) => 0,
            Err(_) => 1,
        };
        *self = Self {
            failed: self.failed + other_failed,
            total: self.total + 1,
        }
    }
}

/*
Copyright 2022-2026 Andreas Warnke

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

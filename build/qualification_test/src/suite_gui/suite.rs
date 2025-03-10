//! This module implements a TestSuite for testing the graphical user interface
//! of crystal-facet-uml.

use crate::test_tool::test_case::TestCase;
use crate::test_tool::test_suite::TestSuite;

/// A test suite consists of a name, a test fixture and an array of test cases
///
/// # Lifetimes
///
/// * `'all_testing` refers to the lifetime of `TestSuite` and `TestCase`
///   objects: They exist during the whole test run.
///
pub struct SuiteGui<'all_testing> {
    name: &'all_testing str,
    cases: [TestCase<'all_testing, ()>; 0],
}

/// A test suite comes with a constructor
impl<'all_testing, 'a, 'b> SuiteGui<'all_testing>
where
    'a: 'all_testing,
    'b: 'all_testing,
{
    /// Defines all test cases in this test suite
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    ///
    pub fn new(_exe_to_test: &'a str, _temp_dir: &'b str) -> SuiteGui<'all_testing> {
        SuiteGui {
            name: "SuiteGui",
            cases: [],
        }
    }
}

/// The `SuiteGui` test suite implements the `TestSuite` trait
impl<'all_testing> TestSuite<'all_testing, ()> for SuiteGui<'all_testing> {
    /// The name of the test suite
    fn name(self: &'all_testing Self) -> &'all_testing str {
        self.name
    }

    /// Creates a test directory
    ///
    /// Panics if the test environment reports errors.
    fn setup(self: &'all_testing Self) -> () {
        ()
    }

    /// Removes the test directory
    ///
    /// Panics if the test environment reports errors.
    fn teardown(self: &'all_testing Self, _environment: ()) -> () {}

    fn testcases(self: &'all_testing Self) -> &'all_testing [TestCase<'all_testing, ()>] {
        &self.cases
    }
}

/*
Copyright 2022-2025 Andreas Warnke

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

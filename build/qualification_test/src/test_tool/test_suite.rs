//! This module implements a test suite which sets up test
//! fixtures (test environments) and provides a list of test cases.

use super::test_case::TestCase;

/// A test suite is an object that provides functions to
/// setup a `TestFixture` (test environment) for each test case,
/// to run each test case and to tear down the test fixtures.
///
/// # Lifetimes
///
/// * `'all_testing` refers to the lifetime of `TestSuite` and `TestCase`
///   objects: They exist during the whole test run.
/// * `'during_run` refers to the lifetime of a `TestFixture`: This is set up
///   for the duration of executing one test case.
///
/// # Generics
///
/// * `TestFixture` is a generic test environment. The exact type may differ
///   between different `TestSuite`s.
pub trait TestSuite<'all_testing, 'during_run, TestFixture>
where
    'all_testing: 'during_run,
{
    /// The name of this test suite
    fn name(self: &'all_testing Self) -> &'all_testing str;
    /// This function sets up the test environment (`TestFixture`) for the test
    /// cases in this test suite.
    fn setup(self: &'all_testing Self) -> &'during_run TestFixture;
    /// This function cleans up the test environment (`TestFixture`) for the
    /// test cases in this test suite.
    ///
    /// # Arguments
    ///
    /// * `environment` - A test fixture to be cleaned up
    fn teardown(self: &'all_testing Self, environment: &'during_run TestFixture) -> ();
    /// List of test cases in this test suite
    fn testcases(
        self: &'all_testing Self,
    ) -> &'all_testing [TestCase<'all_testing, 'during_run, TestFixture>];
}

/*
Copyright 2022-2022 Andreas Warnke

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

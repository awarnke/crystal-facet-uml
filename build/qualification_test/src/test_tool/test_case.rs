//! This module implements a TestCase that is run in a generic test
//! environment TestFixture

/// A test case consists of a name and a run function.
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
pub struct TestCase<'all_testing, 'during_run, TestFixture> {
    pub(super) name: &'all_testing str,
    pub(super) run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
}

/// The `TestCase` struct comes with a constructor
impl<'all_testing, 'during_run, TestFixture> TestCase<'all_testing, 'during_run, TestFixture> {
    /// Defines a test fixture (the test environment)
    ///
    /// # Arguments
    ///
    /// * `name` - Name of the test case
    /// * `run` - Function to call when the test case shall be run.
    ///   This function may panic in case of an error in the test environment;
    ///   it shall return `Result::Err(())` if the test case did not produce
    ///   the expected observation, `Result::Ok(())` in case of success.
    pub fn new(
        name: &'static str,
        run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
    ) -> TestCase<'all_testing, 'during_run, TestFixture> {
        TestCase {
            name: name,
            run: run,
        }
    }
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

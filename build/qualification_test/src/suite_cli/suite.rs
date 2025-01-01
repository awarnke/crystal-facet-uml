//! This module implements a TestSuite for testing the command line interface
//! of crystal-facet-uml.

use super::fixture::FixtureCli;
use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_import_to_new_cfu1;
use super::test_repair;
use crate::test_tool::test_case::TestCase;
use crate::test_tool::test_suite::TestSuite;
use std::fs;

/// A test suite consists of a name, a test fixture and an array of test cases
///
/// # Lifetimes
///
/// * `'all_testing` refers to the lifetime of `TestSuite` and `TestCase`
///   objects: They exist during the whole test run.
/// * `'during_run` refers to the lifetime of a `TestFixture`: This is set up
///   for the duration of executing one single test case.
///
pub struct SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
{
    name: &'all_testing str,
    exe_to_test: &'all_testing str,
    temp_dir: &'all_testing str,
    cases: [TestCase<'all_testing, FixtureCli<'during_run>>; 7],
}

/// The `SuiteCli` test suite comes with a constructor
impl<'all_testing, 'during_run, 'a, 'b> SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
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
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> SuiteCli<'all_testing, 'during_run> {
        SuiteCli {
            name: "SuiteCli",
            exe_to_test: exe_to_test,
            temp_dir: temp_dir,
            cases: [
                TestCase::new("testcase_version", testcase_version),
                TestCase::new("testcase_help", testcase_help),
                TestCase::new(
                    "testcase_repair_new_cfu1",
                    test_repair::testcase_repair_new_cfu1,
                ),
                TestCase::new(
                    "testcase_test_new_cfu1",
                    test_repair::testcase_test_new_cfu1,
                ),
                TestCase::new(
                    "testcase_repair_json_rw",
                    test_repair::testcase_repair_json_rw,
                ),
                TestCase::new("testcase_test_json_ro", test_repair::testcase_test_json_ro),
                TestCase::new("testcase_import_to_new_cfu1", testcase_import_to_new_cfu1),
            ],
        }
    }
}

/// The `SuiteCli` test suite implements the `TestSuite` trait
impl<'all_testing, 'during_run> TestSuite<'all_testing, FixtureCli<'during_run>>
    for SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
{
    /// The name of the test suite
    fn name(self: &'all_testing Self) -> &'all_testing str {
        self.name
    }

    /// Creates a test directory
    ///
    /// Panics if the test environment reports errors.
    fn setup(self: &'all_testing Self) -> FixtureCli<'during_run>
    where
        'all_testing: 'during_run,
    {
        let environment = FixtureCli::new(self.exe_to_test, self.temp_dir);
        /* create the temp directory, panic in case an error occured */
        fs::create_dir_all(environment.temp_dir).unwrap();
        environment
    }

    /// Removes the test directory
    ///
    /// Panics if the test environment reports errors.
    fn teardown(self: &'all_testing Self, environment: FixtureCli<'during_run>) -> () {
        /* delete the temp directory in case of successful test result, panic in case an error occured */
        fs::remove_dir_all(environment.temp_dir).unwrap();
    }

    fn testcases(
        self: &'all_testing Self,
    ) -> &'all_testing [TestCase<'all_testing, FixtureCli<'during_run>>] {
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

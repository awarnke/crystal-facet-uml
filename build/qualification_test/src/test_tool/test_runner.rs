//! This module implements a test runner which sets up test fixtures, runs test
//! cases and collects statistics on test results for one TestSuite

use super::test_result::TestResult;
use super::test_suite::TestSuite;

/// A test runner provides a run function which executes all test cases in the
/// provided test suite
///
/// # Arguments
///
/// * `suite` - The test suite to execute
///
/// # Lifetimes
///
/// * `'all_testing` refers to the lifetime of `TestSuite` and `TestCase`
///   objects: They exist during the whole test run.
///
/// # Generics
///
/// * `TestFixture` is a generic test environment. The exact type may differ
///   between different `TestSuite`s.
///
pub fn run<'all_testing, TestFixture>(
    suite: &'all_testing (dyn TestSuite<'all_testing, TestFixture> + 'all_testing),
) -> TestResult {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    println!(
        "                                        [__ {} __]",
        suite.name()
    );
    for case in suite.testcases() {
        println!(
            "                                        [__ > {} __]",
            case.name
        );
        let mut environment = suite.setup();
        let testcase_result = (case.run)(&mut environment);
        result += testcase_result;
        suite.teardown(environment);
        println!(
            "                                        [__ > {}: {} __]\n",
            case.name,
            match testcase_result {
                Ok(()) => "Ok",
                Err(()) => "Err",
            }
        );
    }

    result
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

use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_import_to_new_cfu1;
use super::test_repair::testcase_repair_new_cfu1;
use crate::test_tool::test_case::TestCase;
use crate::test_tool::test_result::TestResult;
use crate::test_tool::test_suite::TestSuite;
use std::fs;

pub struct SuiteCli<'a> {
    pub name: &'static str,
    exe_to_test: &'a str,
    temp_dir: &'a str,
}

/// Runs all test cases in this test suite
///
/// # Arguments
///
/// * `exe_to_test` - A path to the executable to be tested
/// * `temp_dir` - A path to a directory that exists and can be used for testing
pub fn suite_cli_run(exe_to_test: &str, temp_dir: &str) -> TestResult {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    result += testcase_version(exe_to_test);
    result += testcase_help(exe_to_test);

    let suite: &dyn TestSuite = &SuiteCli {
        name: "SuiteCli",
        exe_to_test: exe_to_test,
        temp_dir: temp_dir,
    };

    suite.setup();
    result += testcase_repair_new_cfu1(exe_to_test, temp_dir);
    suite.teardown();

    suite.setup();
    result += testcase_import_to_new_cfu1(exe_to_test, temp_dir);
    suite.teardown();

    result
}

impl<'a> TestSuite for SuiteCli<'a> {
    /// Creates a test directory
    ///
    /// Panics if the test environment reports errors.
    fn setup(self: &Self) -> () {
        /* create the temp directory, panic in case an error occured */
        fs::create_dir_all(self.temp_dir).unwrap();
    }

    /// Removes the test directory
    ///
    /// Panics if the test environment reports errors.
    fn teardown(self: &Self) -> () {
        /* delete the temp directory in case of successful test result, panic in case an error occured */
        fs::remove_dir_all(self.temp_dir).unwrap();
    }

    fn testcases(self: &Self) -> &'static [TestCase] {
        &[]
    }
}

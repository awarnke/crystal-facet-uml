use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_create_cfu1;
use super::test_repair::testcase_repair_cfu1;
use crate::test_tool::test_result::TestResult;
use std::fs;

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

    setup(temp_dir);
    result += testcase_repair_cfu1(exe_to_test, temp_dir);
    teardown(temp_dir);

    setup(temp_dir);
    result += testcase_create_cfu1(exe_to_test, temp_dir);
    teardown(temp_dir);

    result
}

/// Creates a test directory
///
/// Panics if the test environment reports errors.
fn setup(temp_dir: &str) -> () {
    /* create the temp directory, panic in case an error occured */
    fs::create_dir_all(temp_dir).unwrap();
}

/// Removes the test directory
///
/// Panics if the test environment reports errors.
fn teardown(temp_dir: &str) -> () {
    /* delete the temp directory in case of successful test result, panic in case an error occured */
    fs::remove_dir_all(temp_dir).unwrap();
}

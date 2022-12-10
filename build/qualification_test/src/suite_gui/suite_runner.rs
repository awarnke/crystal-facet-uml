use crate::test_tool::test_result::TestResult;

/// Runs all test cases in this test suite
///
/// # Arguments
///
/// * `_exe_to_test` - A path to the executable to be tested
/// * `_temp_dir` - A path to a directory that exists and can be used for testing
pub fn suite_gui_run(_exe_to_test: &str, _temp_dir: &str) -> TestResult {
    TestResult {
        failed: 0,
        total: 0,
    }
}

use crate::test_tool::test_result::TestResult;
use regex::Regex;

/// Test that creating an sqlite3 based format is possible.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
pub(super) fn testcase_create_cfu1(exe_to_test: &String,temp_dir: &String) -> TestResult {
    let output = match std::process::Command::new(exe_to_test)
        .args(&["-t",temp_dir])
        .output()
    {
        Ok(output) => output,
        Err(err) => panic!("Err at running process: {}", err),
    };

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout) => stdout,
        Err(err) => panic!("Err at translating output: {}", err),
    };

    /* check if the returned string looks valid */
    let expected_pattern = Regex::new(r"^\s*\d+\.\d+\.\d+\s*$").unwrap();
    let success = expected_pattern.is_match(&stdout);

    /* check that the exit code is 0 */
    let exit_ok: bool = output.status.success();

    print!("testcase_version: <<{}>>:{}\n", stdout, stdout.len());
    TestResult {
        failed: if success && exit_ok { 0 } else { 1 },
        total: 1,
    }
}

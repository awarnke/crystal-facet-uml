use crate::test_tool::test_result::TestResult;
use regex::Regex;
use std::process::Stdio;

/// Test that requesting the version is possible.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
pub(super) fn testcase_version(exe_to_test: &str) -> TestResult {
    let output = match std::process::Command::new(exe_to_test)
        .args(&["-v"])
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

/// Test that requesting a help string is possible.
///
/// Returns result of check if the result string looks like a help string,
/// panics if the test environment reports errors.
pub(super) fn testcase_help(exe_to_test: &str) -> TestResult {
    let process = match std::process::Command::new(exe_to_test)
        .args(&["-h"])
        .stdout(Stdio::piped())
        .spawn()
    {
        Ok(process) => process,
        Err(err) => panic!("Err at running process: {}", err),
    };

    let output = match process.wait_with_output() {
        Ok(output) => output,
        Err(err) => panic!("Err at retrieving output: {}", err),
    };

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout) => stdout,
        Err(err) => panic!("Err at translating output: {}", err),
    };

    /* check if the returned string looks valid */
    let success = (stdout.len() >= 200) && (stdout.len() <= 1000);

    /* check that the exit code is 0 */
    let exit_ok: bool = output.status.success();

    println!("testcase_help: <<{}>>:{}", stdout, stdout.len());
    TestResult {
        failed: if success && exit_ok { 0 } else { 1 },
        total: 1,
    }
}

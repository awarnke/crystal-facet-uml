use crate::test_tool::test_result::TestResult;
use std::fs;
use std::path::PathBuf;

/// Test that creating an sqlite3 based format is possible.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
pub(super) fn testcase_create_cfu1(exe_to_test: &String, temp_dir: &String) -> TestResult {
    /* create the temp directory, panic in case an error occured */
    fs::create_dir_all(temp_dir).unwrap();

    /* create the db_to_use_param string, panic in case an error occured */
    let mut db_to_use = PathBuf::from(temp_dir);
    db_to_use.push("sqlite3_db.cfu1");
    let db_to_use_param = db_to_use.into_os_string().into_string().unwrap();

    let output = match std::process::Command::new(exe_to_test)
        .args(&["-r", &db_to_use_param])
        .output()
    {
        Ok(output) => output,
        Err(err) => panic!("Err at running process: {}", err),
    };

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout) => stdout,
        Err(err) => panic!("Err at translating output: {}", err),
    };

    /* check if the returned string looks as expected */
    let success = stdout.contains("Errors found: 1");

    /* check that the exit code is 0 */
    let exit_ok: bool = output.status.success();

    /* cleanup */
    if success && !exit_ok {
        /* delete the temp directory in case of successful test result, panic in case an error occured */
        fs::remove_dir_all(temp_dir).unwrap();
    }

    print!("testcase_create_cfu1: <<{}>>:{}\n", stdout, stdout.len());
    TestResult {
        failed: if success && !exit_ok { 0 } else { 1 },
        total: 1,
    }
}

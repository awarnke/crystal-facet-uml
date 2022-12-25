use super::fixture::FixtureCli;
use std::path::PathBuf;
use std::process::Command;

/// Test that creating an sqlite3 based format is possible and can be repaired.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
///
/// # Arguments
///
/// * `environment` - A test fixture stating the test environment
pub(super) fn testcase_repair_new_cfu1(environment: &FixtureCli) -> Result<(), ()> {
    /* create the db_to_use_param string, panic in case an error occured */
    let mut db_to_use = PathBuf::from(environment.temp_dir);
    db_to_use.push("sqlite3_db.cfu1");
    let db_to_use_param = db_to_use.into_os_string().into_string().unwrap();

    let output = Command::new(environment.exe_to_test)
        .args(&["-r", &db_to_use_param])
        .output()
        .expect("Err at running process");

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout) => stdout,
        Err(_err) => return Result::Err(()),
    };

    /* check if the returned string looks as expected */
    /* 1 error should be found which is the missing root diagram */
    let as_expected = stdout.contains("Errors found: 1");

    /* check that the exit code is 0 */
    let exit_ok: bool = output.status.success();

    print!(
        "testcase_repair_new_cfu1: <<{}>>:{}\n",
        stdout,
        stdout.len()
    );
    if as_expected && !exit_ok {
        Result::Ok(())
    } else {
        Result::Err(())
    }
}

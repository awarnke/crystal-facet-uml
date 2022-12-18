use crate::test_tool::test_result::TestResult;
use std::fs::File;
use std::io::Write;
use std::path::PathBuf;
use std::process::Command;

/// Returns a string slice containting json data.
fn simplejson() -> &'static str {
    "\
{
  \"head\": {
    \"encoding\": \"utf-8\"
  }
  ,
  \"views\":
  [
  ]
  ,
  \"nodes\":
  [
  ]
  ,
  \"edges\":
  [
  ]
}
"
}

/// Test that importing to a newly created sqlite3 based format is possible.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
///
/// # Arguments
///
/// * `temp_dir` - A path to a directory that exists and can be used for testing
pub(super) fn testcase_import_to_new_cfu1(exe_to_test: &str, temp_dir: &str) -> TestResult {
    /* create the db_to_use_param string, panic in case an error occured */
    let mut db_to_use = PathBuf::from(temp_dir);
    db_to_use.push("sqlite3_db.cfu1");
    let db_to_use_param = db_to_use.into_os_string().into_string().unwrap();

    /* create the file to import, panic in case an error occured */
    let mut json_to_use = PathBuf::from(temp_dir);
    json_to_use.push("json.txt");
    let json_to_use_param = json_to_use.into_os_string().into_string().unwrap();
    let mut json_file = File::create(&json_to_use_param).unwrap();
    write!(json_file, "{}", simplejson()).expect("File could not be written");

    let output1 = Command::new(exe_to_test)
        .args(&["-i", &db_to_use_param, "add", &json_to_use_param])
        .output()
        .expect("Err at running process");

    let stdout1 = match std::string::String::from_utf8(output1.stdout) {
        Ok(stdout) => stdout,
        Err(_err) => {
            return TestResult {
                failed: 1,
                total: 1,
            }
        }
    };

    /* check if the returned string looks as expected */
    let as_expected1 = stdout1.contains("  errors: \t\t\t\t\t\t: 0");

    /* check that the exit code is 0 */
    let exit_ok1: bool = output1.status.success();

    print!(
        "testcase_import_to_new_cfu1: <<{}>>:{}\n",
        stdout1,
        stdout1.len()
    );
    TestResult {
        failed: if as_expected1 && exit_ok1 { 0 } else { 1 },
        total: 1,
    }
}

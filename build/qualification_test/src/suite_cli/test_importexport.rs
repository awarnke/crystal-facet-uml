use super::fixture::FixtureCli;
use std::fs::File;
use std::io::Write;
use std::path::PathBuf;
use std::process::Command;

/// A string containing valid json format but no model data
static JSON_EMPTY_CONTENT: &'static str = "\
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
";

/// Test that importing to a newly created sqlite3 based format is possible.
///
/// # Arguments
///
/// * `environment` - A test fixture stating the test environment
///
/// # Errors
///
/// This function returns `Err()` if the test case produced an unexpected result
///
/// # Panics
///
/// This function panics if the test environment causes errors.
///
pub(super) fn testcase_import_to_new_cfu1(environment: &mut FixtureCli) -> Result<(), ()> {
    /* create the db_to_use_param string, panic in case an error occured */
    let mut db_to_use = PathBuf::from(environment.temp_dir);
    db_to_use.push("sqlite3_db.cfu1");
    let db_to_use_param = db_to_use.into_os_string().into_string().unwrap();

    /* create the file to import, panic in case an error occured */
    let mut json_to_use = PathBuf::from(environment.temp_dir);
    json_to_use.push("json.txt");
    let json_to_use_param = json_to_use.into_os_string().into_string().unwrap();
    {
        let mut json_file = File::create(&json_to_use_param).unwrap();
        write!(json_file, "{}", JSON_EMPTY_CONTENT).expect("File could not be written");
        let mut json_perms = json_file.metadata().expect("no metadata").permissions();
        json_perms.set_readonly(true);
        json_file
            .set_permissions(json_perms)
            .expect("cannot set readonly flag");
    }

    let output1 = Command::new(environment.exe_to_test)
        .args(&["-i", &db_to_use_param, "add", &json_to_use_param])
        .output()
        .expect("Err at running process");

    let stdout1 = match std::string::String::from_utf8(output1.stdout) {
        Ok(stdout) => stdout,
        Err(_err) => return Result::Err(()),
    };

    /* check if the returned string looks as expected */
    let as_expected1 = stdout1.contains("  errors: \t\t\t\t\t\t\t: 0");

    /* check that the exit code is 0 */
    let exit_ok1: bool = output1.status.success();

    print!(
        "testcase_import_to_new_cfu1: <<{}>>:{}\n",
        stdout1,
        stdout1.len()
    );
    if as_expected1 && exit_ok1 {
        Result::Ok(())
    } else {
        Result::Err(())
    }
}

/*
Copyright 2022-2024 Andreas Warnke

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

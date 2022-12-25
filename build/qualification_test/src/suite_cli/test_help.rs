use super::fixture::FixtureCli;
use regex::Regex;
use std::process::Stdio;

/// Test that requesting the version is possible.
///
/// Returns result of check if the result string looks like a version,
/// panics if the test environment reports errors.
///
/// # Arguments
///
/// * `environment` - A test fixture stating the test environment
pub(super) fn testcase_version(environment: &FixtureCli) -> Result<(), ()> {
    let output = match std::process::Command::new(environment.exe_to_test)
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
    if success && exit_ok {
        Result::Ok(())
    } else {
        Result::Err(())
    }
}

/// Test that requesting a help string is possible.
///
/// Returns result of check if the result string looks like a help string,
/// panics if the test environment reports errors.
///
/// # Arguments
///
/// * `environment` - A test fixture stating the test environment
pub(super) fn testcase_help(environment: &FixtureCli) -> Result<(), ()> {
    let process = match std::process::Command::new(environment.exe_to_test)
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
    if success && exit_ok {
        Result::Ok(())
    } else {
        Result::Err(())
    }
}

/*
Copyright 2022-2022 Andreas Warnke

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

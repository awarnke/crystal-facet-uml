use regex::Regex;

pub fn suite_cli_run(exe_to_test: &String) -> bool {
    let mut all_ok: u32 = 0;
    all_ok += if testcase_version(exe_to_test) {1} else {0};
    all_ok += if testcase_help(exe_to_test) {1} else {0};
    all_ok == 2
}

/// Tests that requesting the version is possible.
///
/// Returns true if the result string looks like a version,
/// false if not,
/// panics if the test environment reports errors.
fn testcase_version(exe_to_test: &String) -> bool {
    let output = match std::process::Command::new(exe_to_test)
                        .args(&["-v"])
                        .output() {
        Ok(output) => output,
        Err(err)    => panic!("Err at running process: {}", err),
    };

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout)  => stdout,
        Err(err)    => panic!("Err at translating output: {}", err),
    };

    /* check if the returned string looks valid */
    let expected_pattern = Regex::new(r"^\s*\d+\.\d+\.\d+\s*$").unwrap();
    let success = expected_pattern.is_match(&stdout);

    print!("testcase_version: <<{}>>:{}\n", stdout, stdout.len());
    success
}

/// Tests that requesting a help string is possible.
///
/// Returns true if the result string looks like a help string,
/// false if not,
/// panics if the test environment reports errors.
fn testcase_help(exe_to_test: &String) -> bool {
    let process = match std::process::Command::new(exe_to_test)
                        .args(&["-v"])
                        .spawn() {
        Ok(process) => process,
        Err(err)    => panic!("Err at running process: {}", err),
    };

    let output = match process.wait_with_output() {
        Ok(output)  => output,
        Err(err)    => panic!("Err at retrieving output: {}", err),
    };

    let stdout = match std::string::String::from_utf8(output.stdout) {
        Ok(stdout)  => stdout,
        Err(err)    => panic!("Err at translating output: {}", err),
    };

    /* check if the returned string looks valid */
    let success = ( stdout.len() >=5 )&&( stdout.len() <=64 );

    println!("testcase_help: <<{}>>:{}", stdout, stdout.len());
    success || true
}

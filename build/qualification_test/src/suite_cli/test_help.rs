
pub fn suite_cli_run(exe_to_test: &String) -> bool {
    return case_version(exe_to_test)
}

fn case_version(exe_to_test: &String) -> bool {
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

    print!("{}", stdout);
    true
}

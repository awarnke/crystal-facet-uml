pub mod suite_cli;
pub mod suite_gui;
pub mod test_tool;
use suite_cli::test_help::suite_cli_run;
use suite_gui::test_file::suite_gui_run;
use test_tool::test_result::TestResult;

extern crate exitcode;

use std::env;

/* Run this test by calling "cargo run" from the current directory .. */

/// Runs all test suites and returns true if all cases succeeded.
fn run_all_suites(exe_to_test: &String,temp_dir: &String) -> bool {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    result += suite_cli_run(exe_to_test,temp_dir);
    result += suite_gui_run(exe_to_test,temp_dir);

    println!("______________________________");
    println!(
        "TEST RESULT: {}/{} tests failed.",
        result.failed, result.total
    );
    result.failed == 0
}

/// Parses the command line parameters, uses these as test environment input,
/// runs the test cases and returns 0 in case of success.
fn main() {
    let err_code;
    println!("This is the qualification test for crystal-facet-uml.");

    let args: Vec<String> = env::args().collect();

    if args.len() != 3 {
        println!("    please call this program with 2 parameters:");
        println!("    {} EXE_TO_TEST TEMP_DIRECTORY", args[0]);
        err_code = exitcode::USAGE;
    } else {
        println!("{:?}", args);
        err_code = match run_all_suites(&args[1],&args[2]) {
            true => exitcode::OK,
            false => exitcode::SOFTWARE,
        };
    }

    /* When on stable branch, one can replace the exitcode by std::process::ExitCode */
    std::process::exit(err_code)
}

//! This crate performs qualification tests on a binary executable
//! of crystal-facet-uml.
//!
//! # Examples
//!
//! Run the test cases by calling `cargo run` from the current directory.
#![warn(missing_docs)]

pub mod suite_cli;
pub mod suite_gui;
pub mod test_tool;
use suite_cli::suite::SuiteCli;
use suite_gui::suite::SuiteGui;
use test_tool::test_result::TestResult;
use test_tool::test_runner;

extern crate exitcode;

use std::env;

/// Runs all test suites and returns true if all cases succeeded.
fn run_all_suites(exe_to_test: &str, temp_dir: &str) -> bool {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    let suite_1 = SuiteCli::new(exe_to_test, temp_dir);
    result += test_runner::run(&suite_1);
    let suite_2 = SuiteGui::new(exe_to_test, temp_dir);
    result += test_runner::run(&suite_2);

    println!("______________________________");
    println!(
        "TEST RESULT: {}/{} tests failed.",
        result.failed, result.total
    );
    result.failed == 0
}

/// Parses the command line parameters, uses these to configure the test
/// environment, runs the test cases and returns 0 in case of success.
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
        err_code = match run_all_suites(&args[1], &args[2]) {
            true => exitcode::OK,
            false => exitcode::SOFTWARE,
        };
    }

    /* When on stable branch, one can replace the exitcode by std::process::ExitCode */
    std::process::exit(err_code)
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

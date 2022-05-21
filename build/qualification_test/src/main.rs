pub mod suite_cli;
pub mod suite_gui;
use suite_cli::test_help::suite_cli_run;
use suite_gui::test_file::suite_gui_run;

extern crate exitcode;

use std::env;

/* Run this test by calling cargo run from the directory .. */

fn main() {
    let mut err_code = exitcode::OK;
    println!("This is the qualification test for crystal-facet-uml.");

    let args: Vec<String> = env::args().collect();

    if args.len() != 2
    {
        println!("    please call this program with 1 parameter:");
        println!("    {} EXE_TO_TEST", args[0]);
        err_code = exitcode::USAGE;
    }
    else
    {
        println!("{:?}", args);

        err_code = match suite_cli_run(&args[1]) {
                true => exitcode::OK,
                false => exitcode::USAGE,
        };
        err_code = match suite_gui_run(&args[1]) {
                true => exitcode::OK,
                false => exitcode::USAGE,
        };
    }

    /* When on stable branch, one can replace the exitcode by std::process::ExitCode */
    std::process::exit(err_code);
}

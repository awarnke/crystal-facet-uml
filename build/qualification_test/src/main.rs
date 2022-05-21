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

        let command = &args[1];
        let process = match std::process::Command::new(command)
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
    }

    std::process::exit(err_code);
}

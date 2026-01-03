#![warn(missing_docs)]

//! This crate converts c99 code to rust.

mod help;
mod convert;
mod prepare;
use crate::help::print_help;
use crate::convert::run;
use crate::prepare::plan;
use std::env;

/// This is the main function of rusty-shadow.
pub fn main() {
    /* promising: */
    /* https://www.reddit.com/r/rust/comments/11r0ux3/rust_crate_for_reading_and_writing_configuration/ */

    evaluate_params(env::args());
}

fn evaluate_params(mut param_iterator: env::Args) {
    let _zero_argument: Option<String> = param_iterator.next();
    let first_argument: Option<String> = param_iterator.next();
    match first_argument
    {
        None => {
            println!("too few arguments, state -h for help");
        }
        Some(first_arg) => {
            if first_arg == "-h"
            {
                print_help()
            }
            else if first_arg == "-m"
            {
                let second_argument: Option<String> = param_iterator.next();
                match second_argument
                {
                    None => {
                        println!("too few arguments, state -h for help");
                    }
                    Some(second_arg) => {
                        plan::makelist(second_arg.as_str());
                    }
                }
            }
            else if first_arg == "-r"
            {
                let second_argument: Option<String> = param_iterator.next();
                match second_argument
                {
                    None => {
                        println!("too few arguments, state -h for help");
                    }
                    Some(second_arg) => {
                        run::create_rust(second_arg.as_str());
                    }
                }
            }
        }
    }
    let more_arguments: Option<String> = param_iterator.next();
    if more_arguments.is_some()
    {
        println!("too many arguments, state -h for help");
    }
}

/*
 * Copyright 2025-2026 Andreas Warnke
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

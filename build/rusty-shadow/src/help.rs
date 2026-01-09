pub fn print_help() {
    println!("RUST_LOG=info rusty-shadow -h");
    println!("RUST_LOG=info rusty-shadow -m <source_directory>");
    println!("RUST_LOG=info rusty-shadow -r [-c <config_file>] <input_file>...");
    println!("-h:                    help");
    println!("-m <source_directory>: create a Makefile for the c99 source input directory");
    println!("-r <input_file>...:    run the c99 to rust converter on the input files");
    println!("-c <config_file>:      define a configuration file");
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

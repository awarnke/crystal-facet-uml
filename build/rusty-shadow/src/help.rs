use std::env;

pub fn print_help(_param_iterator: env::Args) {
    println!("rusty-shadow [-h] [-i <input_directory>] [-o <output_directory>]");
    println!("-h:                    help");
    println!("-i <input_directory>:  define the input directory");
    println!("-o <output_directory>: define the output directory");
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

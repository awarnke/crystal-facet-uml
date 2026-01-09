#![warn(missing_docs)]

use super::scan_tree::scan_dir;
use super::target_info::TargetInfo;
use std::io;
use log::info;

pub fn makelist<T: io::Write>(source_directory: &str, mut make_output: T) {
    info!("makelist starting...");
    println!("rusty-shadow prepare");
    let targets: Vec<TargetInfo> = scan_dir(source_directory);
    for target in targets
    {
        make_output.write_all(target.name.as_bytes()).expect("A target name could not be written");
        for source_file in target.source_paths
        {
            make_output.write_all(" [".as_bytes()).expect("str to utf8 should work");
            make_output.write_all(source_file.as_bytes()).expect("A target source could not be written");
            make_output.write_all("]".as_bytes()).expect("str to utf8 should work");
        }
        make_output.write_all("\n".as_bytes()).expect("A target source could not be written");
    }
    make_output.flush().expect("The target vec could not be written");
    info!("makelist finished.");
}

/*
 * Copyright 2026-2026 Andreas Warnke
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

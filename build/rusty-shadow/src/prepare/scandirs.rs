use std::io;
use std::fs::read_dir;
use std::fs::ReadDir;
use std::io::Error;

pub fn scan<T: io::Write>(source_directory: &str, _make_output: T) {
    println!("rusty-shadow scan directories");
    println!("-m {}", source_directory);

    let directory_or_not: Result<ReadDir,Error> = read_dir(source_directory);
    match directory_or_not {
        Ok(dir) => {
            for entry_or_not in dir
            {
            let entry = entry_or_not.unwrap();
            let file_name_buf = entry.file_name();
            let file_name = file_name_buf.to_str().unwrap();
            if !file_name.starts_with(".") &&
                entry.file_type().unwrap().is_dir()
                {
                    println!("File {:?} has full path {:?}",
                             file_name, entry.path());
                }
            }
        }
        Err(_err) => {

        }
    }
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

use std::fs::read_dir;
use std::fs::ReadDir;
use std::io::Error;
use log::info;
use super::target_info::TargetInfo;

pub fn scan_dir(source_directory: &str) -> Vec<TargetInfo> {
    println!("rusty-shadow scan directories");
    println!("-m {}", source_directory);
    let mut targets: Vec<TargetInfo> = Vec::new();

    let dir_or_error: Result<ReadDir,Error> = read_dir(source_directory);
    match dir_or_error {
        Ok(dir) => {
            for entry_or_error in dir
            {
                match entry_or_error {
                    Ok(entry) => {
                        let file_name_buf = entry.file_name();
                        let file_name = file_name_buf.to_str().expect("A directory entry has a name");
                        let file_path_obj = entry.path();
                        let file_path: &str = file_path_obj.to_str().expect("A directory entry has a path");
                        if !file_name.starts_with(".")
                        {
                            if entry.file_type().expect("A directory entry has a type").is_dir()
                            {
                                info!("descending into {}",file_path);
                                targets.append( &mut scan_dir(file_path) );
                            }
                            else
                            {
                                info!("file: {}",file_name);
                                let mut target: TargetInfo = {TargetInfo{name:String::from(file_name),source_paths:Vec::new()}};
                                target.source_paths.push(String::from(file_path));
                                targets.push(target);
                            }
                        }
                    }
                    Err(entry_error) => {
                        println!("entry in directory cannot be read: {}",entry_error);
                    }
                }
            }
        }
        Err(dir_error) => {
            println!("directory cannot be read: {}",dir_error);
        }
    }
    targets
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

//! This module implements a TestFixture (test environment) for testing the
//! command line interface of crystal-facet-uml.

use std::fs::File;
use std::io::Write;
use std::path::PathBuf;

/// An enumeration to define access permissions
#[derive(Debug, Copy, Clone, PartialEq)]
pub(super) enum Permissions {
    ReadOnly,
    ReadWrite,
}

/// A test fixture allows to access the test environment;
/// it consists of paths to the executable under test and to a directory to
/// store temporary data
///
/// # Lifetimes
///
/// * `'my_life` refers to the lifetime of a `TestFixture`: This is set up
///   for the duration of executing one test case.
///
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct FixtureCli<'my_life> {
    pub(super) exe_to_test: &'my_life str,
    pub(super) temp_dir: &'my_life str,
}

/// The `FixtureCli` test fixture comes with a constructor
impl<'a, 'b, 'my_life> FixtureCli<'my_life> {
    /// Defines a test fixture (the test environment) for command line tests
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    ///
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> FixtureCli<'my_life>
    where
        'a: 'my_life,
        'b: 'my_life,
    {
        FixtureCli {
            exe_to_test: exe_to_test,
            temp_dir: temp_dir,
        }
    }

    /// Creates a file in the `temp_dir` directory
    ///
    /// # Arguments
    ///
    /// * `self` - The test fixture describing the test environment
    /// * `file_name` - A file name of a file to be created
    /// * `file_contents` - A string that is written to the newly created file as utf-8
    /// * `perm` - Access permissions to apply to the newly created file
    ///
    /// # Panics
    ///
    /// This function panics if the test environment causes errors.
    ///
    pub(super) fn create_file(
        self: &'my_life Self,
        file_name: &'a str,
        file_contents: &'b str,
        perm: Permissions,
    ) -> () {
        /* create the file to import, panic in case an error occured */
        let mut file_path = PathBuf::from(self.temp_dir);
        file_path.push(file_name);
        let file_path_as_string: String = file_path.into_os_string().into_string().unwrap();
        {
            let mut file_handle = File::create(&file_path_as_string).unwrap();
            write!(file_handle, "{}", file_contents).expect("File could not be written");
            let mut file_perms = file_handle.metadata().expect("no metadata").permissions();
            file_perms.set_readonly(perm == Permissions::ReadOnly);
            file_handle
                .set_permissions(file_perms)
                .expect("cannot set read/write flag");
        }
    }
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

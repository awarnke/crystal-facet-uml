/// A test fixture allows to access the test environment;
/// it consists of paths to the executable under test and to a directory to
/// store temporary data
#[derive(Debug, Copy, Clone, PartialEq)]
pub struct FixtureCli<'me> {
    pub(super) exe_to_test: &'me str,
    pub(super) temp_dir: &'me str,
}

/// A test fixture comes with a constructor
impl<'a, 'b, 'me> FixtureCli<'me> {
    /// Defines a test fixture (the test environment) for command line tests
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> FixtureCli<'me>
    where
        'a: 'me,
        'b: 'me,
    {
        FixtureCli {
            exe_to_test: exe_to_test,
            temp_dir: temp_dir,
        }
    }
}

/*
Copyright 2022-2022 Andreas Warnke

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

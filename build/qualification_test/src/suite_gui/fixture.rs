//! This module implements a TestFixture (test environment) for testing the
//! gui of crystal-facet-uml.

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
pub struct FixtureGui<'my_life> {
    pub(super) exe_to_test: &'my_life str,
    pub(super) temp_dir: &'my_life str,
}

/// The `FixtureGui` test fixture comes with a constructor
impl<'a, 'b, 'my_life> FixtureGui<'my_life> {
    /// Defines a test fixture (the test environment) for gui tests
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    ///
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> FixtureGui<'my_life>
    where
        'a: 'my_life,
        'b: 'my_life,
    {
        FixtureGui {
            exe_to_test: exe_to_test,
            temp_dir: temp_dir,
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

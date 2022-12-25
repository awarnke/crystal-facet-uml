/// A test case consists of a name and a run function
pub struct TestCase<'during_run, TestFixture> {
    pub name: &'static str,
    pub run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
}

/// A test case comes with a constructor
impl<'during_run, TestFixture> TestCase<'during_run, TestFixture> {
    /// Defines a test fixture (the test environment)
    ///
    /// # Arguments
    ///
    /// * `name` - Name of the test case
    /// * `run` - Function to call when the test case shall be run
    pub fn new(
        name: &'static str,
        run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
    ) -> TestCase<'during_run, TestFixture> {
        TestCase {
            name: name,
            run: run,
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

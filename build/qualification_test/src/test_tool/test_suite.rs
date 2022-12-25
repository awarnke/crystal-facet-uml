use super::test_case::TestCase;

/// A test suite is an object that provides functions to
/// setup a test fixture (test environment) for each test case,
/// to run each test case and to tear down the test fixtures
pub trait TestSuite<'all_testing, 'during_run, TestFixture>
where
    'all_testing: 'during_run,
{
    fn setup(self: &'all_testing Self) -> &'during_run TestFixture;
    fn teardown(self: &'all_testing Self, environment: &'during_run TestFixture) -> ();
    fn testcases(self: &'all_testing Self) -> &'all_testing [TestCase<'during_run, TestFixture>];
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

use super::fixture::FixtureCli;
use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_import_to_new_cfu1;
use super::test_repair::testcase_repair_new_cfu1;
use crate::test_tool::test_case::TestCase;
use crate::test_tool::test_suite::TestSuite;
use std::fs;

pub struct SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
{
    pub name: &'all_testing str,
    environment: FixtureCli<'during_run>,
    cases: [TestCase<'during_run, FixtureCli<'during_run>>; 4],
}

impl<'all_testing, 'during_run, 'a, 'b> SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
    'a: 'all_testing,
    'b: 'all_testing,
{
    /// Defines all test cases in this test suite
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> SuiteCli<'all_testing, 'during_run> {
        SuiteCli {
            name: "SuiteCli",
            environment: FixtureCli::new(exe_to_test, temp_dir),
            cases: [
                TestCase::new("testcase_version", testcase_version),
                TestCase::<FixtureCli> {
                    name: "testcase_help",
                    run: testcase_help,
                },
                TestCase::<FixtureCli> {
                    name: "testcase_repair_new_cfu1",
                    run: testcase_repair_new_cfu1,
                },
                TestCase::<FixtureCli> {
                    name: "testcase_import_to_new_cfu1",
                    run: testcase_import_to_new_cfu1,
                },
            ],
        }
    }
}

impl<'all_testing, 'during_run> TestSuite<'all_testing, 'during_run, FixtureCli<'during_run>>
    for SuiteCli<'all_testing, 'during_run>
where
    'all_testing: 'during_run,
{
    /// Creates a test directory
    ///
    /// Panics if the test environment reports errors.
    fn setup(self: &'all_testing Self) -> &'during_run FixtureCli<'during_run>
    where
        'all_testing: 'during_run,
    {
        /* create the temp directory, panic in case an error occured */
        fs::create_dir_all(self.environment.temp_dir).unwrap();
        &self.environment
    }

    /// Removes the test directory
    ///
    /// Panics if the test environment reports errors.
    fn teardown(self: &'all_testing Self, environment: &'during_run FixtureCli<'during_run>) -> () {
        /* delete the temp directory in case of successful test result, panic in case an error occured */
        fs::remove_dir_all(environment.temp_dir).unwrap();
    }

    fn testcases(
        self: &'all_testing Self,
    ) -> &'all_testing [TestCase<'during_run, FixtureCli<'during_run>>] {
        &self.cases
    }
}

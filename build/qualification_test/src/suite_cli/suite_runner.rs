use super::fixture::FixtureCli;
use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_import_to_new_cfu1;
use super::test_repair::testcase_repair_new_cfu1;
use crate::test_tool::test_case::TestCase;
use crate::test_tool::test_result::TestResult;
use crate::test_tool::test_suite::TestSuite;
use std::fs;

pub struct SuiteCli<'a> {
    pub name: &'static str,
    environment: FixtureCli<'a>,
}

/// Runs all test cases in this test suite
///
/// # Arguments
///
/// * `exe_to_test` - A path to the executable to be tested
/// * `temp_dir` - A path to a directory that exists and can be used for testing
pub fn suite_cli_run(exe_to_test: &str, temp_dir: &str) -> TestResult {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    result += testcase_version(exe_to_test);
    result += testcase_help(exe_to_test);

    let env: FixtureCli = FixtureCli {
        exe_to_test: exe_to_test,
        temp_dir: temp_dir,
    };
    let suite: &dyn TestSuite<FixtureCli> = &SuiteCli {
        name: "SuiteCli",
        environment: env,
    };

    let env1 = suite.setup();
    result += testcase_repair_new_cfu1(exe_to_test, temp_dir);
    suite.teardown(env1);

    let env2 = suite.setup();
    result += testcase_import_to_new_cfu1(exe_to_test, temp_dir);
    suite.teardown(env2);

    result
}

impl<'b> TestSuite<'b, FixtureCli<'b>> for SuiteCli<'b> {
    /// Creates a test directory
    ///
    /// Panics if the test environment reports errors.
    fn setup(self: &Self) -> FixtureCli<'b> {
        /* create the temp directory, panic in case an error occured */
        fs::create_dir_all(self.environment.temp_dir).unwrap();
        self.environment
    }

    /// Removes the test directory
    ///
    /// Panics if the test environment reports errors.
    fn teardown(self: &Self, environment: FixtureCli<'b>) -> () {
        /* delete the temp directory in case of successful test result, panic in case an error occured */
        fs::remove_dir_all(environment.temp_dir).unwrap();
    }

    fn testcases(self: &Self) -> &'static [TestCase<FixtureCli<'b>>] {
        &[]
    }
}

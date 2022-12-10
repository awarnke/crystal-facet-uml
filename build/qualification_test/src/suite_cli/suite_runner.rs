use super::test_help::testcase_help;
use super::test_help::testcase_version;
use super::test_importexport::testcase_create_cfu1;
use super::test_repair::testcase_repair_cfu1;
use crate::test_tool::test_result::TestResult;

pub fn suite_cli_run(exe_to_test: &str, temp_dir: &str) -> TestResult {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    result += testcase_version(exe_to_test);
    result += testcase_help(exe_to_test);
    result += testcase_repair_cfu1(exe_to_test, temp_dir);
    result += testcase_create_cfu1(exe_to_test, temp_dir);

    result
}

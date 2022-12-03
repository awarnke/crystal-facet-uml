use crate::test_tool::test_result::TestResult;

pub fn suite_gui_run(_exe_to_test: &String) -> TestResult {
    TestResult {
        failed: 0,
        total: 0,
    }
}

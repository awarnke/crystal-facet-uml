use super::test_result::TestResult;
use super::test_suite::TestSuite;

pub fn run(_suite: &dyn TestSuite) -> TestResult {
    TestResult {
        failed: 0,
        total: 0,
    }
}

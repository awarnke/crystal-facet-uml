use super::test_result::TestResult;
use super::test_suite::TestSuite;

pub fn run<'a, TestFixture: 'a>(suite: &dyn TestSuite<'a, TestFixture>) -> TestResult {
    let mut result: TestResult = TestResult {
        failed: 0,
        total: 0,
    };

    for case in suite.testcases() {
        let environment = suite.setup();
        result += (case.run)(&environment);
        suite.teardown(environment);
    }

    result
}

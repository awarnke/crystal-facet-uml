use super::test_result::TestResult;
use super::test_suite::TestSuite;

pub fn run<'all_testing, 'during_run, TestFixture>(
    suite: &'all_testing (dyn TestSuite<'all_testing, 'during_run, TestFixture> + 'all_testing),
) -> TestResult
where
    'all_testing: 'during_run,
{
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

use super::test_case::TestCase;

pub trait TestSuite<'all_testing, 'during_run, TestFixture>
where
    'all_testing: 'during_run,
{
    fn setup(self: &'all_testing Self) -> &'during_run TestFixture;
    fn teardown(self: &'all_testing Self, environment: &'during_run TestFixture) -> ();
    fn testcases(self: &'all_testing Self) -> &'all_testing [TestCase<'during_run, TestFixture>];
}

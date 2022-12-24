use super::test_case::TestCase;

pub trait TestSuite<'a, TestFixture> {
    fn setup(self: &Self) -> TestFixture;
    fn teardown(self: &Self, environment: TestFixture) -> ();
    fn testcases(self: &Self) -> &'a [TestCase<TestFixture>];
}

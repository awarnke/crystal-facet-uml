use super::test_case::TestCase;

pub trait TestSuite {
    fn setup(self: &Self) -> ();
    fn teardown(self: &Self) -> ();
    fn testcases(self: &Self) -> &'static [TestCase];
}

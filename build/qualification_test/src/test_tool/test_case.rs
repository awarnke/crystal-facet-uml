pub struct TestCase<TestFixture> {
    pub name: &'static str,
    pub run: fn(environment: &TestFixture) -> Result<(), ()>,
}

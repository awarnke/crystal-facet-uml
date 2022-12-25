pub struct TestCase<'during_run, TestFixture> {
    pub name: &'static str,
    pub run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
}

impl<'during_run, TestFixture> TestCase<'during_run, TestFixture> {
    /// Defines a test fixture (the test environment)
    ///
    /// # Arguments
    ///
    /// * `name` - Name of the test case
    /// * `run` - Function to call when the test case shall be run
    pub fn new(
        name: &'static str,
        run: fn(environment: &'during_run TestFixture) -> Result<(), ()>,
    ) -> TestCase<'during_run, TestFixture> {
        TestCase {
            name: name,
            run: run,
        }
    }
}

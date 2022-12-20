pub struct TestCase {
    pub name: &'static str,
    pub run: fn() -> Result<(), ()>,
}

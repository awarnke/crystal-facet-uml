#[derive(Debug, Copy, Clone, PartialEq)]
pub struct FixtureCli<'me> {
    pub(super) exe_to_test: &'me str,
    pub(super) temp_dir: &'me str,
}

impl<'a, 'b, 'me> FixtureCli<'me> {
    /// Defines a test fixture (the test environment)
    ///
    /// # Arguments
    ///
    /// * `exe_to_test` - A path to the executable to be tested
    /// * `temp_dir` - A path to a directory that exists and can be used for testing
    pub fn new(exe_to_test: &'a str, temp_dir: &'b str) -> FixtureCli<'me>
    where
        'a: 'me,
        'b: 'me,
    {
        FixtureCli {
            exe_to_test: exe_to_test,
            temp_dir: temp_dir,
        }
    }
}

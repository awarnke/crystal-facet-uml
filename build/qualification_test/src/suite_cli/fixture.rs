#[derive(Debug, Copy, Clone, PartialEq)]
pub struct FixtureCli<'a> {
    pub(super) exe_to_test: &'a str,
    pub(super) temp_dir: &'a str,
}

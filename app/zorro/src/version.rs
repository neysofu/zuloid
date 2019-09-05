/// A [SemVer](https://semver.org/)-compliant version string of the main executable.
pub const VERSION: &str = concat!(
    env!("CARGO_PKG_VERSION"),
    "+",
    env!("CARGO_PKG_COMMIT_HASH"),
    ".",
    env!("CARGO_PKG_BUILD_TIMESTAMP"),
);

#[cfg(test)]
mod test {
    use super::*;
    use semver::Version;

    #[test]
    fn version_is_semver() {
        assert!(Version::parse(VERSION).is_ok());
    }
}

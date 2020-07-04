/// A [SemVer](https://semver.org/)-compliant version string of the main
/// executable.
pub const VERSION: &str = env!("CARGO_PKG_VERSION");

/// A [SemVer](https://semver.org/)-compliant version string of the main
/// executable with associated build metadata.
pub const VERSION_WITH_BUILD_INFO: &str = concat!(
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

    #[test]
    fn version_with_build_info_is_semver() {
        assert!(Version::parse(VERSION_WITH_BUILD_INFO).is_ok());
    }
}

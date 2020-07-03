use chrono::Utc;
use std::process::Command;

fn main() {
    set_rustc_var("CARGO_PKG_COMMIT_HASH", git_commit_hash().as_str());
    set_rustc_var("CARGO_PKG_BUILD_TIMESTAMP", timestamp().as_str());
}

fn set_rustc_var(key: &str, value: &str) {
    println!("cargo:rustc-env={}={}", key, value);
}

fn git_commit_hash() -> String {
    Command::new("git")
        .args(&["rev-parse", "--short=8", "HEAD"])
        .output()
        .map(|p| String::from_utf8_lossy(&p.stdout[..]).to_string())
        .unwrap_or("NOGITERR".to_string())
}

fn timestamp() -> String {
    Utc::now().format("%Y%m%dT%H%M%S").to_string()
}

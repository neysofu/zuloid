use std::str;

pub fn buf_to_str(buffer: &mut Vec<u8>) -> &str {
    buffer.pop();
    str::from_utf8(&buffer[..]).unwrap()
}

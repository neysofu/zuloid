pub fn clear_screen() {
    println!("{}[2J", 27 as char);
}

pub const NOTE_NAMES: [&str; 12] = [
    "C", "CS", "D", "DS", "E", "F", "FS", "G", "GS", "A", "AS", "B",
];
pub const TARGET_TONE_MS: i8 = 10;
pub const VOICES: i8 = 3;
pub const TOTAL_CHUNK_MS: i8 = TARGET_TONE_MS * VOICES;

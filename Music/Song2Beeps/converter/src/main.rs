mod constants;
mod get_audio;
mod use_fft;

use std::fs::File;
use std::io::Write;

use anyhow::Result;

use crate::constants::TARGET_TONE_MS;

fn main() -> Result<()> {
    let (samples, sample_rate) = get_audio::load_mono_normalized("song.mp3")?;

    let notes = use_fft::analyze(&samples, sample_rate);

    let filename = "..\\tones.h";
    let mut file = File::create(filename)?;

    file.write_all(b"#ifndef TONES_H\n")?;
    writeln!(&mut file, "#define TONES_H\n")?;
    writeln!(&mut file, "#include \"notes.h\"\n")?;
    writeln!(&mut file, "#define TARGET_TONE_MS {}\n", TARGET_TONE_MS)?;

    let notes_str = notes.join(", ");
    writeln!(&mut file, "const int tones[] = {{{}}};", notes_str)?;

    writeln!(&mut file, "\n#endif")?;

    println!("Successfully wrote to {}", filename);

    Ok(())
}

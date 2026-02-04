use anyhow::{Context, Result};
use std::fs::File;
use symphonia::core::audio::{AudioBufferRef, Signal};
use symphonia::core::codecs::{CODEC_TYPE_NULL, DecoderOptions};
use symphonia::core::errors::Error;
use symphonia::core::formats::FormatOptions;
use symphonia::core::io::MediaSourceStream;
use symphonia::core::meta::MetadataOptions;
use symphonia::core::probe::Hint;

struct RawAudio {
    samples: Vec<f32>,
    sample_rate: u32,
    channels: usize,
}

fn load_audio(path: &str) -> Result<RawAudio> {
    let file = File::open(path)?;
    let mss = MediaSourceStream::new(Box::new(file), Default::default());

    let mut hint = Hint::new();
    let path = std::path::Path::new(path);
    let ext = path.extension().and_then(|e| e.to_str());
    if let Some(ext) = ext {
        hint.with_extension(ext);
    }

    let probed = symphonia::default::get_probe().format(
        &hint,
        mss,
        &FormatOptions::default(),
        &MetadataOptions::default(),
    )?;

    let mut format = probed.format;

    let track = format
        .tracks()
        .iter()
        .find(|t| t.codec_params.codec != CODEC_TYPE_NULL)
        .context("No audio track found!")?;

    let sample_rate = track
        .codec_params
        .sample_rate
        .context("No sample rate found!")?;

    let channels = track
        .codec_params
        .channels
        .context("No channel info found!")?
        .count();

    let track_id = track.id;

    let mut decoder = symphonia::default::get_codecs()
        .make(&track.codec_params, &DecoderOptions::default())
        .context("Failed to create decoder")?;

    let mut all_samples: Vec<f32> = Vec::new();

    loop {
        let packet = match format.next_packet() {
            Ok(packet) => packet,
            Err(Error::IoError(e)) if e.kind() == std::io::ErrorKind::UnexpectedEof => {
                break;
            }
            Err(e) => return Err(e.into()),
        };

        if packet.track_id() != track_id {
            continue;
        }

        let decoded = match decoder.decode(&packet) {
            Ok(buf) => buf,
            Err(Error::DecodeError(_)) => continue,
            Err(e) => return Err(e.into()),
        };

        match decoded {
            AudioBufferRef::F32(buf) => {
                for frame in 0..buf.frames() {
                    for channel in 0..channels {
                        all_samples.push(buf.chan(channel)[frame]);
                    }
                }
            }
            AudioBufferRef::S16(buf) => {
                for frame in 0..buf.frames() {
                    for channel in 0..channels {
                        // Convert i16 (-32768 to 32767) to f32 (-1.0 to 1.0)
                        let sample = buf.chan(channel)[frame] as f32 / 32768.0;
                        all_samples.push(sample);
                    }
                }
            }
            _ => return Err(anyhow::anyhow!("Unsupported sample format")),
        }
    }

    Ok(RawAudio {
        samples: all_samples,
        sample_rate,
        channels,
    })
}

fn mix_to_mono(raw: RawAudio) -> Vec<f32> {
    if raw.channels == 1 {
        return raw.samples;
    }

    let mut mono = Vec::with_capacity(raw.samples.len() / raw.channels);

    for frame in raw.samples.chunks(raw.channels) {
        let sum: f32 = frame.iter().sum();
        mono.push(sum / raw.channels as f32);
    }

    mono
}

fn normalize(samples: &mut [f32]) {
    let peak = samples.iter().map(|s| s.abs()).fold(0.0f32, f32::max);

    if peak > 0.0 && peak != 1.0 {
        let scale = 1.0 / peak;
        for sample in samples.iter_mut() {
            *sample *= scale;
        }
    }
}

pub fn load_mono_normalized(path: &str) -> Result<(Vec<f32>, u32)> {
    let raw = load_audio(path)?;
    let sample_rate = raw.sample_rate;
    let mut mono = mix_to_mono(raw);
    normalize(&mut mono);
    Ok((mono, sample_rate))
}

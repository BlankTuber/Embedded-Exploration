use std::{collections::HashSet, f32::consts::PI};

use rustfft::{FftPlanner, num_complex::Complex};

use crate::constants::{NOTE_NAMES, TOTAL_CHUNK_MS, VOICES};

fn try_add_note(freq: f32, seen_classes: &mut HashSet<i32>, chunk_notes: &mut Vec<String>) -> bool {
    if freq <= 20.0 {
        return false;
    }

    let midi = (69.0 + 12.0 * (freq / 440.0).log2()).round() as i32;

    if midi > 84 {
        println!("High note detected: {} Hz -> MIDI {}", freq, midi);
    }

    if !(24..=119).contains(&midi) {
        return false;
    }

    let note_class = midi % 12;

    if seen_classes.contains(&note_class) {
        return false;
    }

    seen_classes.insert(note_class);
    let octave = (midi / 12) - 1;
    let note_name = format!("{}{}", NOTE_NAMES[note_class as usize], octave);
    chunk_notes.push(note_name);

    true
}

pub fn analyze(samples: &[f32], sample_rate: u32) -> Vec<String> {
    let chunk_size = (sample_rate as f64 * TOTAL_CHUNK_MS as f64 / 1000.0) as usize;

    let fft_size = (chunk_size * 2).next_power_of_two();

    let mut planner = FftPlanner::<f32>::new();
    let fft = planner.plan_fft_forward(fft_size);

    let hanning: Vec<f32> = (0..chunk_size)
        .map(|i| 0.5 * (1.0 - (2.0 * PI * i as f32 / (chunk_size - 1) as f32).cos()))
        .collect();

    let mut all_notes: Vec<String> = Vec::new();

    for chunk_start in (0..samples.len()).step_by(chunk_size) {
        let chunk_end = (chunk_start + chunk_size).min(samples.len());
        let chunk = &samples[chunk_start..chunk_end];

        if chunk.len() < chunk_size / 2 {
            break;
        }

        let mut buffer: Vec<Complex<f32>> = vec![Complex::new(0.0, 0.0); fft_size];

        for (i, &sample) in chunk.iter().enumerate() {
            buffer[i] = Complex::new(sample * hanning[i], 0.0);
        }

        fft.process(&mut buffer);

        let magnitudes: Vec<f32> = buffer[..fft_size / 2]
            .iter()
            .map(|c| (c.re * c.re + c.im * c.im).sqrt())
            .collect();

        let max_magnitude = magnitudes.iter().cloned().fold(0.0f32, f32::max);

        if max_magnitude < 5.0 {
            for _ in 0..VOICES {
                all_notes.push("S".to_string());
            }
            continue;
        }

        let mut peaks: Vec<(usize, f32)> = magnitudes
            .iter()
            .enumerate()
            .filter(|&(i, _)| {
                let freq = i as f32 * sample_rate as f32 / fft_size as f32;
                (200.0..=1000.0).contains(&freq)
            })
            .map(|(i, &mag)| (i, mag))
            .collect();

        peaks.sort_by(|a, b| b.1.partial_cmp(&a.1).unwrap());

        let mut seen_classes: HashSet<i32> = HashSet::new();
        let mut chunk_notes: Vec<String> = Vec::new();

        for &(bin, mag) in &peaks {
            if mag < max_magnitude * 0.1 || mag < 5.0 {
                break;
            }

            let freq = bin as f32 * sample_rate as f32 / fft_size as f32;
            try_add_note(freq, &mut seen_classes, &mut chunk_notes);

            if chunk_notes.len() >= VOICES as usize {
                break;
            }
        }

        while chunk_notes.len() < VOICES as usize {
            chunk_notes.push("S".to_string());
        }

        all_notes.extend(chunk_notes);
    }

    all_notes
}

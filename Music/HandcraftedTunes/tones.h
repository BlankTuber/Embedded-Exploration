// Octave 4
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

// Octave 5
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 784
#define A5 880
#define B5 988

// Sharps/flats
#define Cs4 277 // C#4 / Db4
#define Ds4 311 // D#4 / Eb4
#define Fs4 370 // F#4 / Gb4
#define Gs4 415 // G#4 / Ab4
#define As4 466 // A#4 / Bb4

// Rest (silence)
#define REST 0

const int tones[] = {
    C4, C4, G4, G4, A4, A4, G4, REST, // "Twinkle twinkle..."
    F4, F4, E4, E4, D4, D4, C4, REST,
};

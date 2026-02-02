// Octave 4
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

// Rest (silence)
#define REST 0

// Quarter note and half note at ~120 BPM
#define Q 500
#define H 1000

const int tones[] = {
    // "Twinkle twinkle little star"
    C4, C4, G4, G4, A4, A4, G4,
    // "How I wonder what you are"
    F4, F4, E4, E4, D4, D4, C4,
    // "Up above the world so high"
    G4, G4, F4, F4, E4, E4, D4,
    // "Like a diamond in the sky"
    G4, G4, F4, F4, E4, E4, D4,
    // "Twinkle twinkle little star"
    C4, C4, G4, G4, A4, A4, G4,
    // "How I wonder what you are"
    F4, F4, E4, E4, D4, D4, C4,
    // Moment of silence
    REST, REST, REST, REST, REST, REST, REST
};

const int durations[] = {
    // "Twinkle twinkle little star"
    Q, Q, Q, Q, Q, Q, H,
    // "How I wonder what you are"
    Q, Q, Q, Q, Q, Q, H,
    // "Up above the world so high"
    Q, Q, Q, Q, Q, Q, H,
    // "Like a diamond in the sky"
    Q, Q, Q, Q, Q, Q, H,
    // "Twinkle twinkle little star"
    Q, Q, Q, Q, Q, Q, H,
    // "How I wonder what you are"
    Q, Q, Q, Q, Q, Q, H,
    // Moment of silence
    Q, Q, Q, Q, Q, Q, Q
};

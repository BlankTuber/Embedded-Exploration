// Example tones.h file

#define SAMPLE_DURATION_MS 30
#define TONE_SLICE_MS 5
#define MAX_TONES 3

const int tones[][MAX_TONES] = {
    {440, 880, 0},   // Two tones
    {523, 659, 784}, // C major chord
    {0, 0, 0},       // Silence
    {440, 0, 0},     // Single tone
};

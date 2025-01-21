#ifndef THUNDER_LIGHTNING_H
#define THUNDER_LIGHTNING_H

#include <Adafruit_NeoPixel.h>

/**
 * Thunder_Lightning class
 *
 * This class creates a thunder lightning effect on a NeoPixel strip.
 */
class Thunder_Lightning
{
private:
  Adafruit_NeoPixel *_strip;

  uint16_t _effectPixel;          // Pixel to flash
  uint8_t _lightningStrikeChance; // Probability of lightning occurring (0-100)
  uint8_t _maxBrightness;         // Maximum brightness of the LEDs

  long _effectDuration;  // Total duration of the lightning effect
  long _effectStartTime; // Start time of the lightning effect

  long _lightningPulseStartTime; // Start time of the current pulse
  long _lightningPulseDuration;  // Duration of a single pulse

  long _glowEndTime; // End time of the glow effect

  long _lightningMinIntensity; // Minimum intensity of the lightning effect
  long _lightningMaxIntensity; // Base intensity of the lightning effect

  bool _lightningActive; // Is a lightning effect active?
  bool _glowActive;      // Is a glow effect active?

  float _intensityMultiplier; // Intensity multiplier for the glow effect

  void applyLightningEffect(); // Helper to flash a single pixel
  void applyGlowEffect();      // Helper to create a glow effect
  void clearEffect();          // Helper to clear the strip

public:
  Thunder_Lightning(Adafruit_NeoPixel *strip, uint8_t lightningStrikeChance = 10, uint8_t maxBrightness = 255);

  void updateLightning();                                       // Call periodically to update the effect
  void setLightningStrikeChance(uint8_t lightningStrikeChance); // Set the probability of lightning occurring
  void setMaxBrightness(uint8_t maxBrightness);                 // Set the maximum brightness of the LEDs
};

#endif

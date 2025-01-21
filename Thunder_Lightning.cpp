#include "Thunder_Lightning.h"

Thunder_Lightning::Thunder_Lightning(Adafruit_NeoPixel *strip, uint8_t lightningStrikeChance, uint8_t maxBrightness)
{
  this->_strip = strip;
  this->_lightningStrikeChance = constrain(lightningStrikeChance, 0, 100);
  this->_maxBrightness = constrain(maxBrightness, 0, 255);
  this->_effectDuration = 0;
  this->_effectStartTime = 0;
  this->_lightningPulseStartTime = 0;
  this->_lightningPulseDuration = 0;
  this->_glowEndTime = 0;
  this->_lightningMinIntensity = 0;
  this->_lightningMaxIntensity = 0;
  this->_lightningActive = false; // No lightning active initially
  this->_glowActive = false;      // No glow active initially
}

void Thunder_Lightning::applyLightningEffect()
{
  uint8_t intensity = constrain(random(_lightningMinIntensity * _intensityMultiplier, _lightningMaxIntensity * _intensityMultiplier), 0, 255); // Randomize brightness

  uint8_t red = intensity;
  uint8_t green = intensity;
  uint8_t blue = intensity;

  if (_intensityMultiplier < 1) // Soft / Medium intensity has warmer tones
  {
    red = constrain(random(intensity * (1 + _intensityMultiplier), intensity * ((1 + _intensityMultiplier) * 1.5)), 0, 255);
    green = constrain(random(intensity, red), 0, 255);
  }

  _strip->setPixelColor(_effectPixel, _strip->Color(red, green, blue));
  _strip->show();
}

// Spread glow effect to adjacent pixels at the end of the lightning strike
void Thunder_Lightning::applyGlowEffect()
{
  int glowIntensity = random(10, 25);
  int startPixel = max(0, _effectPixel - 1);
  int endPixel = min(_strip->numPixels() - 1, _effectPixel + 1);

  for (int i = startPixel; i <= endPixel; i++)
  {
    uint8_t red = glowIntensity;
    uint8_t green = glowIntensity;
    uint8_t blue = glowIntensity + random(5, 20); // Add slight blue tint for glow

    if (_intensityMultiplier < 1) // Soft glow with warm tones
    {
      red += random(5, 10);  // Slight red-orange hue
      green += random(0, 5); // Slight warmth
      blue += random(0, 10); // Keep some blue
    }

    _strip->setPixelColor(i, _strip->Color(red, green, blue));
  }
  _strip->show();
}

// Reset the strip to turn off all LEDs
void Thunder_Lightning::clearEffect()
{
  _strip->clear();
  _strip->show();
}

/**
 * Update the lightning effect. This method should be called in the loop to update the effect.
 *
 * @return void
 */
void Thunder_Lightning::updateLightning()
{
  unsigned long currentTime = millis();

  // Ensure a minimum cooldown between lightning attempts
  static unsigned long lastAttemptTime = 0; // Tracks the last time a strike was attempted
  unsigned long cooldownDuration = 5000;    // Cooldown in milliseconds (e.g., 5 seconds)

  if (!_lightningActive && !_glowActive && (currentTime - lastAttemptTime < cooldownDuration))
  {
    return; // Skip lightning attempts until the cooldown period is over
  }

  if (!_lightningActive && !_glowActive)
  {
    lastAttemptTime = currentTime;

    if (random(0, 100) < _lightningStrikeChance)
    {
      // Determine intensity level with weighted probabilities
      int randomLevel = random(0, 100);
      if (randomLevel < 70)
        _intensityMultiplier = 0.2; // Soft: 70% chance
      else if (randomLevel < 90)
        _intensityMultiplier = 0.5; // Medium: 20% chance
      else
        _intensityMultiplier = 1; // Intense: 10% chance

      _lightningActive = true;
      _effectStartTime = currentTime;
      _effectDuration = random(500, 2000); // Total duration of the lightning strike
      _effectPixel = random(0, _strip->numPixels());

      _lightningMinIntensity = random(0, _maxBrightness / 2);
      _lightningMaxIntensity = random((_maxBrightness / 2) * 1.2, _maxBrightness);

      _lightningPulseStartTime = currentTime;
    }
  }

  if (_lightningActive) // Lightning in progress
  {
    if (currentTime - _effectStartTime < _effectDuration) // Lightning effect duration
    {
      if (currentTime - _lightningPulseStartTime >= _lightningPulseDuration) // New pulse
      {
        _lightningPulseStartTime = currentTime;
        _lightningPulseDuration = random(1, 50); // Random duration between flashes

        // Brief flicker-off between flashes
        if (random(0, 100) < 50) // 50% chance of an off flicker
        {
          _strip->setPixelColor(_effectPixel, _strip->Color(0, 0, 0));
          _strip->show();
        }
        else
        {
          applyLightningEffect(); // Flash the pixel
        }
      }
    }
    else // Lightning effect has ended, maybe activate glow effect
    {
      _lightningActive = false;
      _glowEndTime = currentTime + random(0, 500);
      _glowActive = _glowEndTime > 0; // Start the glow effect
    }
  }

  else if (_glowActive) // Glow effect in progress
  {
    if (currentTime >= _glowEndTime) // Glow effect has ended
    {
      _glowActive = false;
      clearEffect(); // Turn off all LEDs
    }
    else
    {
      applyGlowEffect(); // Spread the glow effect
    }
  }
}

/**
 * Set the probability of lightning occurring
 *
 * @param lightningStrikeChance Probability of lightning occurring (0-100)
 *
 * @return void
 */
void Thunder_Lightning::setLightningStrikeChance(uint8_t lightningStrikeChance)
{
  _lightningStrikeChance = constrain(lightningStrikeChance, 0, 100);
}

/**
 * Set the maximum brightness of the LEDs
 *
 * @param maxBrightness Maximum brightness level (0-255)
 *
 * @return void
 */
void Thunder_Lightning::setMaxBrightness(uint8_t maxBrightness)
{
  _maxBrightness = constrain(maxBrightness, 0, 255);
}
#pragma once

#include <JuceHeader.h>

class LevelMeter : public juce::Component
{
public:
    LevelMeter()
    {
        setOpaque(false);
        colour = juce::Colour(0xffb8b8b8);
    }

    ~LevelMeter() override = default;

    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();
        auto brightness = colour.getBrightness();
        if (std::abs(brightness - 0.5) <= 0.1f)
            brightness = brightness < 0.5 ? 0.3f : 0.7f;

        g.setColour(colour.withAlpha(0.5f).withBrightness((1 - brightness) * 0.5f));

        g.fillRect(bounds);
        g.setColour(colour);

        auto levelInDb = juce::Decibels::gainToDecibels(1 - levelBar, -24.0f);
        auto levelHeight = static_cast<int>(jmap(levelInDb, -24.0f, 0.0f, 0.0f, static_cast<float>(getHeight())));
        // g.fillRect(bounds.removeFromBottom(static_cast<float>(levelHeight)));
        g.fillRect(0, 0, getWidth(), levelHeight);
    }

    void resized() override
    {
    }

    void setLevelBar(float newLevel)
    {
        levelBar = newLevel;
    }

private:
    juce::Colour colour;

    float levelBar = 0.0f;
};

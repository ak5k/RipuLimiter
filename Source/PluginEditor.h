#pragma once

#include <JuceHeader.h>

#include "PluginProcessor.h"
#include "UI.h"

class RipuLimiterAudioProcessorEditor : public juce::AudioProcessorEditor
{
  public:
    RipuLimiterAudioProcessorEditor(RipuLimiterAudioProcessor&);
    ~RipuLimiterAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

  private:
    RipuLimiterAudioProcessor& audioProcessor;
    UI ui;

    juce::OpenGLContext openGLContext;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RipuLimiterAudioProcessorEditor)
};

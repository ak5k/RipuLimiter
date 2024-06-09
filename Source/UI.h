/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.12

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>

#include "LevelMeter.h"
#include "PluginProcessor.h"

//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class UI  : public juce::Component,
            public juce::Timer,
            public juce::Slider::Listener,
            public juce::Button::Listener
{
public:
    //==============================================================================
    UI (RipuLimiterAudioProcessor& p);
    ~UI() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void timerCallback() override
    {
        levelMeter->setLevelBar(audioProcessor.getGainReduction());
        levelMeter->repaint();
    }

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    RipuLimiterAudioProcessor& audioProcessor;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> threshAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> kneeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> holdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> linkAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> oversampleAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cascadeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> deEsserAttachment;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> threshSilder;
    std::unique_ptr<juce::Slider> gainSlider;
    std::unique_ptr<juce::ToggleButton> linkButton;
    std::unique_ptr<LevelMeter> levelMeter;
    std::unique_ptr<juce::Slider> driveSlider;
    std::unique_ptr<juce::ToggleButton> oversampleButton;
    std::unique_ptr<juce::Slider> kneeSlider;
    std::unique_ptr<juce::ToggleButton> cascadeButton;
    std::unique_ptr<juce::Slider> holdSlider;
    std::unique_ptr<juce::Slider> releaseSlider;
    std::unique_ptr<juce::ToggleButton> deessButton;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UI)
};

//[EndFile] You can add extra defines here...
//[/EndFile]


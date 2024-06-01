#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
RipuLimiterAudioProcessorEditor::RipuLimiterAudioProcessorEditor(RipuLimiterAudioProcessor& p)
        : AudioProcessorEditor(&p), audioProcessor(p), ui(p)
{
    openGLContext.attachTo(*this);
    addAndMakeVisible(ui);
    setSize(ui.getBounds().getWidth(), ui.getBounds().getHeight());
}

RipuLimiterAudioProcessorEditor::~RipuLimiterAudioProcessorEditor()
{
    openGLContext.detach();
}

//==============================================================================
void RipuLimiterAudioProcessorEditor::paint(juce::Graphics& g)
{
}

void RipuLimiterAudioProcessorEditor::resized()
{
}

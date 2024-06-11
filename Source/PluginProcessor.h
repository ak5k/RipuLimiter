#pragma once

#include <JuceHeader.h>

#include "Limiter.h"

//==============================================================================
/**
 */
class RipuLimiterAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    RipuLimiterAudioProcessor();

    ~RipuLimiterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;

    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

#endif

    bool supportsDoublePrecisionProcessing() const override
    {
        return true;
    }

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        tempBufferDouble.makeCopyOf(buffer);
        processBlockInternal(tempBufferDouble, midiMessages);
    }

    void processBlock(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override
    {
        processBlockInternal(buffer, midiMessages);
    }

    template<typename T>
    void processBlockInternal(juce::AudioBuffer<T>&, [[maybe_unused]] [[maybe_unused]] juce::MidiBuffer&);

    void processBlockBypassed(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override
    {
        processBlockBypassedInternal(buffer, midiMessages);
    }

    void processBlockBypassed(juce::AudioBuffer<double>& buffer, juce::MidiBuffer& midiMessages) override
    {
        processBlockBypassedInternal(buffer, midiMessages);
    }

    template<typename T>
    void processBlockBypassedInternal(juce::AudioBuffer<T>& buffer,
            [[maybe_unused]] [[maybe_unused]] juce::MidiBuffer& midiMessages);

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;

    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;

    bool producesMidi() const override;

    bool isMidiEffect() const override;

    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;

    int getCurrentProgram() override;

    void setCurrentProgram(int index) override;

    const juce::String getProgramName(int index) override;

    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;

    void setStateInformation(const void* data, int sizeInBytes) override;

    float getGainReduction() const
    {
        return gainReduction.load(std::memory_order_acquire);
    }

    auto& getAPVTS()
    {
        return apvts;
    }

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioBuffer<double> tempBufferDouble;

    std::atomic<float> gainReduction;

    std::vector<juce::LinearSmoothedValue<float>> thresholdSmoothed;
    std::vector<juce::LinearSmoothedValue<float>> gainSmoothed;
    std::vector<juce::LinearSmoothedValue<float>> driveSmoothed;
    std::vector<juce::LinearSmoothedValue<float>> kneeSmoothed;
    std::vector<juce::LinearSmoothedValue<float>> holdSmoothed;
    std::vector<juce::LinearSmoothedValue<float>> releaseSmoothed;

    juce::dsp::DelayLine<double, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    std::unique_ptr<juce::dsp::Oversampling<double>> oversampling;

    std::vector<LimiterAttackHoldRelease> limiters;
    juce::AudioProcessorValueTreeState apvts;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RipuLimiterAudioProcessor)
};

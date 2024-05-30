#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RipuLimiterAudioProcessor::RipuLimiterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
#endif
    , apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
{
}

RipuLimiterAudioProcessor::~RipuLimiterAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout RipuLimiterAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" dB");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "thresh", "Threshold", juce::NormalisableRange<float>(-24.0f, 0.0f, 0.1f), -0.1f, attributes
        ));
    }

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" dB");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "gain", "Gain", juce::NormalisableRange<float>(-24.0f, 0.0f, 0.1f), -0.1f, attributes
        ));
    }

    params.push_back(std::make_unique<juce::AudioParameterBool>("link", "Link", false));

    return {params.begin(), params.end()};
}

//==============================================================================
const juce::String RipuLimiterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RipuLimiterAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool RipuLimiterAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool RipuLimiterAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double RipuLimiterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RipuLimiterAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int RipuLimiterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RipuLimiterAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String RipuLimiterAudioProcessor::getProgramName(int index)
{
    return {};
}

void RipuLimiterAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void RipuLimiterAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    auto numChannels = getTotalNumInputChannels();
    if (numChannels == 0)
        return;

    limiters.resize(numChannels);
    for (auto& limiter : limiters)
        limiter.configure(sampleRate);

    thresholdSmoothed.resize(numChannels);
    gainSmoothed.resize(numChannels);

    for (auto& i : thresholdSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : gainSmoothed)
        i.reset(sampleRate, 0.1);

    setLatencySamples(limiters[0].latencySamples());

    delayLine.prepare({sampleRate, (uint32_t)limiters[0].latencySamples(), (uint32_t)numChannels});
}

void RipuLimiterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RipuLimiterAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

template <typename T>
void RipuLimiterAudioProcessor::processBlockInternal(juce::AudioBuffer<T>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    gainReduction.store(1.0f, std::memory_order_release);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto threshold = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("thresh")->load());
    auto gain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("gain")->load());
    bool isLinked = (bool)apvts.getRawParameterValue("link")->load();

    for (auto& i : thresholdSmoothed)
        i.setTargetValue(threshold);
    for (auto& i : gainSmoothed)
        i.setTargetValue(gain);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            delayLine.pushSample(channelData[sample], channel);
            (void)delayLine.popSample(channel);

            threshold = thresholdSmoothed[channel].getNextValue();
            gain = gainSmoothed[channel].getNextValue();
            limiters[channel].setThreshold(threshold);

            auto newSample = channelData[sample];
            auto val = static_cast<T>(limiters[channel].sample(newSample));
            if (!isLinked)
            {
                val *= 1 / threshold;
                val *= gain;
            }

            channelData[sample] = val;

            auto gr = limiters[channel].gainReduction;
            if ((float)gr < gainReduction.load(std::memory_order_acquire))
                gainReduction.store((float)gr, std::memory_order_release);
        }
    }
}

template <typename T>
void RipuLimiterAudioProcessor::processBlockBypassedInternal(
    juce::AudioBuffer<T>& buffer, juce::MidiBuffer& midiMessages
)
{
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            delayLine.pushSample(static_cast<T>(channelData[sample]), channel);
            channelData[sample] = static_cast<T>(delayLine.popSample(channel));
        }
    }
}

//==============================================================================
bool RipuLimiterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* RipuLimiterAudioProcessor::createEditor()
{
    return new RipuLimiterAudioProcessorEditor(*this);
}

//==============================================================================
void RipuLimiterAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void RipuLimiterAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RipuLimiterAudioProcessor();
}

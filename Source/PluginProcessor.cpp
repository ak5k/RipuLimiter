#include "PluginProcessor.h"

#include "PluginEditor.h"
#include "SoftClipper.h"

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

RipuLimiterAudioProcessor::~RipuLimiterAudioProcessor() = default;

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
            "gain", "Gain", juce::NormalisableRange<float>(-24.0f, 0.0f, 0.1f), -1.0f, attributes
        ));
    }

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" dB");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "drive", "Drive", juce::NormalisableRange<float>(0.0f, 24.0f, 0.1f), 0.0f, attributes
        ));
    }

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" dB");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "knee", "Knee", juce::NormalisableRange<float>(0.0f, 6.0f, 0.1f), 0.0f, attributes
        ));
    }

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" ms");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "hold", "Hold", juce::NormalisableRange<float>(0.0f, 20.0f, 0.1f), 1.0f, attributes
        ));
    }

    {
        auto attributes = AudioParameterFloatAttributes().withLabel(" ms");
        params.push_back(std::make_unique<AudioParameterFloat>(
            "release", "Release", juce::NormalisableRange<float>(5.0f, 300.0f, 0.1f, 0.38f), 5.0f, attributes
        ));
    }

    params.push_back(std::make_unique<juce::AudioParameterBool>("link", "Link", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("oversample", "Oversample", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("cascade", "Cascade", true));
    params.push_back(std::make_unique<juce::AudioParameterBool>("deEsser", "De-Esser", false));

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
    driveSmoothed.resize(numChannels);
    kneeSmoothed.resize(numChannels);
    holdSmoothed.resize(numChannels);
    releaseSmoothed.resize(numChannels);

    for (auto& i : thresholdSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : gainSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : driveSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : kneeSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : holdSmoothed)
        i.reset(sampleRate, 0.1);
    for (auto& i : releaseSmoothed)
        i.reset(sampleRate, 0.1);

    oversampling = std::make_unique<juce::dsp::Oversampling<double>>(
        numChannels, 4, juce::dsp::Oversampling<double>::filterHalfBandPolyphaseIIR, true
    );
    oversampling->initProcessing(samplesPerBlock);

    tempBufferDouble.setSize(numChannels, samplesPerBlock);

    delayLine.prepare(
        {sampleRate,
         (uint32_t)(limiters[0].latencySamples() + oversampling->getLatencyInSamples()),
         (uint32_t)numChannels}
    );

    bool isOversampled = (bool)apvts.getRawParameterValue("oversample")->load();
    if (isOversampled)
        setLatencySamples((int)(limiters[0].latencySamples() + oversampling->getLatencyInSamples()));
    else
        setLatencySamples(limiters[0].latencySamples());

    delayLine.setMaximumDelayInSamples(getLatencySamples());
    delayLine.setDelay(getLatencySamples());
    delayLine.reset();
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
void RipuLimiterAudioProcessor::processBlockInternal(
    juce::AudioBuffer<T>& bufferIn, [[maybe_unused]] juce::MidiBuffer& midiMessages
)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        bufferIn.clear(i, 0, bufferIn.getNumSamples());

    auto threshold = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("thresh")->load());
    auto gain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("gain")->load());
    auto drive = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("drive")->load());

    auto knee = apvts.getRawParameterValue("knee")->load();
    auto hold = apvts.getRawParameterValue("hold")->load();
    auto release = apvts.getRawParameterValue("release")->load();

    bool isLinked = (bool)apvts.getRawParameterValue("link")->load();
    bool isOversampled = (bool)apvts.getRawParameterValue("oversample")->load();
    bool cascade = (bool)apvts.getRawParameterValue("cascade")->load();
    bool deEss = (bool)apvts.getRawParameterValue("deEsser")->load();
    for (auto& limiter : limiters)
    {
        limiter.setCascade(cascade);
        limiter.setDeEsser(deEss);
    }

    for (auto& i : thresholdSmoothed)
        i.setTargetValue(threshold);
    for (auto& i : gainSmoothed)
        i.setTargetValue(gain);
    for (auto& i : driveSmoothed)
        i.setTargetValue(drive);
    for (auto& i : kneeSmoothed)
        i.setTargetValue(knee);
    for (auto& i : holdSmoothed)
        i.setTargetValue(hold);
    for (auto& i : releaseSmoothed)
        i.setTargetValue(release);

    juce::dsp::AudioBlock<T> block(bufferIn);
    juce::dsp::AudioBlock<T> oversampledBlock;

    bool doSaturation = !approximatelyEqual(drive, 1.0f);

    bool doOverSample = isOversampled && doSaturation;

    if (doOverSample)
        oversampledBlock = oversampling->processSamplesUp(block);
    else
        oversampledBlock = block;

    if (doSaturation)
        for (auto channel = 0; channel < oversampledBlock.getNumChannels(); channel++)
        {
            auto* channelData = oversampledBlock.getChannelPointer(channel);
            for (auto sample = 0; sample < oversampledBlock.getNumSamples(); sample++)
            {
                drive = driveSmoothed[channel].getNextValue();
                knee = kneeSmoothed[channel].getNextValue();
                channelData[sample] = SoftClipper(channelData[sample], (T)drive, (T)knee);
            }
        }

    if (doOverSample)
    {
        oversampling->processSamplesDown(block);
        setLatencySamples((int)(limiters[0].latencySamples() + oversampling->getLatencyInSamples()));
    }
    else
    {
        setLatencySamples(limiters[0].latencySamples());
    }

    if ((int)delayLine.getDelay() != getLatencySamples())
    {
        if (delayLine.getMaximumDelayInSamples() < getLatencySamples())
            delayLine.setMaximumDelayInSamples(getLatencySamples());
        delayLine.setDelay(getLatencySamples());
        delayLine.reset();
    }

    for (int channel = 0; channel < block.getNumChannels(); ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
        {
            delayLine.pushSample(channel, static_cast<double>(channelData[sample]));
            (void)delayLine.popSample(channel);
            threshold = thresholdSmoothed[channel].getNextValue();
            gain = gainSmoothed[channel].getNextValue();
            drive = driveSmoothed[channel].getNextValue();
            hold = holdSmoothed[channel].getNextValue();
            release = releaseSmoothed[channel].getNextValue();

            limiters[channel].setThreshold(threshold);
            limiters[channel].setHoldMs(hold);
            limiters[channel].setReleaseMs(release);

            auto newSample = channelData[sample];

            auto val = static_cast<T>(limiters[channel].sample(newSample));
            if (!isLinked)
            {
                val *= 1 / threshold;
                val *= gain;
            }

            channelData[sample] = val;

            auto gr = limiters[channel].gainReduction;
            gainReduction.store((float)gr, std::memory_order_release);
        }
    }
}

template <typename T>
void RipuLimiterAudioProcessor::processBlockBypassedInternal(
    juce::AudioBuffer<T>& buffer, [[maybe_unused]] juce::MidiBuffer& midiMessages
)
{
    for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            delayLine.pushSample(channel, static_cast<double>(channelData[sample]));
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

    if (xmlState != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RipuLimiterAudioProcessor();
}

#pragma once

#include <JuceHeader.h>

#include <signalsmith/dsp/delay.h>
#include <signalsmith/dsp/envelopes.h>

struct ExponentialRelease
{
    double releaseSlew;
    double output = 1;
    int releaseSamples = 0;

    ExponentialRelease() = default;

    ExponentialRelease(double releaseSamples)
    {
        // The exact value is `1 - exp(-1/releaseSamples)`
        // but this is a decent approximation
        // releaseSlew = (1 / (releaseSamples + 1));
        setReleaseSlew(releaseSamples);
    }

    void setReleaseSlew(int newReleaseSamples)
    {
        releaseSamples = newReleaseSamples;
        releaseSlew = (1 / (releaseSamples + 1.0));
    }

    double step(double input)
    {
        // Move towards input
        output += (input - output) * releaseSlew;
        output = std::min(output, input);
        return output;
    }
};

struct LimiterAttackHoldRelease
{
    std::array<ExponentialRelease, 8> releases; // array of 8 ExponentialRelease objects
    bool cascade = true;

    double limit = 1;
    double attackMs = 4 / 3.0;
    double holdMs = 20;
    double releaseMs = 300;

    double sampleRate = 44100;

    double gainReduction = 1;

    signalsmith::envelopes::PeakHold<double> peakHold{1};
    signalsmith::envelopes::BoxStackFilter<double> smoother{1};
    // We don't need fractional delays, so this could be nearest-sample
    // signalsmith::delay::Delay<double> delay;

    juce::dsp::DelayLine<double, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;

    // ExponentialRelease release; // see the previous example code

    int attackSamples = 0;

    LimiterAttackHoldRelease() = default;

    // set threshold in linear gain
    void setThreshold(double newLimit)
    {
        this->limit = newLimit;
    }

    void configure(double newSampleRate)
    {
        sampleRate = newSampleRate;
        attackSamples = (int)(attackMs * 0.001 * sampleRate);

        int holdSamples = (int)(holdMs * 0.001 * sampleRate);
        double releaseSamples = releaseMs * 0.001 * sampleRate;
        // release = ExponentialRelease(releaseSamples);
        for (auto& release : releases)
            release = ExponentialRelease(releaseSamples);

        peakHold.resize(attackSamples + holdSamples);
        smoother.resize(attackSamples, 3);
        smoother.reset(1);

        delayLine.prepare({sampleRate, (uint32_t)attackSamples, 1});
        delayLine.setMaximumDelayInSamples(attackSamples);
        delayLine.setDelay(attackSamples);
    }

    int latencySamples() const
    {
        return attackSamples;
    }

    void setHoldMs(double newHoldMs)
    {
        if (approximatelyEqual(holdMs, newHoldMs))
            return;

        holdMs = newHoldMs;
        int holdSamples = (int)(holdMs * 0.001 * sampleRate);
        peakHold.resize(attackSamples + holdSamples);
    }

    void setReleaseMs(double newReleaseMs)
    {
        releaseMs = newReleaseMs;
        double releaseSamples = releaseMs * 0.001 * sampleRate;
        // release = ExponentialRelease(releaseSamples);
        for (auto& release : releases)
            if (release.releaseSamples != (int)releaseSamples)
                release.setReleaseSlew(releaseSamples);
    }

    void setCascade(bool val)
    {
        cascade = val;
    }

    double gain(double v)
    {
        double maxGain = 1;
        if (std::abs(v) > limit)
            maxGain = limit / std::abs(v);
        double movingMin = -peakHold(-maxGain);
        // double releaseEnvelope = release.step(movingMin);

        int n = cascade ? (int)releases.size() : 1;
        // for (auto& release : releases)
        for (int i = 0; i < n; ++i)
            movingMin = releases[i].step(movingMin);

        double releaseEnvelope = movingMin;
        // return releaseEnvelope;
        return smoother(releaseEnvelope);
    }

    double sample(double v)
    {
        double delayedV = 0;

        // delayedV = delay.read(attackSamples - 1);
        // delay.write(v);

        delayLine.pushSample(0, v);
        delayedV = delayLine.popSample(0);

        double g = gain(v);

        gainReduction = g;

        return delayedV * g;
    }
};
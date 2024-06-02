#pragma once

#include <JuceHeader.h>

#include <cmath>

// #define MAGIC_NUMBER 2.08136898
// #define MAGIC_NUMBER std::log2(std::exp(1.0)) * std::log2(std::exp(1.0))

template <typename T>
static T sign(T num)
{
    if (num > 0)
        return 1;
    if (num < 0)
        return -1;
    return 0;
}

template <typename T>
T SoftClipper(T x, T drive, T knee)
{
    // x *= drive;
    // if (x > 1)
    //     x = 1;
    // else if (x < -1)
    //     x = -1;
    // else
    //     x = x - (x * x * x) / 3;

    // x /= drive;
    // return x;
    constexpr T MAGIC_NUMBER = 2.081368981005608;
    // constexpr T MAGIC_NUMBER = 1;

    auto slider1 = -drive;
    auto slider2 = -drive;

    auto threshdb = slider1;
    auto ceiling = juce::Decibels::decibelsToGain(slider2);
    auto ceildb = slider2;
    auto makeup = juce::Decibels::decibelsToGain(ceildb - threshdb);

    auto sc = -knee + ceildb;
    auto scv = juce::Decibels::decibelsToGain(sc);
    auto peakdb = ceildb + 25;
    auto scmult = std::abs((ceildb - sc) / (peakdb - sc));

    auto spl0 = x * makeup;
    auto sign0 = sign(spl0);
    auto abs0 = std::abs(spl0);
    auto overdb0 = juce::Decibels::gainToDecibels(MAGIC_NUMBER * std::log(abs0)) - ceildb;

    if (!(approximatelyEqual(knee, (T)0)) && abs0 > scv)
        spl0 =
            sign0 * static_cast<T>(
                        juce::Decibels::decibelsToGain(juce::dsp::FastMathApproximations::exp(overdb0 * scmult) + scv)
                    );

    spl0 = std::min(ceiling, std::abs(spl0)) * sign(spl0);
    return spl0;
}

#pragma once

#include <JuceHeader.h>

#include <cmath>

// #define MAGIC_NUMBER 2.08136898
#define MAGIC_NUMBER std::log2(std::exp(1.0)) * std::log2(std::exp(1.0))

template<typename T>
static T sign(T num)
{
    if (num > 0)
        return 1;
    if (num < 0)
        return -1;
    return 0;
}

template<typename T>
T SoftClipper(T x, T drive, T knee)
{
    // x *= drive;
    // if (x < -1)
    //     return -1 / drive;

    // if (x > 1)
    //     return 1 / drive;

    auto slider1 = -drive;
    auto slider2 = -drive;

    // auto log2db = 8.6858896380650365530225783783321;  // 20 / ln(10)
    // auto db2log = 0.11512925464970228420089957273422; // ln(10) / 20

    // thresh = exp(slider1 * db2log);
//    auto thresh = juce::Decibels::decibelsToGain(slider1);
    auto threshdb = slider1;
    // ceiling = exp(slider2 * db2log);
    auto ceiling = juce::Decibels::decibelsToGain(slider2);
    auto ceildb = slider2;
    // makeup = exp((ceildb - threshdb) * db2log);
    auto makeup = juce::Decibels::decibelsToGain(ceildb - threshdb);
//    auto makeupdb = ceildb - threshdb;

    // soft knee
    auto sc = -knee + ceildb;
    // scv = exp(sc * db2log);
    auto scv = juce::Decibels::decibelsToGain(sc);
    // sccomp = exp(-sc * db2log);
    // auto sccomp = juce::Decibels::decibelsToGain(-sc);
    auto peakdb = ceildb + 25;
    // peaklvl = exp(peakdb * db2log);
    // auto peaklvl = juce::Decibels::decibelsToGain(peakdb);
    // scratio = slider4;
    auto scmult = std::abs((ceildb - sc) / (peakdb - sc));

    auto spl0 = x;
//    auto peak = std::abs(spl0);
    spl0 = spl0 * makeup;
    auto sign0 = sign(spl0);
    auto abs0 = std::abs(spl0);
    // auto overdb0 = 2.08136898 * log(abs0) * log2db - ceildb;
    auto overdb0 = juce::Decibels::gainToDecibels(MAGIC_NUMBER * std::log(abs0)) - ceildb;
    // abs0 > scv ? (spl0 = sign0 * (scv + exp(overdb0 * scmult) * db2log););
    if (!(approximatelyEqual(knee, (T)0)) && abs0 > scv)
        spl0 =
                sign0 * static_cast<T>(
                        juce::Decibels::decibelsToGain(juce::dsp::FastMathApproximations::exp(overdb0 * scmult) + scv)
                );

    spl0 = std::min(ceiling, std::abs(spl0)) * sign(spl0);
    return spl0;
}
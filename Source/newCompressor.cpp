/*
  ==============================================================================

    newCompressor.cpp
    Created: 30 Dec 2021 11:08:25am
    Author:  bmour

  ==============================================================================
*/

#include "newCompressor.h"

template <typename SampleType>
newCompressor<SampleType>::newCompressor()
{
    update();
}

//==============================================================================
template <typename SampleType>
void newCompressor<SampleType>::setThreshold(SampleType newThreshold)
{
    thresholddB = newThreshold;
    update();
}

template <typename SampleType>
void newCompressor<SampleType>::setRatio(SampleType newRatio)
{
    jassert(newRatio >= static_cast<SampleType> (1.0));

    ratio = newRatio;
    update();
}

template <typename SampleType>
void newCompressor<SampleType>::setAttack(SampleType newAttack)
{
    attackTime = newAttack;
    update();
}

template <typename SampleType>
void newCompressor<SampleType>::setKneeWidth(SampleType newKnee)
{
    kneeWidth = newKnee;
    update();
}

template <typename SampleType>
void newCompressor<SampleType>::setRelease(SampleType newRelease)
{
    releaseTime = newRelease;
    update();
}

//==============================================================================
template <typename SampleType>
void newCompressor<SampleType>::prepare(const juce::dsp::ProcessSpec& spec)
{
    jassert(spec.sampleRate > 0);
    jassert(spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    envelopeFilter.prepare(spec);

    update();
    reset();
}

template <typename SampleType>
void newCompressor<SampleType>::reset()
{
    envelopeFilter.reset();
}

//==============================================================================
template <typename SampleType>
SampleType newCompressor<SampleType>::processSample(int channel, SampleType inputValue)
{
    // Ballistics filter with peak rectifier
    auto env = envelopeFilter.processSample(channel, inputValue);

    float x_db = juce::Decibels::gainToDecibels(env);

    if (x_db > (thresholddB + kneeWidth / 2)) {
        // Above knee curve
        g_sc = thresholddB + ((x_db - thresholddB) / ratio);
    }
    else if (x_db > (thresholddB - kneeWidth / 2)) {
        // Within knee curve
        g_sc = x_db + (((1 / ratio - 1) * (std::pow((x_db - thresholddB + kneeWidth / 2), 2))) / (2 * kneeWidth));
    }
    else {
        // Do not compress
        g_sc = x_db;
    }

    auto gainAlpha = g_sc - x_db;

    auto linA = std::pow(10,(gainAlpha / 20));

    return linA * inputValue;
}

template <typename SampleType>
SampleType newCompressor<SampleType>::processSampleStereo(float env, float x_max)
{
    float x_db = juce::Decibels::gainToDecibels(env);

    if (x_db > (thresholddB + kneeWidth / 2)) {
        // Above knee curve
        g_sc = thresholddB + ((x_db - thresholddB) / ratio);
    }
    else if (x_db > (thresholddB - kneeWidth / 2)) {
        // Within knee curve
        g_sc = x_db + (((1 / ratio - 1) * (std::pow((x_db - thresholddB + kneeWidth / 2), 2))) / (2 * kneeWidth));
    }
    else {
        // Do not compress
        g_sc = x_db;
    }

    auto gainAlpha = g_sc - x_db;

    auto linA = std::pow(10, (gainAlpha / 20));

    return linA * x_max;
}

template <typename SampleType>
void newCompressor<SampleType>::update()
{
    threshold = Decibels::decibelsToGain(thresholddB, static_cast<SampleType> (-200.0));
    thresholdInverse = static_cast<SampleType> (1.0) / threshold;
    ratioInverse = static_cast<SampleType> (1.0) / ratio;

    envelopeFilter.setAttackTime(attackTime);
    envelopeFilter.setReleaseTime(releaseTime);
}

//==============================================================================
template class newCompressor<float>;
template class newCompressor<double>;
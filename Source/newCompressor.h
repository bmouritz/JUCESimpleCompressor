/*
  ==============================================================================

    newCompressor.h
    Created: 30 Dec 2021 11:08:25am
    Author:  bmour

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename SampleType>
class newCompressor
{
public:
    //==============================================================================
    /** Constructor. */
    newCompressor();

    //==============================================================================
    /** Sets the threshold in dB of the compressor.*/
    void setThreshold(SampleType newThreshold);

    /** Sets the ratio of the compressor (must be higher or equal to 1).*/
    void setRatio(SampleType newRatio);

    /** Sets the attack time in milliseconds of the compressor.*/
    void setAttack(SampleType newAttack);

    /** Sets the release time in milliseconds of the compressor.*/
    void setRelease(SampleType newRelease);

    /** Sets the width of the knee of the compressor.*/
    void setKneeWidth(SampleType newKnee);

    //==============================================================================
    /** Initialises the processor. */
    void prepare(const juce::dsp::ProcessSpec& spec);

    /** Resets the internal state variables of the processor. */
    void reset();

    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        jassert(inputBlock.getNumChannels() == numChannels);
        jassert(inputBlock.getNumSamples() == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom(inputBlock);
            return;
        }

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inputBlock.getChannelPointer(channel);
            auto* outputSamples = outputBlock.getChannelPointer(channel);

            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = processSample((int)channel, inputSamples[i]);
        }
    }
    /** Performs the processing operation on a single sample at a time. */
    SampleType processSample(int channel, SampleType inputValue);

    template <typename ProcessContext>
    void processStereo(const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        jassert(inputBlock.getNumChannels() == numChannels);
        jassert(inputBlock.getNumSamples() == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom(inputBlock);
            return;
        }

        // process samples with gain amount
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples = inputBlock.getChannelPointer(channel);
            auto* outputSamples = outputBlock.getChannelPointer(channel);

            for (size_t i = 0; i < numSamples; ++i) {
                float x_max = jmax(std::abs(inputBlock.getSample(0, i)), std::abs(inputBlock.getSample(1, i)));
                float env = envelopeFilter.processSample(channel, x_max);
                outputSamples[i] = processSampleStereo(env, inputSamples[i]);
            }
        }
    }

    /** Performs the processing operation on a single sample at a time. */
    SampleType processSampleStereo(float env, SampleType inputSample);

private:
    //==============================================================================
    void update();

    //==============================================================================
    SampleType threshold, thresholdInverse, ratioInverse;
    juce::dsp::BallisticsFilter<SampleType> envelopeFilter;

    double sampleRate = 44100.0;
    double kneeWidth = 0.0;
    float g_sc = 0.0f;
    SampleType thresholddB = 0.0, ratio = 1.0, attackTime = 1.0, releaseTime = 100.0;
};
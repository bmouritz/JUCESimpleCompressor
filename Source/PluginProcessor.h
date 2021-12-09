/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class CompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    CompressorAudioProcessor();
    ~CompressorAudioProcessor() override;
    //===========================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void setFilteringEnbaled(const bool shouldBeEnabled)
    {
        filteringEnabled = shouldBeEnabled;
    }

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    AudioProcessorValueTreeState& getState();

    void process(dsp::ProcessContextReplacing<float> context);
    void updateParameters();

private:
    bool filteringEnabled = false;

    dsp::Compressor<float> compressor;

    // setKnee in compressor class by /2 to be +- threshold
    // 
    
    // if DB > threshold + KneeWidth / 2 => compress
    // if DB > threshold - KneeWidth / 2 => (1/Ratio - 1) * (compression + KneeWidth /2)^2 / 2*KneeWidth 
    // else no compression



    ScopedPointer<AudioProcessorValueTreeState> state;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessor)
};

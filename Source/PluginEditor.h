/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CompressorAudioProcessorEditor (CompressorAudioProcessor&);
    ~CompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    std::unique_ptr<Slider> attackKnob;
    std::unique_ptr<Slider> releaseKnob;
    std::unique_ptr<Slider> ratioKnob;
    std::unique_ptr<Slider> thresholdKnob;
    std::unique_ptr<Slider> gainKnob;
//    std::unique_ptr<TextButton> onOffBtn;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
 //   std::unique_ptr<AudioProcessorValueTreeState::ButtonAttachment> onOffBtnAttachment;

    CompressorAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};

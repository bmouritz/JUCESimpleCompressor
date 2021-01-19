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
class CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
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
    ScopedPointer<Slider> attackKnob;
    ScopedPointer<Slider> releaseKnob;
    ScopedPointer<Slider> ratioKnob;
    ScopedPointer<Slider> thresholdKnob;
    ScopedPointer<Slider> gainKnob;
    ScopedPointer<TextButton> onOffBtn;
    
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    ScopedPointer<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
 //   ScopedPointer<AudioProcessorValueTreeState::ButtonAttachment> onOffBtnAttachment;

    CompressorAudioProcessor& audioProcessor;
    void buttonClicked(Button* buttonThatWasClicked) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};

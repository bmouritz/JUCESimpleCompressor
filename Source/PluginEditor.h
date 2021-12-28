/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#pragma once

#include "myLookAndFeel.h"
#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class CompressorAudioProcessorEditor  : public juce::AudioProcessorEditor, private Button::Listener
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
    std::unique_ptr<Slider> kneeKnob;
    myLookAndFeelV1 myLookAndFeelV1;
    TextButton On;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> ratioAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> thresholdAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> kneeAttachment;

    CompressorAudioProcessor& audioProcessor;

    OpenGLContext openGLContext;

    void buttonClicked(Button* buttonThatWasClicked) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};

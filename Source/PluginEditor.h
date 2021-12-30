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

struct CustomRotarySlider : Slider {
    CustomRotarySlider() : Slider(Slider::SliderStyle::RotaryHorizontalVerticalDrag, Slider::TextEntryBoxPosition::NoTextBox) {}
};


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
    CompressorAudioProcessor& audioProcessor;
    OpenGLContext openGLContext;

    using APVTS = AudioProcessorValueTreeState;
    using Attachment = APVTS::SliderAttachment;

    Attachment attackAttachment, releaseAttachment, ratioAttachment, thresholdAttachment, kneeAttachment;
    CustomRotarySlider attackSlider, releaseSlider, ratioSlider, thresholdSlider, kneeSlider;
    myLookAndFeelV1 myLookAndFeelV1;
    TextButton StereoBtn;

    void buttonClicked(Button* buttonThatWasClicked) override;
    void addVisual();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorAudioProcessorEditor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "myLookAndFeel.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
    attackAttachment(audioProcessor.apvts, "Attack", attackSlider),
    releaseAttachment(audioProcessor.apvts, "Release", releaseSlider),
    ratioAttachment(audioProcessor.apvts, "Ratio", ratioSlider),
    thresholdAttachment(audioProcessor.apvts, "Threshold", thresholdSlider),
    kneeAttachment(audioProcessor.apvts, "Knee", kneeSlider)
{
    openGLContext.attachTo(*this);
    addVisual();
    setSize (600, 200);
}

void CompressorAudioProcessorEditor::addVisual()
{
    addAndMakeVisible(attackSlider);
    attackSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    attackSlider.setLookAndFeel(&myLookAndFeelV1);
    attackSlider.setTextBoxStyle(Slider::NoTextBox, true, 100, 100);
    attackSlider.setTextValueSuffix(" Milliseconds");
    attackSlider.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(releaseSlider);
    releaseSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    releaseSlider.setLookAndFeel(&myLookAndFeelV1);
    releaseSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    releaseSlider.setTextValueSuffix(" Milliseconds");
    releaseSlider.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(ratioSlider);
    ratioSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    ratioSlider.setLookAndFeel(&myLookAndFeelV1);
    ratioSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    ratioSlider.setTextValueSuffix(":1");
    ratioSlider.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(thresholdSlider);
    thresholdSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    thresholdSlider.setLookAndFeel(&myLookAndFeelV1);
    thresholdSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    thresholdSlider.setTextValueSuffix(" dB");
    thresholdSlider.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(kneeSlider);
    kneeSlider.setSliderStyle(Slider::RotaryVerticalDrag);
    kneeSlider.setLookAndFeel(&myLookAndFeelV1);
    kneeSlider.setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    kneeSlider.setTextValueSuffix(" dB");
    kneeSlider.setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(StereoBtn);
    StereoBtn.setButtonText(" Dual Mono ");
    StereoBtn.setClickingTogglesState("true");
    StereoBtn.addListener(this);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    attackSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
    thresholdSlider .setLookAndFeel(nullptr);
    kneeSlider .setLookAndFeel(nullptr);
    ratioSlider.setLookAndFeel(nullptr);
}

void CompressorAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &StereoBtn) {
        audioProcessor.setFilteringEnbaled(StereoBtn.getToggleState());
        if (StereoBtn.getToggleState()) {
            StereoBtn.setButtonText(" Stereo ");
        }
        else {
            StereoBtn.setButtonText(" Dual Mono ");
        }
    }
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);

    // Drawing the text for the dials
    g.drawFittedText("Knee", (((getWidth() / 6) * 1) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Attack", (((getWidth() / 6) * 2) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Release", (((getWidth() / 6) * 3) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Ratio", (((getWidth() / 6) * 4) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Threshold", (((getWidth() / 6) * 5) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
}

void CompressorAudioProcessorEditor::resized()
{
    // Drawing the dials
    kneeSlider.setBounds(((getWidth() / 6) * 1) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    attackSlider.setBounds(((getWidth() / 6) * 2) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    releaseSlider.setBounds(((getWidth() / 6) * 3) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    ratioSlider.setBounds(((getWidth() / 6) * 4) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    thresholdSlider.setBounds(((getWidth() / 6) * 5) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);

    StereoBtn.setBounds((getWidth() / 2) - (100 / 2), 15, 100, 50);
}

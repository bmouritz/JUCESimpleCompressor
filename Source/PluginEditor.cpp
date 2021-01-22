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
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(*(attackKnob = std::make_unique<Slider>("Attack")));
    attackKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    attackKnob->setLookAndFeel(&myLookAndFeelV1);
    attackKnob->setTextBoxStyle(Slider::NoTextBox, true, 100, 100);
    attackKnob->setTextValueSuffix(" Milliseconds");
    attackKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(releaseKnob = std::make_unique<Slider>("Release")));
    releaseKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    releaseKnob->setLookAndFeel(&myLookAndFeelV1);
    releaseKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    releaseKnob->setTextValueSuffix(" Milliseconds");
    releaseKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(ratioKnob = std::make_unique<Slider>("Ratio")));
    ratioKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    ratioKnob->setLookAndFeel(&myLookAndFeelV1);
    ratioKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    ratioKnob->setTextValueSuffix(":1");
    ratioKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(thresholdKnob = std::make_unique<Slider>("Volume")));
    thresholdKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    thresholdKnob->setLookAndFeel(&myLookAndFeelV1);
    thresholdKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    thresholdKnob->setTextValueSuffix(" dB");
    thresholdKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(gainKnob = std::make_unique<Slider>("Gain")));
    gainKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    gainKnob->setLookAndFeel(&myLookAndFeelV1);
    gainKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    gainKnob->setTextValueSuffix(" dB");
    gainKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(On);
    On.setButtonText(" 2x ");
    On.setClickingTogglesState("true");
    On.setColour(TextButton::buttonColourId, Colours::green);
    On.addListener(this);

    attackAttachment = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "attack", *attackKnob);
    releaseAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "release", *releaseKnob);
    ratioAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "ratio", *ratioKnob);
    thresholdAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "threshold", *thresholdKnob);
    gainAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "gain", *gainKnob);

    setSize (600, 200);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    attackKnob->setLookAndFeel(nullptr);
    releaseKnob->setLookAndFeel(nullptr);
    thresholdKnob->setLookAndFeel(nullptr);
    gainKnob->setLookAndFeel(nullptr);
    ratioKnob->setLookAndFeel(nullptr);
}

// Used to send a bool value to the PluginProcessor to check the state of the button
void CompressorAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &On)
        audioProcessor.setFilteringEnbaled(On.getToggleState());
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);

    // Drawing the text for the dials
    g.drawFittedText("Gain", (((getWidth() / 6) * 1) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Attack", (((getWidth() / 6) * 2) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Release", (((getWidth() / 6) * 3) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Ratio", (((getWidth() / 6) * 4) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Threshold", (((getWidth() / 6) * 5) - (100 / 2)), (((getHeight() / 4) * 3) - (50 / 2)), 100, 100, juce::Justification::centred, 1);

    g.drawFittedText("Oversampling",(getWidth() / 2) - (100 / 2), 15, 100, 100, juce::Justification::centred, 1);
}

void CompressorAudioProcessorEditor::resized()
{
    // Drawing the dials
    gainKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    attackKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    releaseKnob->setBounds(((getWidth() / 6) * 3) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    ratioKnob->setBounds(((getWidth() / 6) * 4) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    thresholdKnob->setBounds(((getWidth() / 6) * 5) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    
    // Drawing the buttons
    On.setBounds((getWidth() / 2) - (50/ 2), 25, 50, 25);
}

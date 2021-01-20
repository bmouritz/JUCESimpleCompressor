/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessorEditor::CompressorAudioProcessorEditor (CompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(*(attackKnob = std::make_unique<Slider>("Attack")));
    attackKnob->setSliderStyle(Slider::Rotary);
    attackKnob->setTextBoxStyle(Slider::NoTextBox, true, 100, 100);
    attackKnob->setTextValueSuffix(" Milliseconds");
    attackKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(releaseKnob = std::make_unique<Slider>("Release")));
    releaseKnob->setSliderStyle(Slider::Rotary);
    releaseKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    releaseKnob->setTextValueSuffix(" Milliseconds");
    releaseKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(ratioKnob = std::make_unique<Slider>("Ratio")));
    ratioKnob->setSliderStyle(Slider::Rotary);
    ratioKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    ratioKnob->setTextValueSuffix(":1");
    ratioKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(thresholdKnob = std::make_unique<Slider>("Volume")));
    thresholdKnob->setSliderStyle(Slider::Rotary);
    thresholdKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    thresholdKnob->setTextValueSuffix(" dB");
    thresholdKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(*(gainKnob = std::make_unique<Slider>("Gain")));
    gainKnob->setSliderStyle(Slider::Rotary);
    gainKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    gainKnob->setTextValueSuffix(" dB");
    gainKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(On);
    On.setButtonText(" 4x ");
    On.setClickingTogglesState("true");
    On.setColour(TextButton::buttonColourId, Colours::green);
    On.setRadioGroupId(2);
    On.addListener(this);

    addAndMakeVisible(Off);
    Off.setButtonText(" OFF ");
    Off.setClickingTogglesState("True");
    Off.setRadioGroupId(2);
    Off.setColour(TextButton::buttonColourId, Colours::red);
    Off.setToggleState("true", dontSendNotification);
    Off.addListener(this);

    attackAttachment = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "attack", *attackKnob);
    releaseAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "release", *releaseKnob);
    ratioAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "ratio", *ratioKnob);
    thresholdAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "threshold", *thresholdKnob);
    gainAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "gain", *gainKnob);

    setSize (600, 200);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

// Used to send a bool value to the PluginProcessor to check the state of the button
void CompressorAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &On || buttonThatWasClicked == &Off)
        audioProcessor.setFilteringEnbaled(On.getToggleState());
}

//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);

    // Drawing the text for the dials
    g.drawFittedText("Gain", (((getWidth() / 6) * 1) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Attack", (((getWidth() / 6) * 2) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Release", (((getWidth() / 6) * 3) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Ratio", (((getWidth() / 6) * 4) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Threshold", (((getWidth() / 6) * 5) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);

}

void CompressorAudioProcessorEditor::resized()
{
    // Drawing the dials
    gainKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    attackKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    releaseKnob->setBounds(((getWidth() / 6) * 3) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    ratioKnob->setBounds(((getWidth() / 6) * 4) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    thresholdKnob->setBounds(((getWidth() / 6) * 5) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    
    // Drawing the buttons
    On.setBounds((getWidth() / 2) - (150 / 2), 10, 50, 50);
    Off.setBounds((getWidth() / 2) + (50 / 2), 10, 50, 50);
}

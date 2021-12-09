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
    openGLContext.attachTo(*this);

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

    addAndMakeVisible(*(kneeKnob = std::make_unique<Slider>("Knee")));
    kneeKnob->setSliderStyle(Slider::RotaryVerticalDrag);
    kneeKnob->setLookAndFeel(&myLookAndFeelV1);
    kneeKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    kneeKnob->setTextValueSuffix(" dB");
    kneeKnob->setPopupDisplayEnabled(true, false, this);

    attackAttachment = std::make_unique <AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "attack", *attackKnob);
    releaseAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "release", *releaseKnob);
    ratioAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "ratio", *ratioKnob);
    thresholdAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "threshold", *thresholdKnob);
    kneeAttachment = std::make_unique < AudioProcessorValueTreeState::SliderAttachment>(p.getState(), "knee", *kneeKnob);

    setSize (600, 200);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
    attackKnob->setLookAndFeel(nullptr);
    releaseKnob->setLookAndFeel(nullptr);
    thresholdKnob->setLookAndFeel(nullptr);
    kneeKnob->setLookAndFeel(nullptr);
    ratioKnob->setLookAndFeel(nullptr);
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
    kneeKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    attackKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    releaseKnob->setBounds(((getWidth() / 6) * 3) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    ratioKnob->setBounds(((getWidth() / 6) * 4) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
    thresholdKnob->setBounds(((getWidth() / 6) * 5) - (100 / 2), ((getHeight() / 2) - (60 / 2)), 100, 100);
}

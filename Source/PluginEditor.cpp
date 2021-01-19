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
    addAndMakeVisible(attackKnob = new Slider("Attack"));
    attackKnob->setSliderStyle(Slider::Rotary);
    attackKnob->setTextBoxStyle(Slider::NoTextBox, true, 100, 100);
    attackKnob->setTextValueSuffix(" Seconds");
    attackKnob->setPopupDisplayEnabled(true, false, this);


    addAndMakeVisible(releaseKnob = new Slider("Release"));
    releaseKnob->setSliderStyle(Slider::Rotary);
    releaseKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    releaseKnob->setTextValueSuffix(" Seconds");
    releaseKnob->setPopupDisplayEnabled(true, false, this);


    addAndMakeVisible(ratioKnob = new Slider("Ratio"));
    ratioKnob->setSliderStyle(Slider::Rotary);
    ratioKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    ratioKnob->setTextValueSuffix(":1");
    ratioKnob->setPopupDisplayEnabled(true, false, this);


    addAndMakeVisible(thresholdKnob = new Slider("Volume"));
    thresholdKnob->setSliderStyle(Slider::Rotary);
    thresholdKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    thresholdKnob->setTextValueSuffix(" dB");
    thresholdKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(gainKnob = new Slider("Gain"));
    gainKnob->setSliderStyle(Slider::Rotary);
    gainKnob->setTextBoxStyle(Slider::NoTextBox, false, 100, 100);
    gainKnob->setTextValueSuffix(" dB");
    gainKnob->setPopupDisplayEnabled(true, false, this);

    addAndMakeVisible(onOffBtn = new TextButton("On Off Button"));
    onOffBtn->setButtonText("ON");
    onOffBtn->addListener(this);
    onOffBtn->setClickingTogglesState(true);

    attackAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "attack", *attackKnob);
    releaseAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "release", *releaseKnob);
    ratioAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "ratio", *ratioKnob);
    thresholdAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "threshold", *thresholdKnob);
    gainAttachment = new AudioProcessorValueTreeState::SliderAttachment(p.getState(), "gain", *gainKnob);
//    onOffBtnAttachment = new AudioProcessorValueTreeState::ButtonAttachment(p.getState(), "On Off Btn", *onOffBtn);

    setSize (600, 200);
}

CompressorAudioProcessorEditor::~CompressorAudioProcessorEditor()
{
}

void CompressorAudioProcessorEditor::buttonClicked(Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == onOffBtn && buttonThatWasClicked->getClickingTogglesState() == true)
    {
        onOffBtn->setButtonText("OFF");
        onOffBtn->setClickingTogglesState(false);
    }
    else {
        onOffBtn->setButtonText("ON");
        onOffBtn->setClickingTogglesState(true);
    }
}


//==============================================================================
void CompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawFittedText("Gain", (((getWidth() / 6) * 1) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Attack", (((getWidth() / 6) * 2) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Release", (((getWidth() / 6) * 3) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Ratio", (((getWidth() / 6) * 4) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);
    g.drawFittedText("Threshold", (((getWidth() / 6) * 5) - (100 / 2)), (((getHeight() / 4) * 3) - (100 / 2)), 100, 100, juce::Justification::centred, 1);

}

void CompressorAudioProcessorEditor::resized()
{
    gainKnob->setBounds(((getWidth() / 6) * 1) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    attackKnob->setBounds(((getWidth() / 6) * 2) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    releaseKnob->setBounds(((getWidth() / 6) * 3) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    ratioKnob->setBounds(((getWidth() / 6) * 4) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
    thresholdKnob->setBounds(((getWidth() / 6) * 5) - (100 / 2), ((getHeight() / 2) - (100 / 2)), 100, 100);
  //  onOffBtn->setBounds(((getWidth() / 2)) - (50/ 2), ((getHeight() / 5) * 2.5) - (100 / 2) + 100, 50, 25);
}

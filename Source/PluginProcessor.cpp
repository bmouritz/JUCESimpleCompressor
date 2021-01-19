/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

    Created by Ben Mouritz
    18/01/2021

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompressorAudioProcessor::CompressorAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    state = new AudioProcessorValueTreeState(*this, nullptr);

    state->createAndAddParameter("attack", "Attack", "Attack", NormalisableRange<float>(0.0f, 20.0f, 0.1f), 0.0f, nullptr, nullptr);
    state->createAndAddParameter("release", "Release", "Release", NormalisableRange<float>(0.0f, 200.0f, 0.1f), 0.0f, nullptr, nullptr);
    state->createAndAddParameter("ratio", "Ratio", "Ratio", NormalisableRange<float>(1.0f, 30.0f, 3.0f), 1.0f, nullptr, nullptr);
    state->createAndAddParameter("threshold", "Threshold", "Threshold", NormalisableRange<float>(-50.0, 0.0f, 1.0f), -40.0f, nullptr, nullptr);
    state->createAndAddParameter("gain", "Gain", "Gain", NormalisableRange<float>(-15.0f, 40.0f, 5.0f), 0.0f, nullptr, nullptr);

    state->state = ValueTree("attack");
    state->state = ValueTree("release");
    state->state = ValueTree("ratio");
    state->state = ValueTree("threshold");
    state->state = ValueTree("gain");

    dsp::Gain<float> inputGain;
    inputGain.setGainDecibels(0);
    
    dsp::Compressor<float> compressor;
    compressor.setAttack(1.0f);
    compressor.setRatio(1);
    compressor.setRelease(5.0f);
    compressor.setThreshold(-40.0f);
}

CompressorAudioProcessor::~CompressorAudioProcessor()
{
}

//==============================================================================
const juce::String CompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}


bool CompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void CompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    compressor.prepare(spec);
    inputGain.prepare(spec);
}

void CompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CompressorAudioProcessor::updateParameters() {
    //update your parameters for your processes
    float attack = *state->getRawParameterValue("attack");
    float release = *state->getRawParameterValue("release");
    float ratio = *state->getRawParameterValue("ratio");
    float threshold = *state->getRawParameterValue("threshold");
    float gain = *state->getRawParameterValue("gain");

    inputGain.setGainDecibels(gain);
    compressor.setAttack(attack);
    compressor.setRatio(ratio);
    compressor.setRelease(release);
    compressor.setThreshold(threshold);
}

void CompressorAudioProcessor::process(dsp::ProcessContextReplacing<float> context) {
    //do processing here and output
    compressor.process(context);
    inputGain.process(context);
    updateParameters();
}

void CompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    dsp::AudioBlock<float> block(buffer);
    process(dsp::ProcessContextReplacing<float>(block));
}

AudioProcessorValueTreeState& CompressorAudioProcessor::getState() {
    return *state;
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
    return new CompressorAudioProcessorEditor (*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    MemoryOutputStream stream(destData, false);
    state->state.writeToStream(stream);
}

void CompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    ValueTree tree = ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        state->state = tree;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompressorAudioProcessor();
}

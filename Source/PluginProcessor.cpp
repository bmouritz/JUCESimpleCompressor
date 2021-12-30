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
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
#endif
{

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

void CompressorAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String CompressorAudioProcessor::getProgramName(int index)
{
	return {};
}

void CompressorAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void CompressorAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	dsp::ProcessSpec spec;

	spec.sampleRate = sampleRate;
	spec.maximumBlockSize = samplesPerBlock;
	spec.numChannels = getNumOutputChannels();

	compressor.reset();
	compressor.prepare(spec);

	updateParameters();
}

void CompressorAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CompressorAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
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
	float attack = apvts.getRawParameterValue("Attack")->load();
	float release = apvts.getRawParameterValue("Release")->load();
	float ratio = apvts.getRawParameterValue("Ratio")->load();
	float threshold = apvts.getRawParameterValue("Threshold")->load();
	float knee = apvts.getRawParameterValue("Knee")->load();

	compressor.setAttack(attack);
	compressor.setRatio(ratio);
	compressor.setRelease(release);
	compressor.setThreshold(threshold);
	compressor.setKneeWidth(knee);
}

void CompressorAudioProcessor::process(dsp::ProcessContextReplacing<float> context) {
	//do processing here and output
	updateParameters();
	compressor.process(context);
}

void CompressorAudioProcessor::processStereo(dsp::ProcessContextReplacing<float> context) {
	//do processing here and output
	updateParameters();
	compressor.processStereo(context);
}

void CompressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	dsp::AudioBlock<float> block(buffer);

	if (stereo) {
		// Stereo linked
		processStereo(dsp::ProcessContextReplacing<float>(block));
	}
	else {
		// Dual Mono
		process(dsp::ProcessContextReplacing<float>(block));
	}
}

juce::AudioProcessorValueTreeState::ParameterLayout CompressorAudioProcessor::createParameterLayout() {
	juce::AudioProcessorValueTreeState::ParameterLayout layout;

	layout.add(std::make_unique<juce::AudioParameterFloat>("Attack", "Attack", NormalisableRange<float>(1.0f, 50.0f, 1.0f), 10.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>("Release", "Release", NormalisableRange<float>(40.0f, 120.0f, 5.0f), 80.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>("Ratio", "Ratio", NormalisableRange<float>(2.0f, 10.0f, 1.0f), 3.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>("Threshold", "Threshold", NormalisableRange<float>(-60.0, 0.0f, 0.1f), 0.0f));
	layout.add(std::make_unique<juce::AudioParameterFloat>("Knee", "Knee", NormalisableRange<float>(0.0, 20.0f, 1.0f), 6.0f));

	return layout;
}

//==============================================================================
bool CompressorAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CompressorAudioProcessor::createEditor()
{
	return new CompressorAudioProcessorEditor(*this);
}

//==============================================================================
void CompressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	MemoryOutputStream mos(destData, true);
	apvts.state.writeToStream(mos);
}

void CompressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	auto tree = ValueTree::readFromData(data, sizeInBytes);
	if (tree.isValid()) {
		apvts.replaceState(tree);
		updateParameters();
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new CompressorAudioProcessor();
}
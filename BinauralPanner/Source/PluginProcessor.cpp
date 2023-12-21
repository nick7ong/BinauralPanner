/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BinauralPannerAudioProcessor::BinauralPannerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
    BinauralPanner()
#endif
{
    elevationParameter = new juce::AudioParameterFloat(juce::ParameterID("1", 1), "Elevation", -45.0f, 75.0f, 0.0f);
    azimuthParameter = new juce::AudioParameterFloat(juce::ParameterID("2", 2), "Azimuth", 0.0f, 360.0f, 0.0f);
    gainParameter = new juce::AudioParameterFloat(juce::ParameterID("3", 3), "Gain", 0.0f, 1.0f, 0.5f);
    addParameter(elevationParameter);
    addParameter(azimuthParameter);
    addParameter(gainParameter);

    BinauralPanner.setTheta(0);
    BinauralPanner.setGain(*gainParameter);

    currentElevation = *elevationParameter;
    currentAzimuth = *azimuthParameter;
    currentGain = *gainParameter;
}

BinauralPannerAudioProcessor::~BinauralPannerAudioProcessor()
{
}

//==============================================================================
const juce::String BinauralPannerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BinauralPannerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BinauralPannerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BinauralPannerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BinauralPannerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BinauralPannerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BinauralPannerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BinauralPannerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BinauralPannerAudioProcessor::getProgramName (int index)
{
    return {};
}

void BinauralPannerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BinauralPannerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;

    BinauralPanner.prepare(spec, samplesPerBlock);
    BinauralPanner.updateHRTFFilter();
}

void BinauralPannerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BinauralPannerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void BinauralPannerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (*elevationParameter != currentElevation || *azimuthParameter != currentAzimuth
        || *gainParameter != currentGain)
    {
        currentElevation = *elevationParameter;
        currentAzimuth = *azimuthParameter;
        currentGain = *gainParameter;

        updateTheta();
        BinauralPanner.setGain(currentGain);
    }

    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    BinauralPanner.processBlock(buffer, totalNumInputChannels);
}

//==============================================================================
bool BinauralPannerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BinauralPannerAudioProcessor::createEditor()
{
    return new BinauralPannerAudioProcessorEditor (*this);
}

//==============================================================================
void BinauralPannerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BinauralPannerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
int BinauralPannerAudioProcessor::getElevationBaseIndex(float elevation)
{
    if (elevation == -45) return 0;
    else if (elevation == -30) return 24;
    else if (elevation == -15) return 48;
    else if (elevation == 0) return 72;
    else if (elevation == 15) return 96;
    else if (elevation == 30) return 120;
    else if (elevation == 45) return 144;
    else if (elevation == 60) return 168;
    else if (elevation == 75) return 180;
    else return 186;
}

int BinauralPannerAudioProcessor::getAzimuthOffset(float elevation, float azimuth)
{
    int offset = 0;
    if (elevation <= 45) // -45 to 45 degrees
    {
        offset = static_cast<int>(azimuth / 15.0f);
    }
    else if (elevation == 60)
    {
        offset = static_cast<int>(azimuth / 30.0f);
    }
    else if (elevation == 75)
    {
        offset = static_cast<int>(azimuth / 60.0f);
    }
    // No offset for elevation 90
    return offset;
}

void BinauralPannerAudioProcessor::updateTheta()
{
    int baseIndex = getElevationBaseIndex(currentElevation);
    int azimuthOffset = getAzimuthOffset(currentElevation, currentAzimuth);

    int theta = baseIndex + azimuthOffset;

    // Update BinauralPanner with new theta
    BinauralPanner.setTheta(theta);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BinauralPannerAudioProcessor();
}

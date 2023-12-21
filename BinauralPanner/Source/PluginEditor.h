/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BinauralPannerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    BinauralPannerAudioProcessorEditor (BinauralPannerAudioProcessor&);
    ~BinauralPannerAudioProcessorEditor() override;

    void sliderValueChanged(Slider* slider) override;


    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    BinauralPannerAudioProcessor& audioProcessor;

    juce::AudioParameterFloat* elevationParameter;
    juce::AudioParameterFloat* azimuthParameter;
    juce::AudioParameterFloat* gainParameter;

    Slider gainSlider;
    Slider elevationSlider;
    Slider azimuthSlider;

    Label elevationLabel;
    Label azimuthLabel;
    Label azimuthValueLabel;
    Label gainLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralPannerAudioProcessorEditor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BinauralPannerAudioProcessorEditor::BinauralPannerAudioProcessorEditor (BinauralPannerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 350);

    auto parameterTree = audioProcessor.getParameters();

    elevationParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(0);
    azimuthParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(1);
    gainParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::LinearVertical);
    gainSlider.setRange(0.0, 1.0, 0.1);
    gainSlider.setValue(0.5);
    gainSlider.setColour(Slider::thumbColourId, Colour::fromRGB(73, 166, 201));
    gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 15);
    gainSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    gainSlider.addListener(this);


    addAndMakeVisible(elevationSlider);
    elevationSlider.setSliderStyle(Slider::Rotary);
    elevationSlider.setRange(-45, 90, 15);
    elevationSlider.setValue(0);
    elevationSlider.setColour(Slider::thumbColourId, Colour::fromRGB(73, 166, 201));
    elevationSlider.setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    elevationSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 60, 15);
    elevationSlider.setColour(Slider::textBoxOutlineColourId, Colours::transparentBlack);
    elevationSlider.addListener(this);

    addAndMakeVisible(azimuthSlider);
    azimuthSlider.setSliderStyle(Slider::Rotary);
    azimuthSlider.setRotaryParameters(0.0, (2.0 * float_Pi), false);
    azimuthSlider.setRange(0, 360, 15);
    azimuthSlider.setColour(Slider::rotarySliderFillColourId, Colours::transparentBlack);
    azimuthSlider.setTextBoxStyle(Slider::NoTextBox, true, 60, 15);
    azimuthSlider.addListener(this);

    // Elevation Label
    addAndMakeVisible(elevationLabel);
    elevationLabel.setText("Elevation", dontSendNotification);
    elevationLabel.setColour(Label::textColourId, Colours::white);
    elevationLabel.setJustificationType(Justification::centred);

    // Azimuth Label
    addAndMakeVisible(azimuthLabel);
    azimuthLabel.setText("Azimuth", dontSendNotification);
    azimuthLabel.setColour(Label::textColourId, Colours::white);
    azimuthLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(azimuthValueLabel);
    azimuthValueLabel.setFont(Font(15.0f));
    azimuthValueLabel.setJustificationType(Justification::centred);
    azimuthValueLabel.setEditable(false, false, false);
    azimuthValueLabel.setColour(Label::textColourId, Colours::white);
    azimuthValueLabel.setColour(Label::backgroundColourId, Colours::transparentBlack);
    azimuthValueLabel.setText(String(azimuthSlider.getValue(), 0), dontSendNotification);

    // Gain Label
    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain", dontSendNotification);
    gainLabel.setColour(Label::textColourId, Colours::white);
    gainLabel.setJustificationType(Justification::centred);
}

BinauralPannerAudioProcessorEditor::~BinauralPannerAudioProcessorEditor()
{
}

//==============================================================================
void BinauralPannerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour(Colours::white);
    g.setFont(25.0f);
    g.drawFittedText("Binaural Panner", getLocalBounds(), juce::Justification::centredTop, 1);
}

void BinauralPannerAudioProcessorEditor::resized()
{
    gainSlider.setBounds(325, 30, 50, 300);
    elevationSlider.setBounds(10, 30, 310, 310);
    azimuthSlider.setBounds(40, 52, 250, 250);

    gainLabel.setBounds(325, 10, 50, 20); 
    elevationLabel.setBounds(10, 305, 310, 20);
    azimuthLabel.setBounds(135, 160, 60, 30);
    azimuthValueLabel.setBounds(135, 177, 60, 30);
}

void BinauralPannerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
    if (slider == &elevationSlider)
    {
        *elevationParameter = slider->getValue();
    }
    else if (slider == &azimuthSlider)
    {
        int azimuthValue = static_cast<int>(slider->getValue());
        azimuthValueLabel.setText(String(azimuthValue), dontSendNotification);
        *azimuthParameter = slider->getValue();
    }
    else if (slider == &gainSlider)
    {
        *gainParameter = slider->getValue();
    }
}

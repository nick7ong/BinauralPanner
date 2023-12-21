/*
  ==============================================================================

    BinauralPanner.h
    Created: 28 Nov 2023 10:28:23am
    Author:  nickn

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <fstream>
#include <sstream>
#include <string>
#include "HRTF.h"

using namespace juce;

class BinauralPanner
{
public:
    BinauralPanner();
    ~BinauralPanner();

    void prepare(const dsp::ProcessSpec& spec, int samplesPerBlock);
    void setTheta(int newTheta);
    void setGain(float newGain);
    void updateHRTFFilter();
    void processBlock(AudioBuffer<float>& buffer, int channel);

private:
    dsp::FIR::Filter<float> IR_L, IR_R;
    dsp::IIR::Filter<float> lowShelfFilter;

    int theta = 72;
    float gain = 0.5f;
};

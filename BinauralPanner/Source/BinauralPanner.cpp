/*
  ==============================================================================

    BinauralPanner.cpp
    Created: 28 Nov 2023 10:28:23am
    Author:  nickn

  ==============================================================================
*/

#include "BinauralPanner.h"

BinauralPanner::BinauralPanner()
{
}

BinauralPanner::~BinauralPanner()
{
}

void BinauralPanner::prepare(const dsp::ProcessSpec& spec, int samplesPerBlock)
{
    // Prepare FIR for Binaural Rendering
    IR_L.prepare(spec);
    IR_R.prepare(spec);
    IR_L.reset();
    IR_R.reset();

    // Prepare LSF
    lowShelfFilter.reset();
    lowShelfFilter.prepare(spec);
    lowShelfFilter.coefficients = dsp::IIR::Coefficients<float>::makeLowShelf(spec.sampleRate, 120.0f, 0.25, 1.5);;
}

void BinauralPanner::setTheta(int newTheta) {
    this->theta = newTheta;
}

void BinauralPanner::setGain(float newGain) {
    this->gain = newGain;
}

void BinauralPanner::updateHRTFFilter()
{
    int IRSize = 512;
    *(IR_L.coefficients) = dsp::FIR::Coefficients<float>(hrtf_l[theta], IRSize);
    *(IR_R.coefficients) = dsp::FIR::Coefficients<float>(hrtf_r[theta], IRSize);
}

void BinauralPanner::processBlock(AudioBuffer<float>& buffer, int channels)
{
    buffer.applyGain(gain);
    auto bufferL = buffer.getWritePointer(0);
    auto bufferR = buffer.getWritePointer(1);
    const auto bufferSize = buffer.getNumSamples();

    // Binaural Rendering
    updateHRTFFilter();
    dsp::AudioBlock<float> blockL = dsp::AudioBlock<float>(&bufferL, 1, bufferSize);
    dsp::AudioBlock<float> blockR = dsp::AudioBlock<float>(&bufferR, 1, bufferSize);
    IR_L.process(dsp::ProcessContextReplacing<float>(blockL));
    IR_R.process(dsp::ProcessContextReplacing<float>(blockR));

    // Apply LSF
    lowShelfFilter.process(dsp::ProcessContextReplacing<float>(blockL));
    lowShelfFilter.process(dsp::ProcessContextReplacing<float>(blockR));
}

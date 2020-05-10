#include "Delay.h"
#include <include_juce_dsp.cpp>

Delay::Delay() : 
	wet(0.5), 
	feedback(0.3), 
	delayTimes({7000.0, 7000.0}), 
	n_channels(2),
	m_sampleRate(44100)
{
	reset(44100, 2);
}

Delay::~Delay(){}

void Delay::reset(int sampleRate, int num_channels) 
{
	m_sampleRate = sampleRate;
	n_channels = num_channels;
	delayLines.resize(n_channels);
	for (int i = 0; i < n_channels; ++i) { delayLines[i].reset(44100); }
	reset_delay_lengths();
}

void Delay::set_wet(double w)
{
	wet = w;
}

void Delay::set_feedback(double fb)
{
	feedback = fb;
}

void Delay::set_delay_length(double len, int channel)
{
	delayTimes[channel] = len;
	reset_delay_lengths();
}

void Delay::reset_delay_lengths()
{
	for (int i = 0; i < n_channels; ++i) 
	{
		delayLines[i].setDelayLength(delayTimes[i]);
	}
}

void Delay::process(const float* readPointer, float* writePointer, int channel, int num_samples)
{
	auto& dline = delayLines[channel];
	for (int i = 0; i < num_samples; ++i)
	{
		auto delayedSample = dline.getRead();
		auto inputSample = readPointer[i];
		auto dlineInputSample = std::tanh(((1.0 - feedback) * inputSample) + (feedback * delayedSample));
		dline.writeToBuffer((double)dlineInputSample);
		auto outputSample = ((1.0 - wet) * inputSample) + (wet * delayedSample);
		writePointer[i] = outputSample;
	}
}
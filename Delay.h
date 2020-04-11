#pragma once
#include <JuceHeader.h>

template <typename Type, size_t maxNumChannels = 2>
class Delay
{
private:
    std::array<DelayLine<Type>, maxNumChannels> delayLines;
    std::array<size_t, maxNumChannels> delayTimesSample;
    std::array<Type, maxNumChannels> delayTimes;
    Type feedback{ Type(0) };
    Type wetLevel{ Type(0) };

    std::array<juce::dsp::IIR::Filter<Type>, maxNumChannels> filters;
    typename juce::dsp::IIR::Coefficients<Type>::Ptr filterCoefs;

    Type sampleRate{ Type(44.1e3) };
    Type maxDelayTime{ Type(2) };

    void updateDelayLineSize()
    {
        auto delayLineSizeSamples = (size_t)std::ceil(maxDelayTime * sampleRate);

        for (auto& dline : delayLines)
            dline.resize(delayLineSizeSamples);
    }

    void updateDelayTime() noexcept
    {
        for (size_t ch = 0; ch < maxNumChannels; ++ch)
            delayTimesSample[ch] = (size_t)juce::roundToInt(delayTimes[ch] * sampleRate);
    }

public:
    Delay()
    {
        setMaxDelayTime(2.0f);
        setDelayTime(0, 0.7f);
        setDelayTime(1, 0.5f);
        setWetLevel(0.8f);
        setFeedback(0.5f);
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        jassert(spec.numChannels <= maxNumChannels);
        sampleRate = (Type)spec.sampleRate;
        updateDelayLineSize();
        updateDelayTime();

        filterCoefs = juce::dsp::IIR::Coefficients<Type>::makeFirstOrderLowPass(sampleRate, Type(1e3));

        for (auto& f : filters)
        {
            f.prepare(spec);
            f.coefficients = filterCoefs;
        }
    }

    void reset() noexcept
    {
        for (auto& f : filters)
            f.reset();

        for (auto& dline : delayLines)
            dline.clear();
    }

    size_t getNumChannels() const noexcept
    {
        return delayLines.size();
    }

    void setMaxDelayTime(Type newValue)
    {
        jassert(newValue > Type(0));
        maxDelayTime = newValue;
        updateDelayLineSize();
    }

    void setFeedback(Type newValue) noexcept
    {
        jassert(newValue >= Type(0) && newValue <= Type(1));
        feedback = newValue;
    }

    void setWetLevel(Type newValue) noexcept
    {
        jassert(newValue >= Type(0) && newValue <= Type(1));
        wetLevel = newValue;
    }

    void setDelayTime(size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert(newValue >= Type(0));
        delayTimes[channel] = newValue;
        updateDelayTime();
    }

   
    void process(AudioBuffer<Type>& buffer, int n_samples, int n_channels) noexcept
    {
        for (size_t channel = 0; channel < n_channels; ++channel)
        {
            auto* inBuffer = buffer.getReadPointer(channel);
            auto* outBuffer = buffer.getWritePointer(channel);
            auto& dline = delayLines[channel];
            auto delayTime = delayTimesSample[channel];
            auto& filter = filters[channel];

            for (size_t i = 0; i < n_samples; ++i)
            {
                auto delayedSample = dline.get(delayTime);
                auto inputSample = inBuffer[i];
                auto dlineInputSample = std::tanh(inputSample + feedback * delayedSample);
                dline.push(dlineInputSample);
                auto outputSample = inputSample + wetLevel * delayedSample;
                outBuffer[i] = outputSample;
            }
        }
    }
};
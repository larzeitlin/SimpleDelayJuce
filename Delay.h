#include <vector>
#include "Cbuffer.h"
#ifndef Delay_hpp
#define Delay_hpp
#endif /* Delay_h */

class Delay
{
private:
    int m_sampleRate;
    int n_channels;
    double feedback;
    double wet;
    std::vector<double> delayTimes;
    std::vector<Cbuffer> delayLines;

public:
    Delay();
    ~Delay();
    void reset(int sampleRate, int n_channels);
    void set_feedback(double fb);
    void set_wet(double w);
    void set_delay_length(double len, int channel);
    void reset_delay_lengths();
    void process(const float* readPointer, float* writePointer, int channel, int num_samples);
};

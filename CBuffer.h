#ifndef CBuffer_hpp
#define CBuffer_hpp

#include <stdio.h>

#endif /* CBuffer_h */
#include <vector>

class Cbuffer
{

private:
    double m_delayLength;
    int writeIndex;
    int m_length;
    std::vector<double> mpBuffer;
    std::vector<double> dataPoints;
    int interpSampN;


public:
    Cbuffer(int length = 44100);
    ~Cbuffer();
    void reset(int length = 44100);
    void setDelayLength(int delayLength);
    void writeToBuffer(double inVal);
    void setDelayLength(double newDelay);
    double getRead();
   
    double lagrangeInterp(int length, double x);
    void setInterpSamN(int numSams);
};

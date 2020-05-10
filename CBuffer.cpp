#include "CBuffer.h"

Cbuffer::Cbuffer(int length) : m_length(length)
{
    mpBuffer.resize(length);
    std::fill(mpBuffer.begin(), mpBuffer.end(), 0.0);
    m_delayLength = 1.0;
    writeIndex = 0;
    interpSampN = 5;
    dataPoints.resize(interpSampN);
}

Cbuffer::~Cbuffer()
{}

void Cbuffer::reset(int length)
{
    m_length = length;
    mpBuffer.resize(length);
    std::fill(mpBuffer.begin(), mpBuffer.end(), 0.0);
    m_delayLength = 0.;
    writeIndex = 0;
}

void Cbuffer::writeToBuffer(double inVal)
{
    mpBuffer[writeIndex] = inVal;
    ++writeIndex;
    if (writeIndex >= m_length) { writeIndex = 0; }
}


void Cbuffer::setDelayLength(double delayLength)
{
    m_delayLength = delayLength;
}

double Cbuffer::lagrangeInterp(int length, double x)
{
    using namespace std;
    double sum = 0;
    int N = length - 1;

    for (double k = 0; k <= N; k += 1.)
    {
        double product = 1.0;
        for (double n = 0; n <= N; n += 1.)
        {
            if (k - n != 0)
            {
                product = product * ((x - n) / (k - n));
            }
        }
        sum += (product * dataPoints[(int)k]);
    }
    return(sum);
}

void Cbuffer::setInterpSamN(int numSams)
{
    interpSampN = numSams;
    dataPoints.resize(interpSampN);
}


double Cbuffer::getRead()
{
    using namespace std;
    double globalTargetX = (double(writeIndex) - m_delayLength);

    if (globalTargetX < 0) { globalTargetX += (double)m_length; }
    int centerSampleX = (int)globalTargetX;
    int startSamp = 0;

    double targetX = globalTargetX - (int)globalTargetX + (int)(interpSampN / 2);

    //for centering odd / even number of samples
    if (interpSampN % 2 == 1) { startSamp = interpSampN / 2; }
    else { startSamp = (interpSampN - 1) / 2; }
    {
        for (int i = 0; i < interpSampN; ++i)
        {
            int k = centerSampleX - startSamp + i;
            while (k < 0) { k += m_length; }
            if (k >= m_length) { k = 0; }
            dataPoints[i] = mpBuffer[k];
        }
    }
    return(lagrangeInterp(interpSampN, targetX));
}
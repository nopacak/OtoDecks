/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 20 Feb 2024 6:00:33pm
    Author:  ndoks

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{
}


DJAudioPlayer::~DJAudioPlayer()
{
}


void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}


void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}


void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) //good file
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        transportSource.start();
    }
}


void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain >1)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else
    {
        transportSource.setGain(gain);
    }
}


void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio >1)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 10");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}


void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}


void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos >1)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}


void DJAudioPlayer::stop()
{
    transportSource.stop();
}


double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
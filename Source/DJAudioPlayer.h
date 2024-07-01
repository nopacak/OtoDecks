/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 20 Feb 2024 6:00:33pm
    Author:  ndoks

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PlaylistComponent.h"


class DJAudioPlayer : public juce::AudioSource{
    public:
        DJAudioPlayer(juce::AudioFormatManager& _formatManager);
        ~DJAudioPlayer();

        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void loadURL(juce::URL audioURL);
        void setGain(double gain);
        void setSpeed(double ratio);
        void setPosition(double posInSecs);
        void setPositionRelative(double pos);

        void start();
        void stop();

        double getPositionRelative();


    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
        juce::AudioTransportSource transportSource;
        juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};


#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


class MainComponent : public juce::AudioAppComponent,
                      public juce::Timer
{
    public:
        MainComponent();
        ~MainComponent() override;

        void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
        void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
        void releaseResources() override;

        void paint (juce::Graphics& g) override;
        void resized() override;

        void timerCallback() override;


    private:

        juce::AudioFormatManager formatManager;
        juce::AudioThumbnailCache thumbCache{ 100 };

        DJAudioPlayer player1{formatManager};
        DJAudioPlayer player2{formatManager};

        DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
        DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

        juce::MixerAudioSource mixerSource;

        PlaylistComponent playlistComponent;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

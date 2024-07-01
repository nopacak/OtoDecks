/*
  ==============================================================================

    DeckGUI.h
    Created: 21 Feb 2024 3:09:36pm
    Author:  ndoks

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

class DeckGUI : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
{
    public:
        DeckGUI(DJAudioPlayer* player,
                juce::AudioFormatManager& formatManagerToUse,
                juce::AudioThumbnailCache& cacheToUse);

        ~DeckGUI() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        void buttonClicked(juce::Button* button) override;
        void sliderValueChanged(juce::Slider* slider) override;

        bool isInterestedInFileDrag(const juce::StringArray& files) override;
        void filesDropped(const juce::StringArray& files, int x, int y) override;

        void timerCallback() override;
        void playlistToDeckGUI();  // This function reads which audio file the user has selected to be added into either Deck 1 or 2

    private:
        juce::TextButton playButton{ "PLAY" };
        juce::TextButton stopButton{ "STOP" };
        juce::TextButton loadButton{ "LOAD" };
        juce::ToggleButton loopButton{ "LOOP" };
        juce::TextEditor trackName;
        juce::String mtrackName;
        juce::Label volSliderLabel;
        juce::Label speedSliderLabel;
        juce::Slider volSlider;
        juce::Slider speedSlider;
        juce::Slider posSlider;

        juce::FileChooser fChooser{ "Select a file..." };

        DJAudioPlayer* player; 

        WaveformDisplay waveformDisplay;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};

/*
  ==============================================================================

    DeckGUI.cpp
    Created: 21 Feb 2024 3:09:36pm
    Author:  ndoks

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse
                ) : player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse)

{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(trackName);
    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);

    //changing volume and speed sliders into rotary knobs
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);

    //adding text to vol and speed slider
    volSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 25);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 100, 25);

    //attaching text to the volume slider component
    volSliderLabel.setText("Volume", juce::NotificationType::dontSendNotification);
    volSliderLabel.attachToComponent(&volSlider, false);

    //attaching text to the volume speed slider component
    speedSliderLabel.setText("Speed", juce::NotificationType::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);

    //track name displayed on top of the deck is only readable
    trackName.setReadOnly(true);
    trackName.setTextToShowWhenEmpty("No track loaded", juce::Colours::thistle);

    //making label text visible
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(volSliderLabel);

    //the position slider which is the playback slider is set as transparent and overlayed above the waveform display,
    //this allows the user to directly click on the waveform display to navigate through the audio file's playback
    posSlider.setSliderStyle(juce::Slider::LinearBar);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    posSlider.setColour(juce::Slider::trackColourId, juce::Colours::transparentWhite);

    startTimer(100);
}


DeckGUI::~DeckGUI()
{
    stopTimer();
}


void DeckGUI::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (juce::Colours::darkslategrey);   // clear the background

    g.setColour (juce::Colours::thistle);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::thistle);
    g.setFont (18.0f);
    //g.drawText ("DJ OtoDecks App", getLocalBounds(),
                //juce::Justification::centredBottom, true);   // draw some placeholder text
}


void DeckGUI::resized()
{
    double rowH = getHeight() / 7;
    trackName.setBounds((getWidth() / 7) * 2, 0, (getWidth() / 7) * 3, rowH);

    volSlider.setBounds(0, rowH, getWidth() / 2, rowH * 3);
    speedSlider.setBounds(getWidth() / 2, rowH, getWidth() / 2, rowH * 3);

    waveformDisplay.setBounds(0, rowH * 4, getWidth(), rowH * 2);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH * 2);

    playButton.setBounds(getWidth() / 7 * 2, rowH * 6, getWidth() / 7, rowH);
    stopButton.setBounds(getWidth() / 7 * 3, rowH * 6, getWidth() / 7, rowH);
    loadButton.setBounds((getWidth() / 7) * 4, rowH * 6, getWidth() / 7, rowH);
    loopButton.setBounds((getWidth() / 7) * 5, rowH * 6, getWidth() / 7, rowH);

    volSliderLabel.setCentreRelative(0.43f, 0.4f);
    speedSliderLabel.setCentreRelative(0.94f, 0.4f);


    trackName.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkslategrey);
    trackName.setColour(juce::TextEditor::outlineColourId, juce::Colours::darkslategrey);

}


void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        player->start();
    }

    if (button == &stopButton)
    {
        player->stop();
    }

    if (button == &loadButton)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();
                player->loadURL(juce::URL{ chosenFile });
                waveformDisplay.loadURL(juce::URL{ chosenFile });

                DBG(juce::URL{ chooser.getResult() }.getFileName());

                trackName.setText(juce::URL{ chooser.getResult() }.getFileName(), juce::NotificationType::dontSendNotification);
            });
    }
}


void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray &files)
{
    return true;
}


void DeckGUI::filesDropped(const juce::StringArray &files, int x, int y) 
{
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }
}


void DeckGUI::timerCallback()
{

    if (std::to_string(loopButton.getToggleState()) == "1") { //If loop button is checked
        if (player->getPositionRelative() >= 1) {// and the position more than 1 meaning it is the end of the audio file
            player->setPositionRelative(0); // Sets position back to the start of the audio file
            player->start(); // Plays the audio file
        }
    }

    if (std::to_string(loopButton.getToggleState()) == "0") { //If loop button is not checked
        if (player->getPositionRelative() >= 1) { //and the position more than 1 meaning it is the end of the audio file
            player->setPositionRelative(0); //Sets position back to the start of the audio file
            player->stop(); // Stops the audio file
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative()); 
}


void DeckGUI::playlistToDeckGUI() 
{ //function rea

    std::ifstream file("playlist.txt"); //Opens playlist.txt which contains the audio file URL which the user loads from the playlist into the deck
    std::string str;
    std::getline(file, str);
    std::string URL = "file:///" + str; //adds file:/// to the audio file URL, which converts the URL into a file
    DBG(URL);
    juce::URL audioURL{ URL };
    player->loadURL(audioURL); //Loads the file into the DJAudioPlayer
    waveformDisplay.loadURL(audioURL); //Loads the file and returns the waveform of the audio file
    DBG(audioURL.getFileName());

    mtrackName = audioURL.getFileName(); //Gets the file name from the URL

    trackName.setText(mtrackName, juce::NotificationType::dontSendNotification);
}
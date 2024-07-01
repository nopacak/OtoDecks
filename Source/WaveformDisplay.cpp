/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 21 Feb 2024 8:06:45pm
    Author:  ndoks

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse) : 
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0)

{
    audioThumb.addChangeListener(this);
}


WaveformDisplay::~WaveformDisplay()
{
}



void WaveformDisplay::paint (juce::Graphics& g)
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
    if (fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        g.setColour(juce::Colours::aquamarine);
        g.drawRect(position * getWidth(), 0, getWidth()/40, getHeight());
    }
    else
    {
        g.setFont(18.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);
    }
    
}


void WaveformDisplay::resized()
{
}


void WaveformDisplay::loadURL(juce::URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded)
    {
        repaint();
    }
    else
    {
        std::cout << "File not loaded..." << std::endl;
    }
}


void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}


void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }   
}
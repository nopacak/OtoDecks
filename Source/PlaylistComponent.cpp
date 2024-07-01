/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 22 Feb 2024 12:35:53am
    Author:  ndoks

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <string>
#include <time.h>


PlaylistComponent::PlaylistComponent()
{

    tableComponent.getHeader().addColumn("Track title", 1, 300);
    tableComponent.getHeader().addColumn("Duration(hhmmss)", 2, 100);
    tableComponent.getHeader().addColumn("Load into", 3, 150);
    tableComponent.getHeader().addColumn("Load into", 4, 150);
    tableComponent.getHeader().addColumn("Delete", 5, 100);

    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    addAndMakeVisible(libraryLoadButton);
    addAndMakeVisible(librarySearch);
    libraryLoadButton.addListener(this);
    formatManager.registerBasicFormats();
    librarySearch.addListener(this);

    //creates a text file named playlist.txt and inserts a placeholder "0" as to which deckGUI to be put into. This is for loading playlist files into either deck 1 or 2
    std::ofstream myfile("playlist.txt");
    myfile << stdString << std::endl << "0" << std::endl;
    myfile.close();

    //creats a text file named PlaylistHistory.txt, which stores the current playlist audio file URLs so that the playlist data persists when reopening
    std::ifstream file("PlaylistHistory.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }

    //If text file contains audio file URLs, add each line into existing files array
    for (int i = 0; i < lines.size(); ++i) 
    {
        existingFiles.push_back(lines[i]);
    }

    //If there are existing files before
    if (existingFiles.size() != 0) 
    {

        //adds existing files into my files
        for (int i = 0; i < existingFiles.size(); ++i) {
            myFiles.add(juce::File{ existingFiles[i] });

        }

        for (int i = 0; i < myFiles.size(); ++i) 
        { //for every file, get the file name and duration and prepares them for reading

            fileName = juce::URL::removeEscapeChars(juce::URL{ myFiles[i] }.getFileName()); //gets file name
            fileNames.add(fileName); //adds file name into filenames array, to be displayed in playlist table component
            juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]); //creates reader for each file
            duration = reader->lengthInSamples / reader->sampleRate; //gets duration of the file by dividing length by sample rate
            myFilesDuration.add(duration); //adds duration into duration array to be displayed in playlist table component

        }
        tableComponent.updateContent(); //updates the table component to display the file names and duration
    }

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (juce::Colours::darkslategrey);   // clear the background

    g.setColour (juce::Colours::darkslategrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::thistle);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 7;
    libraryLoadButton.setBounds(0, 0, getWidth() / 2, rowH);
    librarySearch.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);
    tableComponent.setBounds(0, rowH, getWidth(), getHeight());

    //playlist header columns
    tableComponent.getHeader().setColumnWidth(1, (getWidth() / 8) * 3);
    tableComponent.getHeader().setColumnWidth(2, getWidth() / 8);
    tableComponent.getHeader().setColumnWidth(3, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(4, (getWidth() / 8) * 1.5);
    tableComponent.getHeader().setColumnWidth(5, (getWidth() / 8));

    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colours::darkslategrey);

    //search bar
    librarySearch.setTextToShowWhenEmpty("Search for tracks...", juce::Colours::thistle);
    librarySearch.setFont(20.0f);
}


int PlaylistComponent::getNumRows()
{
    return fileNames.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::thistle);
    }
    else
    {
        g.fillAll(juce::Colours::darkslategrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
                                    int rowNumber,
                                    int columnId,
                                    int width,
                                    int height,
                                    bool rowIsSelected)
{
    if (columnId == 1) 
    {//column 1 shows the file name of the audio file
        g.drawText(fileNames[rowNumber],
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
    if (columnId == 2) 
    {// column 2 shows the file's duration, in the format of hh:mm:ss
        std::time_t seconds(myFilesDuration[rowNumber]);
        tm* p = gmtime(&seconds);
        std::string hhmmss;
        std::string hours;
        std::string mins;
        std::string secs;
        //converting file duration from double into seconds and then into hours, minutes and seconds
        if (p->tm_hour < 10) 
        {
            hours = "0" + std::to_string(p->tm_hour);
        }
        else 
        {
            hours = std::to_string(p->tm_hour);
        }
        if (p->tm_min < 10) 
        {
            mins = "0" + std::to_string(p->tm_min);
        }
        else 
        {
            mins = std::to_string(p->tm_min);
        }
        if (p->tm_sec < 10) 
        {
            secs = "0" + std::to_string(p->tm_sec);
        }
        else 
        {
            secs = std::to_string(p->tm_sec);
        }

        hhmmss = hours + ":" + mins + ":" + secs;
        g.drawText(hhmmss,
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
}


juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    if (columnId == 3) 
    {//column 3 is a button that loads the audio file into deck 1
        if (existingComponentToUpdate == nullptr) 
        {
            juce::TextButton* btn = new juce::TextButton{ "Deck 1" }; //creates button with deck 1 as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id); //set component id to button for identifying which button was clicked
            btn->addListener(this); //adding listener to the button
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadFileIntoDeck1(); }; //loadFileIntoDeck1() runs when button is clicked
        }
    }
    if (columnId == 4) 
    {//column 4 is a button that loads the audio file into deck 2
        if (existingComponentToUpdate == nullptr) 
        {
            juce::TextButton* btn = new juce::TextButton{ "Deck 2" }; //creates button with deck 2 as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {loadFileIntoDeck2(); }; //loadFileIntoDeck2() runs when button is clicked
        }
    }
    if (columnId == 5) 
    {//column 5 is a button that deletes the audio file
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* btn = new juce::TextButton{ "Delete" }; //creates button with delete as the text
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteFile(); };//deleteFile() runs when button is clicked
        }

    }
    return existingComponentToUpdate;
}


void PlaylistComponent::buttonClicked(juce::Button * button) 
{

    if (button == &libraryLoadButton) 
    {//If button is clicked and matchs the library load button
        //lets the user choose for one or multiple files at once

        auto folderChooserFlags = juce::FileBrowserComponent::canSelectMultipleItems;
        choosers.launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
            {
                myFiles = choosers.getResults(); //myFiles is an array of files
                for (int i = 0; i < myFiles.size(); ++i)
                { //for loop repeats for myFiles size

                    if (existingFiles.size() != 0 || i == 0)
                    { //if there are existing files, or it is the first iteration of the loop
                        for (int j = 0; j < existingFiles.size(); ++j)
                        { //loops through number of times based on how many existing files there is
                            if (myFiles[i].getFullPathName().toStdString() == existingFiles[j])
                            { //loops through the existing files, if any matches, do not add the index
                                DBG("There are duplicates");

                                //creates alert window alerting user they have added a duplicate file, and it will not be added
                                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::InfoIcon,
                                    "Duplicate file added",
                                    "Duplicated file " + juce::URL::removeEscapeChars(juce::URL{ myFiles[i] }.getFileName()) + " has not been added",
                                    "Continue",
                                    nullptr);
                            }
                        }
                    }

                    if (i != duplicateIndex)
                    { //If the file is not a duplicated file index, add the file
                        fileName = juce::URL::removeEscapeChars(juce::URL{ myFiles[i] }.getFileName());  //get the file name and adds them to the fileNames array for display
                        fileNames.add(fileName);
                        juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]);
                        duration = reader->lengthInSamples / reader->sampleRate;   //gets duration of the file by dividing the length in samples by the sample rate
                        myFilesDuration.add(duration);   //adds duration into myFilesDuration, which is used in paint cell
                        DBG(fileNames[i]);
                        DBG(duration);

                        existingFiles.push_back(myFiles[i].getFullPathName().toStdString()); //file path is added to the existingFiles array
                    }
                }

                std::ofstream history("PlaylistHistory.txt"); //Opens PlaylistHistory.txt file

                for (int i = 0; i < existingFiles.size(); ++i)
                { //For each file in existingFiles
                    history << existingFiles[i] << std::endl; //Writes the added file path into the text file
                }
                history.close(); //closes the text file

                tableComponent.updateContent();

            });

    }
    
    rowclicked = button->getComponentID().getDoubleValue(); //gets which row(file) the user has clicked, used for deleting or adding files into deck 1/2

}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor & editor) { //Search bar input

    searchInput = librarySearch.getText(); //Gets the text which the user has typed in the search bar

    for (int i = 0; i < fileNames.size(); ++i) 
    {//Loops through all the files names of existing files
        if (fileNames[i].containsWholeWord(searchInput) && searchInput != "") 
        { // Matches the user's search input with any of the file names
            tableComponent.selectRow(i, false, true); //Selects the row which has a match
        }
    }

}

void PlaylistComponent::deleteFile() {//only runs when user clicks on the delete button on a file

    DBG(rowclicked);

    for (int i = 0; i < myFiles.size(); ++i) 
    {//loops through myFiles
        if (rowclicked == i) 
        { //matches row(file) which the user clicked on

            DBG("deletefile");
            fileNames.remove(i); //Removes file name from fileNames array
            myFilesDuration.remove(i); //Removes file duration from myFilesDuration array
            existingFiles.erase(existingFiles.begin() + i); //Removes the file from the existingFiles array
        }
    }
    //Opens the PlaylistHistory text file, to remove the deleted file and update the current existing files
    std::ofstream history("PlaylistHistory.txt");
    for (int i = 0; i < existingFiles.size(); ++i) 
    {
        history << existingFiles[i] << std::endl;
    }
    history.close();

    tableComponent.updateContent();
}


void PlaylistComponent::loadFileIntoDeck1() 
{ //only runs when user clicks on "Deck 1" button on the file
    for (int i = 0; i < myFiles.size(); ++i) 
    { //Loops through myFiles
        juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]); //creates reader for the file
        if (rowclicked == i) 
        { //Matches the row(file) which the user clicked to be added into Deck 1
            DBG("loadintodeck1");
            DBG(myFiles[i].getFileName());
            DBG(reader->getFormatName());
            fileToDeckGUI = myFiles[i].getFullPathName();//Gets the path of the file
            stdString = fileToDeckGUI.toStdString();//Converts the path from juce string into std string
            std::replace(stdString.begin(), stdString.end(), '\\', '/');//Replaces all \ in path to /, as reading the path into a file requires back slash"/"
            DBG(stdString);

            std::ofstream myfile("playlist.txt");//Opens text file named playlist.txt
            myfile << stdString << std::endl << "1" << std::endl; //writes the path of the file, and "1" into the text file, indicating loading into deck 1
            myfile.close(); //closes the text file
        }
    }
}


void PlaylistComponent::loadFileIntoDeck2() 
{ //only runs when user clicks on "Deck 2" button on the file
    for (int i = 0; i < myFiles.size(); ++i) 
    {
        juce::AudioFormatReader* reader = formatManager.createReaderFor(myFiles[i]);
        if (rowclicked == i) 
        {
            DBG("loadintodeck2");
            DBG(myFiles[i].getFileName());
            DBG(reader->getFormatName());
            fileToDeckGUI = myFiles[i].getFullPathName();
            stdString = fileToDeckGUI.toStdString();
            std::replace(stdString.begin(), stdString.end(), '\\', '/');
            DBG(stdString);
            std::ofstream myfile("playlist.txt");
            myfile << stdString << std::endl << "2" << std::endl; //writes the path of the file, and "2" into the text file, indicating loading into deck 2
            myfile.close();
        }
    }
}
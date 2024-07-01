OtoDecks application is a basic DJ application that utilizes two decks that can
simultaneously play two tracks at the same time and includes functionalities
which enable track mixing.
Basic program included Play, Stop and a Load button, Waveform display as well as
3 slider components for volume, speed and track position.
Play button initiates audio file that was loaded into a deck via a load button,
which, upon clicking, opens a file browser and allows for a single file selection,
while the stop button stops the execution of the audio file that was playing.
Waveform display paints a waveform of the audio file that is being loaded into the
deck.
Slider controls allows us to increase or decrease the volume and the speed of the
audio file that is currently playing.
Basic program also includes a table view of the playlist component which has 5
columns. First one displays track title, second the duration of the audio file in
hh:mm:ss format, third and fourth include buttons for loading the audio file in
either of the decks displayed and the fifth one contains a delete button which,
upon clicking, removes the audio file from the playlist table.
Additional components added are:
- Load into playlist button which, upon clicking on it, opens a file browser
that allows multiple file selections and then prints them out in the playlist
table correctly filling up the table columns.
- Search for tracks text input field which highlights the row of the track we
are searching for by the track name.
- Loop checkbox which loops the audio file that is being played within the
deck, playing it again from the beginning once the track reaches its end.
- Track name display text box which prints out the track name that is being
loaded into the deck.

Basic application developed throughout the module course had the following
outline: upper half of the window frame was vertically split in two and facilitated
two deck gui components which had play and stop button, followed by three
slider components, then a waveform display and load button at the bottom of it.
Lower half of the window was dedicated to a playlist table component, which was
split into two columns, one that displayed track title, second containing play
buttons, as shown in the screenshot of the lecture video captured:
![image](https://github.com/nopacak/OtoDecks/assets/65850108/3c2538b0-3247-449a-ae25-26b5db62d1ec)

I kept the original positioning of the deck gui and the playlist component and did
slight modifications in the positioning and the shape of the elements as well as
changed colour scheme (darkslategrey for the background and thistle for playlist
row highlight, waveform and text colour). Component positioning is as follows:
first row of the deck gui displays the track name if the track is loaded into the
deck, otherwise it shows “No track loaded” text. Second row has two sliders,
volume and speed, in rotary shape. Following is the waveform display component
which paints a waveform of the audio file loaded into the deck, otherwise displays
“File not loaded” text message. Last row of the deck gui contains play, stop and
load buttons, as well as the loop checkbox.
Playlist table component has load into playlist button and search bar on the top of
it, followed by 5 columns for track title, track duration, load into and delete
buttons, which all load into the rows once the files are being selected from the file
browser window. Screenshot:
![image](https://github.com/nopacak/OtoDecks/assets/65850108/aad6dea0-c213-45d5-8442-29c5fb326d3d)

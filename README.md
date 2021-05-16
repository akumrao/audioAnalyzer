# Audio Analyzer

Small library using SDL2 and SDL_TTF used to analyze audio stream and  plot points into a 2D graph at webpages.

It renders graph using webassemply at HTML5 canvas. Which makes it very fast.
It displays Audio, FFT, Tonality, Spreading Function, SNR, SMR etc




Current supported formats:

* WAV, RTP packet, webrtc analysis

Author
------

AudioAnalyser is written and maintained by Arvind Umrao.


Class

Plot
Small library using SDL2 and SDL_TTF used to plot points into a 2D graph at webpages

Audio
A simple C++ library for reading and writing audio files. 

RTPStream
Work on progress

-----

### Plot
        coordinate_list = push_back_coord(coordinate_list, 0, -5, -2);
        coordinate_list = push_back_coord(coordinate_list, 0, -4, -9);
        coordinate_list = push_back_coord(coordinate_list, 0, -3, -9);
        coordinate_list = push_back_coord(coordinate_list, 0, -2, -3);
        coordinate_list = push_back_coord(coordinate_list, 0, 0, 0);
        coordinate_list = push_back_coord(coordinate_list, 0, 0, 9);
        coordinate_list = push_back_coord(coordinate_list, 0, 1, 5);
        coordinate_list = push_back_coord(coordinate_list, 0, 2, 12);
        coordinate_list = push_back_coord(coordinate_list, 0, 3, 18);
        coordinate_list = push_back_coord(coordinate_list, 0, 4, 3);


        //populate plot parameter object
        plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list);
        params->scale.x = 1;
        params->scale.y = 2;
        params->max.x = +5;
        params->max.y = 20;
        params->min.x = -5;
        params->min.y = -20;

        //  Plot_Window_params win_param;
        plotwin_list = push_back_plot_win(plotwin_list, params);
  	int ret = plot.plot_graph(plotwin_list, "Plot");

	if (ret == EXIT_FAILURE) {
		printf("plot_graph return with status %d\n", ret);
		return EXIT_FAILURE;
	}

### Create an AudioFile object:

	#include "AudioFile.h"

	AudioFile<double> audioFile;
		
### Load an audio file:

	audioFile.load ("/path/to/my/audiofile.wav");
	
### Get some information about the loaded audio:

	int sampleRate = audioFile.getSampleRate();
	int bitDepth = audioFile.getBitDepth();
	
	int numSamples = audioFile.getNumSamplesPerChannel();
	double lengthInSeconds = audioFile.getLengthInSeconds();
	
	int numChannels = audioFile.getNumChannels();
	bool isMono = audioFile.isMono();
	bool isStereo = audioFile.isStereo();
	
	// or, just use this quick shortcut to print a summary to the console
	audioFile.printSummary();
	
-----------------

AudioFile is a template class and so it can be instantiated using floating point precision:

	AudioFile<float> audioFile;

...	or double precision:

	AudioFile<double> audioFile;
	

Tools 
http://onlinetonegenerator.com/time-stretcher.html

In psychoacoustics, a pure tone is a sound with a sinusoidal waveform; this is, a sine wave of any frequency, phase, and amplitude.[1]

A sine wave is characterized by its frequency (the number of cycles per second), its amplitude (the strength of each cycle), and its phase shift (which indicates the time alignment relative to a zero-time reference point). A pure tone has the property – unique among real-valued wave shapes – that its wave shape is unchanged by linear time-invariant systems; that is, only the phase and amplitude change between such a system's pure-tone input and its output.

Sine and cosine waves can be used as basic building blocks of more complex waves. A pure tone of any frequency and phase can be decomposed into, or built up from, a sine wave and a cosine wave of that frequency. As additional sine waves having different frequencies are combined, the waveform transforms from a sinusoidal shape into a more complex shape.

Sound localization is often more difficult with pure tones than with other sounds.[2


to compile

git clone https://github.com/emscripten-core/emsdk.git

 cd emsdk
./emsdk install latest

./emsdk activate latest


root:emsdk# emcc


make 

make wasm

chmod +x servewasm.py
./servewasm.py


License
-------

Copyright (c) 2019 Arvind Umrao

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.



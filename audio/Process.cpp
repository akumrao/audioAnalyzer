/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.cpp
 * Author: root
 * 
 * Created on April 28, 2019, 11:47 PM
 */

#include "Process.h"
#include  <iostream>

Process::Process(const char * filePath) : pBuffer(NULL), samplesPerFrame(0), outputFile(filePath, std::ios::binary) {

    if (!outputFile.is_open()) {

        std::cout << "could not able to create mp3 file " << filePath << std::endl;
    }

}

Process::~Process() {


}

void Process::Exit() {
    
    if(pBuffer)
    {
        int encodedChunkSize = codecFlush (pBuffer);
        if (encodedChunkSize != 0)
                 outputFile.write(pBuffer, encodedChunkSize);
   
         encodedChunkSize = codecExit(pBuffer);
        if (encodedChunkSize != 0)
            outputFile.write(pBuffer, encodedChunkSize);

        free(pBuffer);

        outputFile.close();

       

        pBuffer = NULL;
    
     }

}

void Process::Init(int sfreq, int mode) {

    CodecInitIn sCodec;

    sCodec.frequency = sfreq;
    sCodec.mode = mode;


    if (sCodec.bitrate == -1) {
        if (sCodec.mode == 3)
            sCodec.bitrate = 64;
        else
            sCodec.bitrate = 128;
    }


    pCodecInfo = codecInit(&sCodec);


    samplesPerFrame = pCodecInfo->nSamples;

    pBuffer = (char *) malloc(pCodecInfo->bufferSize);

}

unsigned int Process::EncodeChunk(AudioBuffer& newBuffer) {

    
     static int inc =0 ;

 
    
    if (inc + samplesPerFrame < newBuffer.size())
    {
        int encodedChunkSize = codecEncodeChunk(samplesPerFrame, &newBuffer[inc], pBuffer);
        if (encodedChunkSize != 0)
            outputFile.write(pBuffer, encodedChunkSize);
     
         inc = inc + samplesPerFrame;
    }
    else
    {
         Exit();
    }

    
    return 0;
    
    
}


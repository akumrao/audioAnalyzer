/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Process.h
 * Author: root
 *
 * Created on April 28, 2019, 11:47 PM
 */

#ifndef PROCESS_H
#define PROCESS_H

typedef short int		int16_t;
#include <vector>
#include <fstream>

using namespace std;

#include	"encoder/common.h"
#include	"encoder/codec.h"

//#include "Common.h"
///#include "Psycho_anal.h"
//#include "Encoder.h"
/* Header Information Structure */



class Process {
public:
    
    
    typedef std::vector<int16_t>  AudioBuffer;
    
     typedef std::vector<float>  AudioBuffer1;
     typedef std::vector<double>  AudioBuffer2;
     
     
    Process(const char * filePat);

    virtual ~Process();
    unsigned int EncodeChunk(AudioBuffer& newBuffer);
    unsigned int EncodeChunk(AudioBuffer1& newBuffer){};
    unsigned int EncodeChunk(AudioBuffer2& newBuffer){};

    void Init(int sfreq, int mode);
    void Exit();
private:
    
    	CodecInitOut			*pCodecInfo;
	char					*pBuffer ;   /* Initialized just to prevent compiler warnings */
    
private:
    int						samplesPerFrame ; 
    std::ofstream outputFile;
};

#endif /* PROCESS_H */


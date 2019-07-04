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

Process::Process() {
}

Process::Process(const Process& orig) {
}

Process::~Process() {
}

void Process::rebuffer_audio(short buffer[2][1152], short * insamp, unsigned int samples_read, int stereo){
    unsigned int j;

    if (stereo == 2)
    {

        for (j = 0; j < samples_read / 2; j++)
        {
            buffer[0][j] = insamp[2 * j];
            buffer[1][j] = insamp[2 * j + 1];
        }
    } else
    {
        for (j = 0; j < samples_read; j++)
        {
            buffer[0][j] = insamp[j];
            buffer[1][j] = 0;
        }
    }

    for (; j < 1152; j++)
    {
        buffer[0][j] = 0;
        buffer[1][j] = 0;
    }

    return;
}


unsigned int Process::codecEncodeChunk(int nSamples, short * pSamples, char * pDest){

    static double xr[2][2][576];
    static double xr_dec[2][2][576];
    static double pe[2][2];
    static int l3_enc[2][2][576];

    int gr, ch;
    int mean_bits, sideinfo_len;
    int bitsPerFrame;
    int j;
    
    pEncodedOutput = pDest;
    outputBit = 8;
    pEncodedOutput[0] = 0;

    rebuffer_audio(buffer, pSamples, nSamples, stereo);

    if (frac_SpF != 0)
    {
        if (slot_lag > (frac_SpF - 1.0))
        {
            slot_lag -= frac_SpF;
            extra_slot = 0;
            info.padding = 0;
        } else
        {
            extra_slot = 1;
            info.padding = 1;
            slot_lag += (1 - frac_SpF);
        }
    }


    bitsPerFrame = 8 * whole_SpF + (info.padding * 8);

    /*		determine the mean bitrate for main data */

    sideinfo_len = 32;

    if (stereo == 1)
        sideinfo_len += 136;
    else
        sideinfo_len += 256;

    if (info.error_protection)
        sideinfo_len += 16;
    mean_bits = (bitsPerFrame - sideinfo_len) / 2;

    /*		psychoacoustic model */

    for (gr = 0; gr < 2; gr++)
        for (ch = 0; ch < stereo; ch++)
        {
            //psycho_anal(&buffer[ch][gr * 576], &sam[ch][0], ch, 3, snr32, &ratio.l[gr][ch][0],
             //       &ratio.s[gr][ch][0], &pe[gr][ch], &l3_side.gr[gr].ch[ch].tt);

        }

    
 return 0;   
}
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

#include "Common.h"

/* Header Information Structure */

typedef struct {
    int version;
    int error_protection;
    int bitrate_index;
    int sampling_frequency;
    int padding;
    int extension;
    int mode;
    int mode_ext;
    int copyright;
    int original;
    int emphasis;
/*		int	freq; */
} layer, *the_layer;

typedef struct {
	double	l[2][2][21];
	double	s[2][2][12][3];
} III_psy_ratio;

typedef struct {
	int main_data_begin; /* unsigned -> int */
	unsigned private_bits;
	int resvDrain;
	unsigned scfsi[2][4];
	struct {
		struct gr_info_s {
			gr_info tt;
			} ch[2];
		} gr[2];
	} III_side_info_t;

class Process {
public:
    Process();
    Process(const Process& orig);
    virtual ~Process();
    void rebuffer_audio(short buffer[2][1152], short * insamp, unsigned int samples_read, int stereo);
    unsigned int codecEncodeChunk(int nSamples, short * pSamples, char * pDest);
private:
    char * pEncodedOutput;
    int outputBit;
     short buffer[2][1152];
     int stereo;
     double frac_SpF, slot_lag;
     int whole_SpF, extra_slot;
     layer info;
     short sam[2][1344];
     float snr32[32];
     III_psy_ratio ratio;
      III_side_info_t l3_side;
};

#endif /* PROCESS_H */


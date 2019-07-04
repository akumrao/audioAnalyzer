/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Common.h
 * Author: root
 *
 * Created on April 28, 2019, 11:25 PM
 */

#ifndef COMMON_H
#define COMMON_H
#include <cmath>


#define BLKSIZE         1024
#define         PI                      3.14159265358979

#define  RING_BUFFER 1

#define         LN_TO_LOG10             0.2302585093


typedef struct {
	unsigned part2_3_length;
	unsigned big_values;
	unsigned count1;
 	unsigned global_gain;
	unsigned scalefac_compress;
	unsigned window_switching_flag;
	unsigned block_type;
	unsigned mixed_block_flag;
	unsigned table_select[3];
int /*	unsigned */ subblock_gain[3];
	unsigned region0_count;
	unsigned region1_count;
	unsigned preflag;
	unsigned scalefac_scale;
	unsigned count1table_select;

	unsigned part2_length;
	unsigned sfb_lmax;
	unsigned sfb_smax;
	unsigned address1;
	unsigned address2;
	unsigned address3;
	double quantizerStepSize;
	/* added for LSF */
	unsigned *sfb_partition_table;
	unsigned slen[4];
} gr_info;


#endif /* COMMON_H */


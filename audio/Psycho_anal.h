/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Psycho_anal.h
 * Author: root
 *
 * Created on April 28, 2019, 7:11 PM
 */
#ifndef PSYCHO_ANAL_H
#define PSYCHO_ANAL_H

#define maximum(x,y) ( (x>y) ? x : y )
#define minimum(x,y) ( (x<y) ? x : y )

/* #define CBANDS 63 */
#define CBANDS_s 42
#define BLKSIZE_s 256
#define HBLKSIZE_s 129
#define TCBMAX_l 63
#define TCBMAX_s 42
#define SBMAX_l 21
#define SBMAX_s 12

/* #define switch_pe        1800 */
#define NORM_TYPE       0
#define START_TYPE      1
#define SHORT_TYPE      2
#define STOP_TYPE       3

#include "Common.h"
#include "../plot/plot.h"

/* Psychoacoustic Model 2 Definitions */

#define LOGBLKSIZE      10

#define HBLKSIZE        513
#define CBANDS          63
#define LXMIN           32.0



/* Psychoacoustic Model 2 Type Definitions */

typedef int        ICB[CBANDS];
typedef int        IHBLK[HBLKSIZE];
typedef float      F32[32];
typedef float      F2_32[2][32];
typedef float      FCB[CBANDS];
typedef float      FCBCB[CBANDS][CBANDS];

typedef float      FBLK[BLKSIZE];
typedef float      FHBLK[HBLKSIZE];
typedef float      F2HBLK[2][HBLKSIZE];
typedef float      F22HBLK[2][2][HBLKSIZE];
typedef double     DCB[CBANDS];






class Psycho_anal {
public:
    Psycho_anal();
    Psycho_anal(double sfreq);
    
    Psycho_anal(const Psycho_anal& orig);
    virtual ~Psycho_anal();
    
    void psycho_anal( short int *buffer, short int savebuf[1344], int chn, int lay, float snr32[32],
				  double ratio_d[SBMAX_l], double ratio_ds[SBMAX_s][3], double *pe, gr_info *cod_info );
    
    
    void L3para_read(int sfreq, int *numlines, int *partition_l, double *minval,
        double *qthr_l, double *norm_l, double (*s3_l)[63], int *partition_s,
        double *qthr_s, double *norm_s, double *SNR, int *cbw_l, int *bu_l,
        int *bo_l, double *w1_l, double *w2_l, int *cbw_s, int *bu_s,
        int *bo_s, double *w1_s, double *w2_s);
private:
    
    /*____ Global Static Variables ______________________________________________*/

 double ratio[2][SBMAX_l];
 double ratio_s[2][SBMAX_s][3];

/* The  variables "r", "phi_sav", "new_", "old" and "oldest" have    */
/* to be remembered for the unpredictability measure.  For "r" and        */
/* "phi_sav", the first index from the left is the channel select and     */
/* the second index is the "age" of the data.                             */


 float window_s[BLKSIZE_s];
 int new_, old, oldest;
 int flush, sync_flush, syncsize;
#if RING_BUFFER==1
 int savebuf_start_idx[2];
#endif
 double cw[HBLKSIZE], eb[CBANDS];
 double ctb[CBANDS];
 double SNR_l[CBANDS], SNR_s[CBANDS_s];
 double minval[CBANDS], qthr_l[CBANDS], norm_l[CBANDS];
 double qthr_s[CBANDS_s], norm_s[CBANDS_s];
 double nb_1[2][CBANDS], nb_2[2][CBANDS];
 double s3_l[CBANDS][CBANDS];

/* Scale Factor Bands */
 int cbw_l[SBMAX_l], bu_l[SBMAX_l], bo_l[SBMAX_l];
 int cbw_s[SBMAX_s], bu_s[SBMAX_s], bo_s[SBMAX_s];
 double w1_l[SBMAX_l], w2_l[SBMAX_l];
 double w1_s[SBMAX_s], w2_s[SBMAX_s];
 double en[SBMAX_l], thm[SBMAX_l];
 int blocktype_old[2];
 int partition_l[HBLKSIZE], partition_s[HBLKSIZE_s];


/* The following pointer variables point to large areas of memory         */
/* dynamically allocated by the mem_alloc() function.  Dynamic memory     */
/* allocation is used in order to avoid stack frame or data area          */
/* overflow errors that otherwise would have occurred at compile time     */
/* on the Macintosh computer.                                             */

 float energy_s[3][HBLKSIZE_s];
 float phi_s[3][HBLKSIZE_s];

 int *numlines;
 float *window;
 F2HBLK *r, *phi_sav;
 
public:
};

#endif /* PSYCHO_ANAL_H */


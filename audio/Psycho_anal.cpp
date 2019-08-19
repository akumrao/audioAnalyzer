/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Psycho_anal.cpp
 * Author: root
 * 
 * Created on April 28, 2019, 7:11 PM
 */

#include "Psycho_anal.h"
#include <cstdlib>
#include "Fft.h"
#include "Tables.h"
#include "SaveJson.h"
#include <cstring>

Psycho_anal::Psycho_anal() {
}

Psycho_anal::Psycho_anal(double sfreq) {
    
    Init(sfreq);
}
void Psycho_anal::Init(double sfreq) {
    unsigned int i, j, k;


    old = 1 - (new_ = oldest = 0);

    for (i = 0; i < 2; i++) {
        for (j = 0; j < SBMAX_l; j++)
            ratio[i][j] = 0.0;
        for (j = 0; j < SBMAX_s; j++)
            for (k = 0; k < 2; k++)
                ratio_s[i][j][k] = 0.0;
    }

    /* These dynamic memory allocations simulate "static" variables placed    */
    /* in the data space.  Each mem_alloc() call here occurs only once at     */
    /* initialization time.  The mem_free() function must not be called.      */

    int z = sizeof (ICB);
    int x = sizeof (FBLK);
    
    numlines = (int *) std::malloc(sizeof (ICB));
    window = (float *) std::malloc(sizeof (FBLK));
    r = (F2HBLK *) std::malloc(sizeof (F22HBLK));
    phi_sav = (F2HBLK *) std::malloc(sizeof (F22HBLK));

    
    
 /*    double cw[HBLKSIZE], eb[CBANDS];
 double ctb[CBANDS];
 double SNR_l[CBANDS], SNR_s[CBANDS_s];
 double minval[CBANDS], qthr_l[CBANDS], norm_l[CBANDS];
 double qthr_s[CBANDS_s], norm_s[CBANDS_s];
 double nb_1[2][CBANDS], nb_2[2][CBANDS];
 double s3_l[CBANDS][CBANDS];


 int cbw_l[SBMAX_l], bu_l[SBMAX_l], bo_l[SBMAX_l];
 int cbw_s[SBMAX_s], bu_s[SBMAX_s], bo_s[SBMAX_s];
 double w1_l[SBMAX_l], w2_l[SBMAX_l];
 double w1_s[SBMAX_s], w2_s[SBMAX_s];
 double en[SBMAX_l], thm[SBMAX_l];
 int blocktype_old[2];
 int partition_l[HBLKSIZE], partition_s[HBLKSIZE_s];
 */

    std::memset(norm_s, 0, sizeof (norm_s));
    std::memset(ctb, 0, sizeof (ctb));
    std::memset(eb, 0, sizeof (eb));
    std::memset(cbw_l, 0, sizeof (cbw_l));

    std::memset(SNR_l, 0, sizeof (SNR_l));
    std::memset(SNR_s, 0, sizeof (SNR_s));

    std::memset(minval, 0, sizeof (minval));
    std::memset(qthr_l, 0, sizeof (qthr_l));


    std::memset(norm_l, 0, sizeof (norm_l));
    std::memset(qthr_s, 0, sizeof (qthr_s));


    std::memset(cbw_l, 0, sizeof (cbw_l));
    std::memset(bu_l, 0, sizeof (bu_l));
    std::memset(bo_l, 0, sizeof (bo_l));
    std::memset(bo_s, 0, sizeof (bo_s));
    std::memset(bu_s, 0, sizeof (bu_s));
    std::memset(cbw_s, 0, sizeof (cbw_s));


    std::memset(w2_l, 0, sizeof (w2_l));
    std::memset(w1_l, 0, sizeof (w1_l));
    std::memset(w2_s, 0, sizeof (w2_s));
    std::memset(w1_s, 0, sizeof (w1_s));
    std::memset(thm, 0, sizeof (thm));
    std::memset(en, 0, sizeof (en));

    std::memset(partition_s, 0, sizeof (partition_s));


    std::memset(partition_l, 0, sizeof (partition_l));
    
            
    sync_flush = 768;
    flush = 576;
    syncsize = 1344; /* sync_flush + flush */

#if RING_BUFFER==1
    for (i = 0; i < 2; i++)
        savebuf_start_idx[i] = 0;
#endif

    /* calculate HANN window coefficients */
    for (i = 0; i < BLKSIZE; i++)
        window[i] = 0.5 * (1 - cos(2.0 * PI * (i - 0.5) / BLKSIZE));
    for (i = 0; i < BLKSIZE_s; i++)
        window_s[i] = 0.5 * (1 - cos(2.0 * PI * (i - 0.5) / BLKSIZE_s));
    
    
  /*
    {
    
        captionlist caption_list = NULL;
        coordlist coordinate_list = NULL;
    
        caption_list = push_back_caption(caption_list, "HANN Window", 0, 0x0000FF);
        coordinate_list = push_back_coords(coordinate_list, 0, window, BLKSIZE );
    
        //plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list, 600, 400, {64,64 }, {BLKSIZE, BLKSIZE}, {0,0});

        // params->push_back( 0, window, BLKSIZE);
    
        plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list, 800, 800);
    
        //  Plot_Window_params win_param;
         push_back_plot_win(params); 
    
    }*/

    /* reset states used in unpredictability measure */
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            for (k = 0; k < HBLKSIZE; k++) {
                r[i][j][k] = 0.0;
                phi_sav[i][j][k] = 0.0;
            }


    L3para_read((int) sfreq, numlines, partition_l, minval, qthr_l, norm_l, s3_l,
            partition_s, qthr_s, norm_s, SNR_s,
            cbw_l, bu_l, bo_l, w1_l, w2_l, cbw_s, bu_s, bo_s, w1_s, w2_s);
    
    
    {
        captionlist caption_list = NULL;
        caption_list = push_back_caption(caption_list, "Energy", 0, 0x0000FF);
        params1 = new plot_params("x", "y", caption_list, NULL, 800, 400,{512, 1.41994e+07+15 },  { 0  , 453698});
       // params1 = new plot_params("x", "y", caption_list, NULL, 800, 400,{5, 3.9265e+5 },  { -5  , -3.9265e+5 });
        
        push_back_plot_win(params1);
    }
    
            
    {
        captionlist caption_list = NULL;
        caption_list = push_back_caption(caption_list, "Uncertinity", 0, 0x0000FF);
        params2 = new plot_params("x", "y", caption_list, NULL, 800, 400,{512, 2},  {   0, 0 });
        push_back_plot_win(params2);
    }
    
     {
        captionlist caption_list = NULL;
        caption_list = push_back_caption(caption_list, "UncertinityEN", 0, 0x0000FF);
        params3 = new plot_params("x", "y", caption_list, NULL, 800, 400,{63, 1.41994e+07+15},  {   0, 0 });
        push_back_plot_win(params3);
    }

    {
        captionlist caption_list = NULL;
        caption_list = push_back_caption(caption_list, "UncertinityEN", 0, 0x0000FF);
        params4 = new plot_params("x", "y", caption_list, NULL, 800, 400,{63, 1.41994e+07 + 15},
        {
            0, 0 });
        push_back_plot_win(params4);
    }
    

    {
        captionlist caption_list = NULL;
        caption_list = push_back_caption(caption_list, "UncertinityEN", 0, 0x0000FF);
        params5 = new plot_params("x", "y", caption_list, NULL, 800, 400,{63, 1.41994e+07 + 15},
        {
            0, 0 });
        push_back_plot_win(params5);
    }
      
  /*
    {
        captionlist caption_list = NULL;
        coordlist coordinate_list = NULL;
        caption_list = push_back_caption(caption_list, "Function of MinVal", 0, 0x0000FF);
        coordinate_list = push_back_coords(coordinate_list, 0, minval, 62 );
        plot_params *params = new plot_params( "partition", "SNR", caption_list, coordinate_list, 400, 400);
        push_back_plot_win( params); 
    }
            
      
    {
        captionlist caption_list = NULL;
        coordlist coordinate_list = NULL;
        caption_list = push_back_caption(caption_list, "Threshold of Quite", 0, 0x0000FF);
        coordinate_list = push_back_coords(coordinate_list, 0, qthr_l, 62 );
        plot_params *params = new plot_params( "partition", "qthr_l", caption_list, coordinate_list, 400, 400);
        push_back_plot_win( params); 
    }
 
     {
        captionlist caption_list = NULL;
        coordlist coordinate_list = NULL;
        caption_list = push_back_caption(caption_list, "Spreading Function", 0, 0x0000FF);
        //caption_list = push_back_caption(caption_list, "", 1, 0x0000FF);
        ///plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list, 600, 400, {64,64 }, {BLKSIZE, BLKSIZE}, {0,0});

        for( int x =0; x < 63; ++x)
        coordinate_list = push_back_coords(coordinate_list, x, s3_l[x], 62 );
      
        
        plot_params *params = new plot_params( "partition", "SpreadingFn", caption_list, coordinate_list, 800, 400);
        push_back_plot_win(params); 
    }
  
    
    {
        captionlist caption_list = NULL;
        coordlist coordinate_list = NULL;
        caption_list = push_back_caption(caption_list, "norm", 0, 0x0000FF);
        //caption_list = push_back_caption(caption_list, "", 1, 0x0000FF);
        ///plot_params *params = new plot_params("Time (s)", "Speed (Mbit/s)", caption_list, coordinate_list, 600, 400, {64,64 }, {BLKSIZE, BLKSIZE}, {0,0});

       
        coordinate_list = push_back_coords(coordinate_list, 0, norm_l, 62 );
      
        
        plot_params *params = new plot_params( "partition", "norm_l", caption_list, coordinate_list, 800, 400);
        push_back_plot_win(params); 
    }
    */

    /* Set unpredicatiblility of remaining spectral lines to 0.4 */
    for (j = 206; j < HBLKSIZE; j++)
        cw[j] = 0.4;

}

Psycho_anal::Psycho_anal(const Psycho_anal& orig) {
}

Psycho_anal::~Psycho_anal() {

    free(numlines);
    free(window);
    free(r);
    free(phi_sav);
}

/*____ L3para_read() __________________________________________________________*/

void Psycho_anal::L3para_read(int sfreq, int *numlines, int *partition_l, double *minval,
        double *qthr_l, double *norm_l, double (*s3_l)[63], int *partition_s,
        double *qthr_s, double *norm_s, double *SNR, int *cbw_l, int *bu_l,
        int *bo_l, double *w1_l, double *w2_l, int *cbw_s, int *bu_s,
        int *bo_s, double *w1_s, double *w2_s) {
    static double bval_l[CBANDS];
    int cbmax_tp;

    int sbmax;
    int i, j, k, k2;


    psyDataElem *rpa1;
    psyDataElem2 *rpa2;
    psyDataElem3 *rpa3;


    /* Read long block data */

    switch (sfreq) {
        case 32000: rpa1 = psy_longBlock__32000_58;
            cbmax_tp = 59;
            break;
        case 44100: rpa1 = psy_longBlock_44100_62;
            cbmax_tp = 63;
            break;
        case 48000: rpa1 = psy_longBlock_48000_61;
            cbmax_tp = 62;
            break;
        default: return; /* Just to avoid compiler warnings */
    }

/*
 ndex of the partition,
b
������������������ Lowest frequency line, wlow
b.
������������������ Highest frequency line, whigh
b
������������������ The median Bark value, bval
b
������������������ A lower limit for the SNR in the partition that controls
stereo unmasking effects, minval
b
������������������ The value for tonal masking noise for that partition, TMN
b
 */

    
    for (i = 0, k2 = 0; i < cbmax_tp; i++) {
        numlines[i] = rpa1->lines;
        minval[i] = rpa1->minVal;
        qthr_l[i] = rpa1->qthr;
        norm_l[i] = rpa1->norm;
        bval_l[i] = rpa1->bVal;
        rpa1++;

        for (k = 0; k < numlines[i]; k++)
            partition_l[k2++] = i;
    }


    /************************************************************************
     * Now compute the spreading function, s[j][i], the value of the spread-*
     * ing function, centered at band j, for band i, store for later use    *
     ************************************************************************/

    for (i = 0; i < cbmax_tp; i++) {
        double x, temp, tempx, tempy;
        for (j = 0; j < cbmax_tp; j++) {
            /*			tempx = (bval_l[i]-bval_l[j]) * 1.05; */
            if (j >= i)
                tempx = (bval_l[i] - bval_l[j]) * 3.0;
            else
                tempx = (bval_l[i] - bval_l[j]) * 1.5;
            /*			if (j>=i) tempx = (bval_l[j]-bval_l[i]) * 3.0;
                                    else      tempx = (bval_l[j]-bval_l[i]) * 1.5; */
            if (tempx > 0.5 && tempx < 2.5) {
                temp = tempx - 0.5;
                x = 8.0 * temp * (temp - 2.0);
            } else x = 0.0;
            tempx += 0.474;
            tempy = 15.811389 + 7.5 * tempx - 17.5 * sqrt(1.0 + tempx * tempx);
            if (tempy <= -60.0) s3_l[i][j] = 0.0;
            else s3_l[i][j] = exp((x + tempy) * LN_TO_LOG10);
        }
    }


    /* Read short block data */

    switch (sfreq) {
        case 32000: rpa2 = psy_shortBlock_32000_41;
            cbmax_tp = 42;
            break;
        case 44100: rpa2 = psy_shortBlock_44100_38;
            cbmax_tp = 39;
            break;
        case 48000: rpa2 = psy_shortBlock_48000_37;
            cbmax_tp = 38;
            break;
        default: return; /* Just to avoid compiler warnings */
    }

    for (i = 0, k2 = 0; i < cbmax_tp; i++) {
        numlines[i] = rpa2->lines;
        qthr_s[i] = rpa2->qthr;
        norm_s[i] = rpa2->norm;
        SNR[i] = rpa2->snr;
        rpa2++;

        for (k = 0; k < numlines[i]; k++)
            partition_s[k2++] = i;
    }


    /* Read long block data for converting threshold calculation
       partitions to scale factor bands */

    switch (sfreq) {
        case 32000: rpa3 = psy_data3_32000_20;
            break;
        case 44100: rpa3 = psy_data3_44100_20;
            break;
        case 48000: rpa3 = psy_data3_48000_20;
            break;
        default: return; /* Just to avoid compiler warnings */
    }
    sbmax = SBMAX_l;

    for (i = 0; i < sbmax; i++) {
        cbw_l[i] = rpa3->cbw;
        bu_l[i] = rpa3->bu;
        bo_l[i] = rpa3->bo;
        w1_l[i] = rpa3->w1;
        w2_l[i] = rpa3->w2;
        rpa3++;
    }


    /* Read short block data for converting threshold calculation
       partitions to scale factor bands */

    switch (sfreq) {
        case 32000: rpa3 = psy_data4_32000_11;
            break;
        case 44100: rpa3 = psy_data4_44100_11;
            break;
        case 48000: rpa3 = psy_data4_48000_11;
            break;
        default: return; /* Just to avoid compiler warnings */
    }
    sbmax = SBMAX_s;

    for (i = 0; i < sbmax; i++) {
        cbw_s[i] = rpa3->cbw;
        bu_s[i] = rpa3->bu;
        bo_s[i] = rpa3->bo;
        w1_s[i] = rpa3->w1;
        w2_s[i] = rpa3->w2;
        rpa3++;
    }
}



/*____ psycho_anal() ________________________________________________________*/

void Psycho_anal::psycho_anal(short int *buffer, short int savebuf[1344], int chn, int lay, float snr32[32],
        double ratio_d[SBMAX_l], double ratio_ds[SBMAX_s][3],
        double *pe, gr_info *cod_info){;
    int blocktype;
    unsigned int b, j, k;
    double r_prime, phi_prime; /* not FLOAT */
    double temp1, temp2, temp3;

    int sb, sblock;

    DCB thr;
    FCB cb, nb, ecb;
    FBLK wsamp_r, wsamp_i;
    FHBLK energy, phi;

#if RING_BUFFER==1
    int beg, idx, end;
#endif


    for (j = 0; j < SBMAX_l; j++)
        ratio_d[j] = ratio[chn][j];

    for (j = 0; j < SBMAX_s; j++)
        for (k = 0; k < 3; k++)
            ratio_ds[j][k] = ratio_s[chn][j][k];

    if (chn == 0)
        old = 1 - (new_ = oldest = old);

    /**********************************************************************
     *  Delay signal by sync_flush=768 samples                             *
     **********************************************************************/

#if RING_BUFFER==0
    for (j = 0; j < sync_flush; j++) /* for long window samples */
        savebuf[j] = savebuf[j + flush];
#else
    beg = savebuf_start_idx[chn] = (savebuf_start_idx[chn] + flush) % syncsize;
#endif

#if RING_BUFFER==0
    for (j = sync_flush; j < syncsize; j++)
        savebuf[j] = *buffer++;
#else
    idx = (beg + sync_flush) % syncsize;
    end = (idx + flush) % syncsize; /* beg */
    if (idx > end)
    {
        while (idx < syncsize)
            savebuf[idx++] = *buffer++;
        idx = 0;
    }
    while (idx < end)
        savebuf[idx++] = *buffer++;
#endif


    /**********************************************************************
     *    compute unpredicatability of first six spectral lines            * 
     **********************************************************************/

#if RING_BUFFER==0
    for (j = 0; j < BLKSIZE; j++)
    { /* window data with HANN window */
        wsamp_r[j] = window[j] * savebuf[j];
    }
#else
    j = 0;
    idx = beg;
    end = (idx + BLKSIZE) % syncsize;
    if (idx > end)
    {
        while (idx < syncsize)
        {
            wsamp_r[j] = window[j] * savebuf[idx++];
            j++;
        }
        idx = 0;
    }
    while (idx < end)
    {
        wsamp_r[j] = window[j] * savebuf[idx++];
        j++;
    }
#endif
    



    fft(wsamp_r, wsamp_i, energy, phi, BLKSIZE); /* long FFT */

    coordlist coordinate_list = NULL;
    params1->clean();
    params1->update = true;
   coordinate_list = push_back_coords(coordinate_list, 0, &energy[0] , 512);
    
    Pair max = coordinate_list->max();
    Pair min = coordinate_list->min();
    
    
    std::cout <<  max.x  <<  ", " << max.y << " , " << min.x  <<  ", " << min.y << " , "  << std::endl << std::flush;
    
    params1->max = max;
    params1->min = min;
    
    
    clear_coord(coordinate_list);
    
    params1->push_back(0,  &energy[0], 512);
    
      /*  params1->push_back( 0, -5, -3.9265e+5);
        params1->push_back( 0, -4, -3.9265e+4);
        params1->push_back( 0, -3, -3.9265e+3);
        params1->push_back( 0, -2, -3.9265e+2);
        params1->push_back( 0, -1, -3.9265e+1);
        params1->push_back( 0, 0.0, 0.0);
        params1->push_back( 0, 1, 3.9265e+1);
        params1->push_back( 0, 2, 3.9265e+2);
        params1->push_back( 0, 3, 3.9265e+3);
        params1->push_back( 0, 4, 3.9265e+4);
        params1->push_back( 0, 5, 3.9265e+5);
        
    */
    for (j = 0; j < 6; j++)
    { /* calculate unpredictability measure cw */
        double r1, phi1;
        r_prime = 2.0 * r[chn][old][j] - r[chn][oldest][j];
        phi_prime = 2.0 * phi_sav[chn][old][j] - phi_sav[chn][oldest][j];
        r[chn][new_][j] = r1 = sqrt((double) energy[j]);
        phi_sav[chn][new_][j] = phi1 = phi[j];

        temp3 = r1 + fabs(r_prime);
        if (temp3 != 0.0)
        {
            temp1 = r1 * cos(phi1) - r_prime * cos(phi_prime);
            temp2 = r1 * sin(phi1) - r_prime * sin(phi_prime);
            cw[j] = sqrt(temp1 * temp1 + temp2 * temp2) / temp3;
        } else
            cw[j] = 0;
    }


    /**********************************************************************
     *     compute unpredicatibility of next 200 spectral lines            *
     **********************************************************************/

    for (sblock = 0; sblock < 3; sblock++)
    {
#if RING_BUFFER==0
        for (j = 0, k = 128 * (2 + sblock); j < BLKSIZE_s; j++, k++)
        { /* window data with HANN window */
            wsamp_r[j] = window_s[j] * savebuf[k];
        }
#else
        j = 0;
        idx = (beg + 128 * (2 + sblock)) % syncsize;
        end = (idx + BLKSIZE_s) % syncsize;
        if (idx > end)
        {
            while (idx < syncsize)
            {
                wsamp_r[j] = window_s[j] * savebuf[idx++];
                j++;
            }
            idx = 0;
        }
        while (idx < end)
        {
            wsamp_r[j] = window_s[j] * savebuf[idx++];
            j++;
        }
#endif

        fft(wsamp_r, wsamp_i, energy_s[sblock], phi_s[sblock], BLKSIZE_s); /* short FFT*/
    }

    for (j = 6; j < 206; j += 4)
    { /* calculate unpredictability measure cw */
        double r1, phi1;
        k = (j + 2) / 4;

        r_prime = 2.0 * sqrt((double) energy_s[0][k]) - sqrt((double) energy_s[2][k]);
        phi_prime = 2.0 * phi_s[0][k] - phi_s[2][k];
        r1 = sqrt((double) energy_s[1][k]);
        phi1 = phi_s[1][k];

        temp3 = r1 + fabs(r_prime);
        if (temp3 != 0.0)
        {
            temp1 = r1 * cos(phi1) - r_prime * cos(phi_prime);
            temp2 = r1 * sin(phi1) - r_prime * sin(phi_prime);
            cw[j] = sqrt(temp1 * temp1 + temp2 * temp2) / temp3;
        } else
            cw[j] = 0.0;

        cw[j + 1] = cw[j + 2] = cw[j + 3] = cw[j];
    }


    {
    coordlist coordinate_list = NULL;
    params2->clean();
    params2->update = true;
   coordinate_list = push_back_coords(coordinate_list, 0, &cw[0] , 512);
    
    Pair max = coordinate_list->max();
    Pair min = coordinate_list->min();
    
    
    std::cout <<  max.x  <<  ", " << max.y << " , " << min.x  <<  ", " << min.y << " , "  << std::endl << std::flush;
    
    params2->max = max;
    params2->min = min;
    
    
    clear_coord(coordinate_list);
    
    params2->push_back(0,  &cw[0], 512);
    
    }
    
    
    
    
    
    
    /**********************************************************************
     *    Calculate the energy and the unpredictability in the threshold   *
     *    calculation partitions                                           *
     **********************************************************************/

    for (b = 0; b < CBANDS; b++)
    {
        eb[b] = 0.0;
        cb[b] = 0.0;
    }
    for (j = 0; j < HBLKSIZE; j++)
    {
        int tp = partition_l[j];
        if (tp >= 0)
        {
            eb[tp] += energy[j];
            cb[tp] += cw[j] * energy[j];
        }
    }


    {
        coordlist coordinate_list = NULL;
        params3->clean();
        params3->update = true;
        coordinate_list = push_back_coords(coordinate_list, 0, &cb[0], 63);

        Pair max = coordinate_list->max();
        Pair min = coordinate_list->min();


        std::cout << max.x << ", " << max.y << " , " << min.x << ", " << min.y << " , " << std::endl << std::flush;

        params3->max = max;
        params3->min = min;


        clear_coord(coordinate_list);

        params3->push_back(0, &cb[0], 63);

    }
    

    {
        coordlist coordinate_list = NULL;
        params4->clean();
        params4->update = true;
        coordinate_list = push_back_coords(coordinate_list, 0, &eb[0], 63);

        Pair max = coordinate_list->max();
        Pair min = coordinate_list->min();


        std::cout << max.x << ", " << max.y << " , " << min.x << ", " << min.y << " , " << std::endl << std::flush;

        params4->max = max;
        params4->min = min;


        clear_coord(coordinate_list);

        params4->push_back(0, &eb[0], 63);

    }

    /**********************************************************************
     *      convolve the partitioned energy and unpredictability           *
     *      with the spreading function, s3_l[b][k]                        *
     ******************************************************************** */

    for (b = 0; b < CBANDS; b++)
    {
        ecb[b] = 0.0;
        ctb[b] = 0.0;
    }
    for (b = 0; b < CBANDS; b++)
    {
        for (k = 0; k < CBANDS; k++)
        {
            ecb[b] += s3_l[b][k] * eb[k]; /* sprdngf for Layer III */
            ctb[b] += s3_l[b][k] * cb[k];
        }
    }
    



    /* calculate the tonality of each threshold calculation partition */
    /* calculate the SNR in each threshhold calculation partition */

    for (b = 0; b < CBANDS; b++)
    {
        double cbb, tbb;
        if (ecb[b] != 0.0)
        {
            cbb = ctb[b] / ecb[b];
            if (cbb < 0.01)
                cbb = 0.01;
            tbb = -0.299 - 0.43 * log(cbb); /* conv1=-0.299, conv2=-0.43 */
            tbb = minimum(maximum(0.0, tbb), 1.0); /* 0<=tbb<=1 */
        } else
            tbb = 0.0; /* cbb==0 => -0.299-0.43*cbb<0 => tbb=0*/

        SNR_l[b] = maximum(minval[b], 23.0 * tbb + 6.0); /* 29*tbb + 6*(1-tbb) */
    } /* TMN=29.0,NMT=6.0 for all calculation partitions */

    for (b = 0; b < CBANDS; b++) /* calculate the threshold for each partition */
        nb[b] = ecb[b] * norm_l[b] * exp(-SNR_l[b] * LN_TO_LOG10);

    for (b = 0; b < CBANDS; b++)
    {
        /* pre-echo control */
        double temp_1; /* BUG of IS */
        temp_1 = minimum(nb[b], minimum(2.0 * nb_1[chn][b], 16.0 * nb_2[chn][b]));
        thr[b] = maximum(qthr_l[b], temp_1); /* rpelev=2.0, rpelev2=16.0 */
        nb_2[chn][b] = nb_1[chn][b];
        nb_1[chn][b] = nb[b];
    }
    

    {
        coordlist coordinate_list = NULL;
        params5->clean();
        params5->update = true;
        coordinate_list = push_back_coords(coordinate_list, 0, &nb[0], 63);

        Pair max = coordinate_list->max();
        Pair min = coordinate_list->min();


        std::cout << max.x << ", " << max.y << " , " << min.x << ", " << min.y << " , " << std::endl << std::flush;

        params5->max = max;
        params5->min = min;


        clear_coord(coordinate_list);

        params5->push_back(0, &nb[0], 63);

    }

    *pe = 0.0; /*  calculate percetual entropy */
    for (b = 0; b < CBANDS; b++)
    {
        double tp;
        tp = minimum(0.0, log((thr[b] + 1.0) / (eb[b] + 1.0))); /*not log*/
        *pe -= numlines[b] * tp;
    } /* thr[b] -> thr[b]+1.0 : for non sound portition */

#define switch_pe  1800

    if (*pe < switch_pe)
    {
        /* no attack : use long blocks */
        blocktype = NORM_TYPE;

        switch (blocktype_old[chn])
        {
            case NORM_TYPE:
            case STOP_TYPE:
                blocktype = NORM_TYPE;
                break;

            case SHORT_TYPE:
                blocktype = STOP_TYPE;
                break;

                /*			case START_TYPE:
                                                dPrintf( stderr, "Error in block selecting\n" );
                                                abort();
                                                break; problem */
        }

        /* threshold calculation (part 2) */
        for (sb = 0; sb < SBMAX_l; sb++)
        {
            en[sb] = w1_l[sb] * eb[bu_l[sb]] + w2_l[sb] * eb[bo_l[sb]];
            thm[sb] = w1_l[sb] * thr[bu_l[sb]] + w2_l[sb] * thr[bo_l[sb]];

            for (b = bu_l[sb] + 1; b < bo_l[sb]; b++)
            {
                en[sb] += eb[b];
                thm[sb] += thr[b];
            }

            if (en[sb] != 0.0)
                ratio[chn][sb] = thm[sb] / en[sb];
            else
                ratio[chn][sb] = 0.0;
        }
    } else
    {
        /* attack : use short blocks */
        blocktype = SHORT_TYPE;

        if (blocktype_old[chn] == NORM_TYPE)
            blocktype_old[chn] = START_TYPE;
        if (blocktype_old[chn] == STOP_TYPE)
            blocktype_old[chn] = SHORT_TYPE;

        /* threshold calculation for short blocks */

        for (sblock = 0; sblock < 3; sblock++)
        {
            for (b = 0; b < CBANDS_s; b++)
            {
                eb[b] = 0.0;
                ecb[b] = 0.0;
            }

            for (j = 0; j < HBLKSIZE_s; j++)
                eb[partition_s[j]] += energy_s[sblock][j];

            for (b = 0; b < CBANDS_s; b++)
                for (k = 0; k < CBANDS_s; k++)
                    ecb[b] += s3_l[b][k] * eb[k];

            for (b = 0; b < CBANDS_s; b++)
            {
                nb[b] = ecb[b] * norm_l[b] * exp((double) SNR_s[b] * LN_TO_LOG10);
                thr[b] = maximum(qthr_s[b], nb[b]);
            }

            for (sb = 0; sb < SBMAX_s; sb++)
            {
                en[sb] = w1_s[sb] * eb[bu_s[sb]] + w2_s[sb] * eb[bo_s[sb]];
                thm[sb] = w1_s[sb] * thr[bu_s[sb]] + w2_s[sb] * thr[bo_s[sb]];

                for (b = bu_s[sb] + 1; b < bo_s[sb]; b++)
                {
                    en[sb] += eb[b];
                    thm[sb] += thr[b];
                }

                if (en[sb] != 0.0)
                    ratio_s[chn][sb][sblock] = thm[sb] / en[sb];
                else
                    ratio_s[chn][sb][sblock] = 0.0;
            }
        }
    }

    cod_info->block_type = blocktype_old[chn];
    blocktype_old[chn] = blocktype;

    if (cod_info->block_type == NORM_TYPE)
        cod_info->window_switching_flag = 0;
    else
        cod_info->window_switching_flag = 1;

    cod_info->mixed_block_flag = 0;
}
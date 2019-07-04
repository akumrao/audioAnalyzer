/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fft.cpp
 * Author: root
 *
 * Created on April 28, 2019, 11:14 PM
 */

#include <cstdlib>

#include "Fft.h"
#include "Common.h"

using namespace std;



/*****************************************************************************
 ************************** Start of Subroutines *****************************
 *****************************************************************************/

/*****************************************************************************
 * FFT computes fast fourier transform of BLKSIZE samples of data            *
 *   based on the decimation-in-frequency algorithm described in "Digital    *
 *   Signal Processing" by Oppenheim and Schafer, refer to pages 304         *
 *   (flow graph) and 330-332 (Fortran program in problem 5).                *
 *                                                                           *
 *   required constants:                                                     *
 *         PI          3.14159265358979                                      *
 *         BLKSIZE     should be 2^^M for a positive integer M               *
 *                                                                           *
 *****************************************************************************/



/* To get the inverse fft, set NORMAL_FFT to 0 */
#define  NORMAL_FFT  1

/* Try this to speed up the fft when using non-complex sequences */
#define  REAL_SEQUENCE 1



int fInit_fft;

void fft(float x_real[], float x_imag[], float energy[], float phi[], int N){
    int i, j, k, off;
    int ip, le, le1;
    static double w_real[BLKSIZE / 2], w_imag[BLKSIZE / 2];
    double t_real, t_imag, u_real, u_imag;
    int N_ORG;

    if (fInit_fft == 0)
    {
        for (i = 0; i < BLKSIZE / 2; i++)
        {
            w_real[i] = cos(PI * i / (BLKSIZE / 2));
#if NORMAL_FFT
            w_imag[i] = -sin(PI * i / (BLKSIZE / 2));
#else
            w_imag[i] = sin(PI * i / (BLKSIZE / 2));
#endif
        }
        fInit_fft++;
    }

#if REAL_SEQUENCE
    N_ORG = N;
    N >>= 1;
    /* packing the sequence to the half length */
    for (i = 0; i < N; i++)
    {
        x_real[i] = x_real[2 * i];
        x_imag[i] = x_real[2 * i + 1];
    }
#endif

    off = BLKSIZE / N;
    for (le = N; le > 2; le = le1)
    {
        le1 = le >> 1;
        k = 0;
        for (j = 0; j < le1; j++)
        {
            u_real = w_real[k];
            u_imag = w_imag[k];
            for (i = j; i < N; i += le)
            {
                ip = i + le1;
                t_real = x_real[i] - x_real[ip];
                t_imag = x_imag[i] - x_imag[ip];
                x_real[i] += x_real[ip];
                x_imag[i] += x_imag[ip];
                x_real[ip] = t_real * u_real - t_imag*u_imag;
                x_imag[ip] = t_imag * u_real + t_real*u_imag;
            }
            k += off;
        }
        off <<= 1;
    }
    /* special case: le1 = 1; all Wn = 1 */
    for (i = 0; i < N; i += 2)
    {
        ip = i + 1;
        t_real = x_real[i] - x_real[ip];
        t_imag = x_imag[i] - x_imag[ip];
        x_real[i] += x_real[ip];
        x_imag[i] += x_imag[ip];
        x_real[ip] = t_real;
        x_imag[ip] = t_imag;
    }

    /* this section reorders the data to the correct ordering */
    j = 0;
    for (i = 0; i < N - 1; i++)
    {
        if (i < j)
        {
            t_real = x_real[j];
            t_imag = x_imag[j];
            x_real[j] = x_real[i];
            x_imag[j] = x_imag[i];
            x_real[i] = t_real;
            x_imag[i] = t_imag;
        }
        k = N / 2;
        while (k <= j)
        {
            j = j - k;
            k = k >> 1;
        }
        j = j + k;
    }

#if REAL_SEQUENCE
    /* unpacking the sequence */
    t_real = x_real[0];
    t_imag = x_imag[0];
    x_real[0] = t_real + t_imag;
    x_imag[0] = 0.0;
    x_real[N] = t_real - t_imag;
    x_imag[N] = 0.0;
    k = off = BLKSIZE / N_ORG;
    for (i = 1; i < N / 2; i++)
    {
        double t1 = x_real[i] + x_real[N - i];
        double t2 = x_real[i] - x_real[N - i];
        double t3 = x_imag[i] + x_imag[N - i];
        double t4 = x_imag[i] - x_imag[N - i];
        double t5 = t2 * w_imag[k] + t3 * w_real[k];
        double t6 = t3 * w_imag[k] - t2 * w_real[k];
        x_real[ i] = (t1 + t5) / 2.0;
        x_imag[ i] = (t6 + t4) / 2.0;
        x_real[N - i] = (t1 - t5) / 2.0;
        x_imag[N - i] = (t6 - t4) / 2.0;
        k += off;
    }
    /* x_real[N/2] doesn�t change */
    /* x_real[N/2] changes the sign in case of a nornal fft */
#if (NORMAL_FFT)
    x_imag[N / 2] = -x_imag[N / 2];
#endif
    N = N_ORG;
#endif

    /* calculating the energy and phase, phi */
#if REAL_SEQUENCE
    for (i = 0; i <= N / 2; i++)
    {
#else
    for (i = 0; i < N; i++)
    {
#endif
        energy[i] = x_real[i] * x_real[i] + x_imag[i] * x_imag[i];
        if (energy[i] <= 0.0005)
        {
            phi[i] = 0;
            energy[i] = 0.0005; /* keep the identity */
            x_real[i] = sqrt(0.0005); /* energy[i] * cos(phi[i]) == x_real[i] */
            x_imag[i] = 0.0; /* energy[i] * sin(phi[i]) == x_imag[i] */
        } else
            phi[i] = atan2((double) x_imag[i], (double) x_real[i]);
    }
}





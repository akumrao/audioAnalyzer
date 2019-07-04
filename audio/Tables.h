/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tables.h
 * Author: root
 *
 * Created on April 28, 2019, 10:15 PM
 */

#ifndef TABLES_H
#define TABLES_H


typedef	struct
{
/*	int	no; */
	int			lines;
	double	minVal;
	double	qthr;
	double	norm;
	double	bVal;
	
} psyDataElem;


typedef	struct
{
/*	int	no; */
	int			lines;
	double	qthr;
	double	norm;
	double	snr;
	double	bVal;
	
} psyDataElem2;

typedef	struct
{
	int			cbw;
	int			bu;
	int			bo;
	float		w1;
	float		w2;
} psyDataElem3;


extern	psyDataElem		psy_longBlock_48000_61[62];
extern	psyDataElem		psy_longBlock_44100_62[63];
extern	psyDataElem		psy_longBlock__32000_58[59];

extern	psyDataElem2	psy_shortBlock_48000_37[38];
extern	psyDataElem2	psy_shortBlock_44100_38[39];
extern	psyDataElem2	psy_shortBlock_32000_41[42];

extern	psyDataElem3	psy_data3_48000_20[21];
extern	psyDataElem3	psy_data3_44100_20[21];
extern	psyDataElem3	psy_data3_32000_20[21];

extern	psyDataElem3	psy_data4_48000_11[12];
extern	psyDataElem3	psy_data4_44100_11[12];
extern	psyDataElem3	psy_data4_32000_11[12];


extern	double	enwindow[512];
extern	char		aHuffcode[1498][36];





#endif /* TABLES_H */


/*
			(c) Copyright 1998-2000 - Tord Jansson
			======================================

		This file is part of the BladeEnc MP3 Encoder, based on
		ISO's reference code for MPEG Layer 3 compression, and might
		contain smaller or larger sections that are directly taken
		from ISO's reference code.

		All changes to the ISO reference code herein are either
		copyrighted by Tord Jansson (tord.jansson@swipnet.se)
		or sublicensed to Tord Jansson by a third party.

	BladeEnc is free software; you can redistribute this file
	and/or modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.



	------------    Changes    ------------

	2000-12-06  Andre Piotrowski

	-	reformatted
*/
#ifndef		__TABLES__
#define		__TABLES__





#define		NEW_L3PARM_TABLES		1
#define		CALC_NORM				0





#define		CBANDS					63
#define		CBANDS_s				42





#if NEW_L3PARM_TABLES


	typedef		struct
				{
					int						cbmax_l;   /* number of valid entries */
					int						numlines_l[CBANDS];
					double					minval    [CBANDS];
					double					qthr_l    [CBANDS];
#if !CALC_NORM   /* do we compute more exact values? */
					double					norm_l    [CBANDS];
#endif
					double					bval_l    [CBANDS];
				}						l3_long_data;


	typedef		struct
				{
					int						cbmax_s;   /* number of valid entries */
					int						numlines_s[CBANDS_s];
					double					qthr_s    [CBANDS_s];
#if !CALC_NORM   /* do we compute more exact values? */
					double					norm_s    [CBANDS_s];
#endif
					double					SNR_s     [CBANDS_s];
					double					bval_s    [CBANDS_s];
				}						l3_short_data;


	typedef		struct
				{
					int						cbw_l     [CBANDS];
					int						bu_l      [CBANDS];
					int						bo_l      [CBANDS];
					double					w1_l      [CBANDS];
					double					w2_l      [CBANDS];
				}						l3_long_thres;


	typedef		struct
				{
					int						cbw_s     [CBANDS_s];
					int						bu_s      [CBANDS_s];
					int						bo_s      [CBANDS_s];
					double					w1_s      [CBANDS_s];
					double					w2_s      [CBANDS_s];
				}						l3_short_thres;


	typedef		struct
				{
					l3_long_data			long_data;
					l3_short_data			short_data;
					l3_long_thres			long_thres;
					l3_short_thres			short_thres;
				}						l3_parm_block;


	extern		l3_parm_block			l3_parm[3];



#endif   /* NEW_L3PARM_TABLES */





extern		double					enwindow[512];
extern		char					aHuffcode[1498][36];





#endif		/* __TABLES__ */



/**********************************************************************
 * ISO MPEG Audio Subgroup Software Simulation Group (1996)
 * ISO 13818-3 MPEG-2 Audio Encoder - Lower Sampling Frequency Extension
 *
 * $Id: loop.h,v 1.1 1996/02/14 04:04:23 rowlands Exp $
 *
 * $Log: loop.h,v $
 * Revision 1.1  1996/02/14 04:04:23  rowlands
 * Initial revision
 *
 * Received from Mike Coleman
 **********************************************************************/

/**********************************************************************
 *   date   programmers                comment                        *
 * 25. 6.92  Toshiyuki Ishino          Ver 1.0                        *
 * 29.10.92  Masahiro Iwadare          Ver 2.0                        *
 * 17. 4.93  Masahiro Iwadare          Updated for IS Modification    *
 *                                                                    *
 *********************************************************************/

#ifndef		__LOOP__
#define		__LOOP__




#include "common.h"
#include "l3side.h"





/*	#define	SBLIMIT			32 */
#define	CBLIMIT			21

#define	SFB_LMAX		22
#define	SFB_SMAX		13




extern	int				fInit_iteration_loop;
/* NOT NEEDED
extern	int				fInit_huffman_read_flag;
*/


extern	int				pretab[];


struct	scalefac_struct
{
	int l[23];
	int s[14];
};

extern	struct scalefac_struct		sfBandIndex[];  /* Table B.8 -- in loop.c */





void					iteration_loop
(
	double					pe[][2],
	double					xr_org[2][2][576],
	III_psy_ratio			*ratio,
	III_side_info_t			*l3_side,
	int						l3_enc[2][2][576],
	int						mean_bits,
	int						stereo,
	double					xr_dec[2][2][576],
	III_scalefac_t			*scalefac,
	frame_params			*fr_ps,
	int						ancillary_pad,
	int						bitsPerFrame
);

int						my_nint (double in);

void					fixStatic_loop (void);

void					genNoisePowTab (void);





#endif		/* __LOOP__ */

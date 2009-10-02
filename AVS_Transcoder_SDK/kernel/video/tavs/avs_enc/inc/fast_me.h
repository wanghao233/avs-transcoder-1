/*
*****************************************************************************
* COPYRIGHT AND WARRANTY INFORMATION
*
* Copyright 2003, Advanced Audio Video Coding Standard, Part II
*
* DISCLAIMER OF WARRANTY
*
* The contents of this file are subject to the Mozilla Public License
* Version 1.1 (the "License"); you may not use this file except in
* compliance with the License. You may obtain a copy of the License at
* http://www.mozilla.org/MPL/
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
* License for the specific language governing rights and limitations under
* the License.
*                     
* THIS IS NOT A GRANT OF PATENT RIGHTS - SEE THE AVS PATENT POLICY.
* The AVS Working Group doesn't represent or warrant that the programs
* furnished here under are free of infringement of any third-party patents.
* Commercial implementations of AVS, including shareware, may be
* subject to royalty fees to patent holders. Information regarding
* the AVS patent policy for standardization procedure is available at 
* AVS Web site http://www.avs.org.cn. Patent Licensing is outside
* of AVS Working Group.
*
* The Original Code is Reference Software for China National Standard 
* GB/T 20090.2-2006 (short for AVS-P2 or AVS Video) at version RM52J.
*
* The Initial Developer of the Original Code is Video subgroup of AVS
* Workinggroup (Audio and Video coding Standard Working Group of China).
* Contributors:   Guoping Li,    Siwei Ma,    Jian Lou,    Qiang Wang , 
*   Jianwen Chen,Haiwu Zhao,  Xiaozhen Zheng, Junhao Zheng, Zhiming Wang
* 
******************************************************************************
*/



/*
*************************************************************************************
* File name: 
   
* Function: Macro definitions and global variables for fast integer pel motion 
		    estimation and fractional pel motio estimation
*
*************************************************************************************
*/
 

#ifndef _FAST_ME_H_
#define _FAST_ME_H_
#include "global.h"
#define MAX_V_SEARCH_RANGE 1024
#define MAX_V_SEARCH_RANGE_FIELD 512
#define MAX_H_SEARCH_RANGE 8192
#define EARLY_TERMINATION  if(ref>0)  \
	{                                                                    \
	if ((min_mcost-pred_SAD_ref)<pred_SAD_ref*betaThird)             \
	goto third_step;                                             \
	else if((min_mcost-pred_SAD_ref)<pred_SAD_ref*betaSec)           \
	goto sec_step;                                               \
	}                                                                    \
	else if(blocktype>1)                                                 \
	{                                                                    \
	if ((min_mcost-pred_SAD_uplayer)<pred_SAD_uplayer*betaThird)     \
		{                                                                \
		goto third_step;                                             \
		}                                                                \
		else if((min_mcost-pred_SAD_uplayer)<pred_SAD_uplayer*betaSec)   \
		goto sec_step;                                               \
	}                                                                    \
	else                                                                 \
	{                                                                    \
	if ((min_mcost-pred_SAD_space)<pred_SAD_space*betaThird)         \
		{                                                                \
		goto third_step;                                             \
		}                                                                \
		else if((min_mcost-pred_SAD_space)<pred_SAD_space*betaSec)       \
		goto sec_step;	                                             \
	}
/*
#define PartCalMad(ret_cost, ref_pic,orig_pic, blocksize_y, blocksize_x,  blocksize_x4, mcost, min_mcost, cand_x, cand_y)\
{\
	int yI,x4;\
	pel_t *orig_line, *ref_line;\
	for (yI=0; yI<blocksize_y; yI++)\
	{\
		ref_line  = (*get_ref_line)(blocksize_x, ref_pic, cand_y + yI, cand_x);\
		orig_line = orig_pic [yI];\
		for (x4=0; x4<blocksize_x4; x4++)\
		{\
			mcost += byte_abs[ *orig_line++ - *ref_line++ ];\
			mcost += byte_abs[ *orig_line++ - *ref_line++ ];\
			mcost += byte_abs[ *orig_line++ - *ref_line++ ];\
			mcost += byte_abs[ *orig_line++ - *ref_line++ ];\
		}\
		if (mcost >= min_mcost)\
		{\
			break;\
		}\
	}\
	ret_cost = mcost;\
}
*/

#define SEARCH_ONE_PIXEL  if(abs(cand_x - center_x) <=search_range && abs(cand_y - center_y)<= search_range) \
		{ \
		if(!McostState[cand_y-center_y+search_range][cand_x-center_x+search_range])	\
		{ \
		mcost = MV_COST (lambda_factor, mvshift, cand_x, cand_y, pred_x, pred_y); \
		mcost = PartCalMad(ref_pic, orig_pic, blocksize_y,blocksize_x,blocksize_x4,mcost,min_mcost,cand_x,cand_y);\
		McostState[cand_y-center_y+search_range][cand_x-center_x+search_range] = mcost; \
		if (mcost < min_mcost) \
		{ \
		best_x = cand_x; \
		best_y = cand_y; \
		min_mcost = mcost; \
		} \
		} \
		}
#define SEARCH_ONE_PIXEL1(value_iAbort)	if(abs(cand_x - center_x) <=search_range && abs(cand_y - center_y)<= search_range) \
			{ \
			if(!McostState[cand_y-center_y+search_range][cand_x-center_x+search_range])	\
				{ \
				mcost = MV_COST (lambda_factor, mvshift, cand_x, cand_y, pred_x, pred_y); \
				mcost = PartCalMad(ref_pic, orig_pic, blocksize_y,blocksize_x,blocksize_x4,mcost,min_mcost,cand_x,cand_y); \
				McostState[cand_y-center_y+search_range][cand_x-center_x+search_range] = mcost; \
				if (mcost < min_mcost) \
					{ \
					best_x = cand_x; \
					best_y = cand_y; \
					min_mcost = mcost; \
					iAbort = value_iAbort; \
					} \
				} \
			}
/* Moved to c_avs_enc by YueLei Xie */
/*
int **McostState; //state for integer pel search

int *****all_mincost;//store the MV and SAD information needed;
int *****all_bwmincost;//store for backward prediction
int pred_SAD_space,pred_SAD_time,pred_SAD_ref,pred_SAD_uplayer;//SAD prediction
int pred_MV_time[2],pred_MV_ref[2],pred_MV_uplayer[2];//pred motion vector by space or tempral correlation,Median is provided

//for half_stop
float Quantize_step;
float  Bsize[8];
int Thresh4x4;
float AlphaSec[8];
float AlphaThird[8];
int  flag_intra[124];//HD enough
int  flag_intra_SAD;
void DefineThreshold();
void DefineThresholdMB();

char **SearchState; //state for fractional pel search
void DefineThreshold();
void DefineThresholdMB();
int get_mem_mincost (int****** mv);
int get_mem_bwmincost (int****** mv);
int get_mem_FME();
void free_mem_mincost (int***** mv);
void free_mem_bwmincost (int***** mv);
void free_mem_FME();
void   decide_intrabk_SAD();
void skip_intrabk_SAD(int best_mode, int ref_max);

int                                     //  ==> minimum motion cost after search
FastIntegerPelBlockMotionSearch  (pel_t**   orig_pic,     // <--  not used
								  int       ref,          // <--  reference frame (0... or -1 (backward))
								  int       pic_pix_x,    // <--  absolute x-coordinate of regarded AxB block
								  int       pic_pix_y,    // <--  absolute y-coordinate of regarded AxB block
								  int       blocktype,    // <--  block type (1-16x16 ... 7-4x4)
								  int       pred_mv_x,    // <--  motion vector predictor (x) in sub-pel units
								  int       pred_mv_y,    // <--  motion vector predictor (y) in sub-pel units
								  int*      mv_x,         //  --> motion vector (x) - in pel units
								  int*      mv_y,         //  --> motion vector (y) - in pel units
								  int       search_range, // <--  1-d search range in pel units                         
								  int       min_mcost,    // <--  minimum motion cost (cost for center or huge value)
								  double    lambda) ;      // <--  lagrangian parameter for determining motion cost
int AddUpSADQuarter(int pic_pix_x,int pic_pix_y,int blocksize_x,int blocksize_y,
					int cand_mv_x,int cand_mv_y, pel_t **ref_pic, pel_t**   orig_pic, int Mvmcost, int min_mcost,int useABT);

int                                                   //  ==> minimum motion cost after search
FastSubPelBlockMotionSearch (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
							 int       ref,           // <--  reference frame (0... or -1 (backward))
							 int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
							 int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
							 int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
							 int       pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
							 int       pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
							 int*      mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
							 int*      mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
							 int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
							 int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
							 int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
							 double    lambda,
							 int	useABT);        // <--  lagrangian parameter for determining motion cost

int                                               //  ==> minimum motion cost after search
SubPelBlockMotionSearch (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
                         int       ref,           // <--  reference frame (0... or -1 (backward))
                         int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
                         int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
                         int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
                         int       pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
                         int       pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
                         int*      mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
                         int*      mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
                         int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
                         int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
                         int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
                         double    lambda         // <--  lagrangian parameter for determining motion cost
                         );

int                                               //  ==> minimum motion cost after search
SubPelBlockMotionSearch_bid (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
                         int       ref,           // <--  reference frame (0... or -1 (backward))
                         int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
                         int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
                         int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
                         int       pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
                         int       pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
                         int*      mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
                         int*      mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
                         int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
                         int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
                         int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
                         double    lambda         // <--  lagrangian parameter for determining motion cost
                         );

void FME_SetMotionVectorPredictor (int  pmv[2],
								   int  **refFrArr,
								   int  ***tmp_mv,
								   int  ref_frame,
								   int  mb_x,
								   int  mb_y,
								   int  blockshape_x,
								   int  blockshape_y,
								   int  blocktype,
								   int  ref);

int                                         //  ==> minimum motion cost after search
FME_BlockMotionSearch (int       ref,           // <--  reference frame (0... or -1 (backward))
					   int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
					   int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
					   int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
					   int       search_range,  // <--  1-d search range for integer-position search
					   double    lambda         // <--  lagrangian parameter for determining motion cost
					   );

int                                         //  ==> minimum motion cost after search
FME_BlockMotionSearch_bid (int       ref,           // <--  reference frame (0... or -1 (backward))
					   int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
					   int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
					   int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
					   int       search_range,  // <--  1-d search range for integer-position search
					   double    lambda         // <--  lagrangian parameter for determining motion cost
					   );
int                                                   //  ==> minimum motion cost after search
FastSubPelBlockMotionSearch_bid (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
							 int       ref,           // <--  reference frame (0... or -1 (backward))
							 int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
							 int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
							 int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
							 int       pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
							 int       pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
							 int*      mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
							 int*      mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
							 int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
							 int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
							 int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
							 double    lambda,
							 int	useABT);        // <--  lagrangian parameter for determining motion cost
  int AddUpSADQuarter_bid(int pic_pix_x,int pic_pix_y,int blocksize_x,int blocksize_y,
    int cand_mv_x,int cand_mv_y, pel_t **ref_pic, pel_t **ref_pic_bid, pel_t**   orig_pic, 
	int Mvmcost, int min_mcost,int useABT,int DistanceIndexFw, int DistanceIndexBw);
*/
#endif
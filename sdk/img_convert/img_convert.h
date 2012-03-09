/******************************************************************************

  Copyright (C), 2001-2011, DCN Co., Ltd.

 ******************************************************************************
  File Name     : img_convert.h
  Version       : Initial Draft
  Author        : Sun
  Created       : 2012/2/29
  Last Modified :
  Description   : img_convert.c header file
  			This module will do colorspace conversion and resize of a raw image
  			also can do some enhance alg
  			buffer for process must be phy continously memory
  Function List :
  History       :
  1.Date        : 2012/2/29
    Author      : Sun
    Modification: Created file

******************************************************************************/
#ifndef __IMG_CONVERT_H__
#define __IMG_CONVERT_H__

#include "common.h"
#include "alg.h"

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/
#define CONV_MAX_OUT_CHAN			2
	
#define CONV_FLAG_AVG_EN			(1 << 0)	/* enable average filter */
#define CONV_FLAG_EE_EN				(1 << 1)	/* edge enhance enable */
#define CONV_FLAG_NF_EN				(1 << 2)	/* noise filter enable */
#define CONV_FLAG_GAMMA_EN			(1 << 3)	/* gamma enable */
#define CONV_FLAG_LUMA_EN			(1 << 4)	/* lumanance adjust enable */
#define CONV_FLAG_DFC_EN			(1 << 5)	/* defect correction enable */

#define CONV_MAX_OUT_CHAN_NUM		2
#define CONV_CHANB_MAX_OUT_WIDTH	1088		/* Maxium output width for channel B */
#define CONV_MIN_EE_TAB_SIZE		2048		/* min EE table size in bytes */

/* Default device name */
#define RESIZER_DEVICE   			"/dev/davinci_resizer"
#define PREVIEWER_DEVICE 			"/dev/davinci_previewer"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/* Init params for this module */
typedef struct _ImgConvInitParams {
	Int32		size;				/* size of this struct */
	const char *prevDevName;		/* preview device name, if NULL, use default */
	const char *rszDevName;			/* resize device name,  if NULL, use default */
}ImgConvInitParams;

/* Convert output attributes */
typedef struct _ConvOutAttrs {
	Bool			enbale; 		/* Enable or disable this output, for chan0, this must always be enabled */
	Uint16			width;			/* Output width, should be aligned to 16 */
	Uint16			height; 		/* Output height, should be aligned to 2 */
	ChromaFormat	pixFmt; 		/* Output color space format, ONLY support FMT_YUV420SP & FMT_YUV422ILE */
}ConvOutAttrs;

/* Dynamic params for this module */
typedef struct _ImgConvDynParams {
	Int32			size;			/* size of this struct */
	ChromaFormat	inputFmt;		/* input color space format, support FMT_BAYER_RGBG, FMT_YUV420SP, FMT_YUV422ILE */
	Uint16			inputWidth;		/* input width */
	Uint16			inputHeight;	/* input height */
	Uint32			ctrlFlags;		/* ctrl flags, see CONV_FLAG_XXX */
	Uint16			digiGain;		/* digital gain for format convert */
	ConvOutAttrs	outAttrs[CONV_MAX_OUT_CHAN_NUM]; /* out attributes */
	Uint8			brigtness;		/* luma params, can only be set when init */
	Uint8			contrast;		/* luma params, can only be set when init */
	Int16			*eeTable;		/* edge enhance table */
	Int32			eeTabSize;		/* size of ee table, in bytes */
}ImgConvDynParams;

/* Input args for process, not used */
typedef struct _ImgConvInArgs {
	Int32			size;			/* size of this struct */
}ImgConvInArgs;

/* Output args for process, not used */
typedef struct _ImgConvOutArgs {
	Int32			size;			/* size of this struct */
}ImgConvOutArgs;

enum ImgConvCmd {
	CONV_CMD_SET_DYN_PARAMS = ALG_CMD_SET_DYN_PARAMS,
	CONV_CMD_GET_DYN_PARAMS = ALG_CMD_GET_DYN_PARAMS,
	CONV_CMD_SET_OUT0_ATTRS = ALG_CMD_USER_BASE + 1,
	CONV_CMD_SET_OUT1_ATTRS,
};

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * routines' implementations                    *
 *----------------------------------------------*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/* Functions of this module */
extern const AlgFxns IMGCONV_ALG_FXNS;

/* Macros for easy use of img convert alg */
#define img_conv_create(initParams, dynParams)	\
	alg_create(&IMGCONV_ALG_FXNS, initParams, dynParams)

#define img_conv_delete(handle) \
	alg_delete(handle)

#define img_conv_process(handle, inBuf, inArgs, outBuf, outArgs) \
	alg_process(handle, inBuf, inArgs, outBuf, outArgs)

#define img_conv_control(handle, cmd, args) \
	alg_control(handle, cmd, args)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __IMG_CONVERT_H__ */

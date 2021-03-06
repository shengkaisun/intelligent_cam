#ifndef __CAM_IMG_ADJ_H__
#define __CAM_IMG_ADJ_H__

#include "common.h"

/* 
  * Front end input info 
  */
typedef ImgDimension CamInputInfo;

/* 
  * Exposure params for camera control 
  */
typedef struct {
	Uint32	shutterTime;		//shutter time, unit: us
	Uint16	globalGain;			//0~1023, equals 6db~42db
	Uint16	reserved;
} CamExprosureParam;

/* 
  * RGB gains
  */
typedef struct {
	Uint16	redGain;			//Red gain, 0~511
	Uint16	greenGain[2];		//Green gain, two channels, 0~511
	Uint16	blueGain;			//Blue gain, 0~511
} CamRGBGains;

/* 
  * Dynamic range compression params 
  */
typedef struct {
	Uint8	strength;			//Adjust strength, 0~255
	Uint8	flag;				//Flag for function enable
	Uint8	reserved[8];		//reserved
} CamDRCParam;

#define DRC_FLAG_EN		(1 << 0)	//DRC enable

/* 
  * Rectangle region in image, used in other structures 
  */
typedef struct
{
	Uint16	startX;		//X position of Start point
	Uint16	startY;		//Y position of Start point
	Uint16	endX;		//X position of End point
	Uint16	endY;		//Y position of End point
} CamRect;

/* 
  * Traffic light correct regions 
  */
typedef struct {
	CamRect 	region[3];		//redlight regions
} CamTrafficLightRegions;

/* 
  * Image enhance params 
  */
typedef struct {
	Uint16	flags;			//control flag
	Uint16	contrast;		//value of contrast 
	Uint16	sharpness;		//value of sharpness
	Uint16	brightness;		//value of brightness, unused at current ver.
	Uint16	saturation;		//value of saturation, unused at current ver.
	Uint16	digiGain;		//digital gain
	Uint16	drcStrength;	//strength of DRC
	Uint16	gamma;			//gamma value plus 100
	Uint8	acSyncOffset;	//offset for AC Sync, 0~200, unit 0.1ms
	Uint8	reserved[7];	//reserved for further use	
}CamImgAdjCfg;

typedef struct {
	CamImgAdjCfg	dayCfg;		// cfg preset for day period
	CamImgAdjCfg	nightCfg;	// cfg preset for night period
}CamImgEnhanceParams;

/* Flags for CamImgAdjCfg.flags */
#define CAM_IMG_CONTRAST_EN		(1 << 0)	//enable contrast adjust
#define CAM_IMG_SHARP_EN		(1 << 1)	//enable sharpness adjust
#define CAM_IMG_GAMMA_EN		(1 << 2)	//enable brightness adjust
#define CAM_IMG_SAT_EN			(1 << 3)	//enable saturation adjust
#define CAM_IMG_MED_FILTER_EN	(1 << 4)	//enable median filter
#define CAM_IMG_NF_EN			(1 << 5)	//enable noise filter
#define CAM_IMG_DRC_EN			(1 << 6)	//enable dynamic range compression
#define CAM_IMG_BRIGHTNESS_EN	(1 << 7)	//enable brightness
#define CAM_IMG_ACSYNC_EN		(1 << 8)	//enable AC sync enable

/*
  * special capture params 
  * Note: specail capture may use auto exposure params diff from normal params 
 */
typedef struct {
	Uint32	expTime;		// Fixed exposure, Unit: us
	Uint16	globalGain;		// Fixed gain, Range: 0~1023
	Uint16	strobeEn;		// Bit[0:1]- strobe0, strobe1, 1-enable, 1-disable
	Uint32	aeMinExpTime;	// AE for special capture, Min exposure time, us
	Uint32	aeMaxExpTime;	// AE for special capture Max exposure time, us 
	Uint16	aeMinGain;		// AE for special capture, Min global gain, 0~1023
	Uint16	aeMaxGain;		// AE for special capture, Max global gain, 0~1023
	Uint16	aeTargetVal;	// AE for special capture, target lum value
	Uint16	flags;			// flags for ctrl
	Uint16	redGain;		// Fixed red gain, 0~511
	Uint16	greenGain;		// Fixed green gain, 0~511
	Uint16	blueGain;		// Fixed blue gain, 0~511
	Uint16	awbMinRedGain;	// AWB for special capture, min red gain, 0~511
	Uint16	awbMaxRedGain;	// AWB for special capture, max red gain, 0~511
	Uint16	awbMinGreenGain;// AWB for special capture, min green gain, 0~511
	Uint16	awbMaxGreenGain;// AWB for special capture, max green gain, 0~511
	Uint16	awbMinBlueGain; // AWB for special capture, min blue gain, 0~511
	Uint16	awbMaxBlueGain;	// AWB for special capture, max blue gain, 0~511
	Uint16	reserved[5];	
}CamSpecCapParams;

#define CAM_SPEC_CAP_FLAG_AE_EN		(1 << 0)	// enable ae for special trigger
#define CAM_SPEC_CAP_FLAG_AG_EN		(1 << 1)	// auto gain
#define CAM_SPEC_CAP_FLAG_AWB_EN	(1 << 2)	// auto white balance
#define CAM_SPEC_CAP_FLAG_AI_EN		(1 << 3)	// auto iris

/*
 * Auto exposure params
 */
#define CAM_AE_MAX_ROI_NUM			5

#define CAM_AE_FLAG_AE_EN			(1 << 0)	//enable Auto Exposure
#define CAM_AE_FLAG_AG_EN			(1 << 1)	//enable Auto Gain
#define CAM_AE_FLAG_AA_EN			(1 << 2)	//enable Auto Aperture

typedef struct
{
	Uint16		flags;					//Control flag
	Uint16 		targetValue;	 		//Target brightness, 0~255
	Uint32 		minShutterTime;			//Minimun shutter time, us
	Uint32 		maxShutterTime;			//Maximum shutter time, us
	Uint16 		minGainValue;			//Minimum global gain, 1~1023
	Uint16 		maxGainValue;			//Maxmum global gain, usMinGainValue~1023
	CamRect 	roi[CAM_AE_MAX_ROI_NUM]; //ROI region
	Uint8		minAperture;			//Min aperture value
	Uint8		maxAperture;			//Max aperture value
	Uint16		reserved;
}CamAEParam;

/*
 * Auto white balance params
 */
#define AWB_FLAG_EN		(1 << 0)		//enable AWB

typedef struct {
	Uint16		flags;					//control flag
	Uint16		maxValueR;				//max Red gain
    Uint16		maxValueG;				//max green gain, unused in current version
    Uint16		maxValueB;				//max blue gain
    Uint16		minValueR;				//min red gain
    Uint16		minValueG;				//min green gain, unused in current version
	Uint16		minValueB;				//min blue gain
	Uint16		redModifyRatio;			//red gain modify ratio
	Uint16		greenModifyRatio;		//green gain modify ratio, unused in current version
	Uint16		blueModifyRatio;		//blue gain modify ratio
	Uint16		initValueR[2];			//Init Red value at day and night, 0-day, 1-night 
	Uint16		initValueG[2];			//Init Green  value at day and night, 0-day, 1-night 
	Uint16		initValueB[2];			//Init Blue value at day and night, 0-day, 1-night 
} CamAWBParam;

/* Len for LUT update */
#define NET_LUT_LEN		4096

#endif


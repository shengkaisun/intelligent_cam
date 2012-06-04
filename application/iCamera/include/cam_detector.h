#ifndef __CAM_DETECTOR__
#define __CAM_DETECTOR__

#include "common.h"

/*
  * Vehicle detector config params
  */
  
/* Max Trigger ways support */
#define APP_MAX_CAP_CNT			4

typedef struct {
	Uint16  detecotorId;			//Type of detector
	Uint16	capDelayTime;			//3rd Image capture delay time, Uint: ms
	Uint16	redLightCapFlag;		//Flag for red light capture strategy 
	Uint16	greenLightCapFlag;		//Flag for green light capture strategy 
	Uint16	retrogradeCapFlag;		//Flag for retrograde capture strategy 
	Uint16	loopDist[APP_MAX_CAP_CNT];//distance between two loops, Uint: cm
	Uint8	limitSpeed;				//nominal limited speed, Uint: km/h
	Uint8	calcSpeed;				//actual speed limitation that triggers a capture, Uint: km/h
	Uint8	speedModifyRatio[APP_MAX_CAP_CNT]; //Ratio for speed modification
	Uint32	trigVideoLen;			//How much time to trigger H264 video, unit: Second, if set to 0, not trigger
	Int8	reserved[8];			//Reserved for future use
} CamDetectorParam;

enum VehicleDetectorId
{
	DETECTOR_IO = 0,			//IO trigger
	DETECTOR_TORY_EP,			//Tory epolice detector
	DETECTOR_TORY_CP,			//Tory checkpost detector
	DETECTOR_TORY_EP_V2,		//Tory new epolice detector
	DETECTOR_VIDEO_TRIG,		//Video trigger detector
	DETECTOR_MAX
};

/* Trigger Position bits define */
#define DETECTOR_FLAG_LOOP1_POS_CAP		(1 << 0)	//Capture @ positive edge of loop1
#define DETECTOR_FLAG_LOOP2_POS_CAP		(1 << 1)	//Capture @ positive edge of loop2
#define DETECTOR_FLAG_LOOP1_NEG_CAP		(1 << 2)	//Capture @ negative edge of loop1
#define DETECTOR_FLAG_LOOP2_NEG_CAP		(1 << 3)	//Capture @ negative edge of loop2
#define DETECTOR_FLAG_OVERSPEED_CAP		(1 << 4)	//capture only when overspeed
#define DETECTOR_FLAG_DELAY_CAP			(1 << 5)	//delay capture



/* Frame Num */
enum FrameID
{
	
	FRAME_TRIG_BASE = 0,		//base number of detector trigger
	
	FRAME_CONTINUE = 128,		//continous capture
	FRAME_MANUALTRIG,			//munual trigger
	FRAME_LIGHTDETECT,			//auto brightness detect
	FRAME_VIDEOTRIGGER,			//video trigger
	FRAME_MAX					//unused
};

/* captrue and trigger info generated at run time */
#define TRIG_INFO_RED_LIGHT		            (1 << 0)
#define TRIG_INFO_RETROGRADE				(1 << 1)
#define TRIG_INFO_OVERSPEED					(1 << 2)
#define TRIG_INFO_LAST_FRAME				(1 << 3)
#define TRIG_INFO_DELAY_CAP					(1 << 4)

/* single way trigger info */
typedef struct {
	Uint8			frameId; 			//Frame ID for Epolice 1-3, Checkpost 1-2, Retrograde, etc.
	Uint8			wayNum;				//Way number for trigger, start from 1
	Uint16			redlightTime; 		//Redlight time, unit: 10Ms
	Uint16			groupId; 			//Group Id
	Uint16			flags;				//Flag for extra info, i.e. overspeed
	Uint8			speed;				//Vehicle speed
	Int8			reserved[3];		//Reversed
}TriggerInfo;

/* Flags for capture info */
#define CAPINFO_FLAG_STROBE_FORCE		(1 << 0)
#define CAPINFO_FLAG_STROBE1_EN			(1 << 1)
#define CAPINFO_FLAG_STROBE2_EN			(1 << 2)
#define CAPINFO_FLAG_DELAY_CAP			(1 << 3)
#define CAPINFO_FLAG_OVERSPEED			(1 << 4)

/* capture info for one frame */
typedef struct {
	Uint32			capCnt;							//num of ways captured
	TriggerInfo		triggerInfo[APP_MAX_CAP_CNT];	//trigger info
	Uint16			flags;							//other Capture info, i.e. strobe 
	Uint8			limitSpeed;						//Limit speed
	Uint8			reserved[5];					//Reserved
}CaptureInfo;

#endif


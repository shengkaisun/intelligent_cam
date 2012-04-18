#include "common.h"
#include "img_ctrl.h"
#include "log.h"
#include <sys/ioctl.h>
#include <stropts.h>


#define DEV_NAME		"/dev/imgctrl"

typedef struct _TestParams {
	const char *devName;
	int abEn;
	int awbEn;
}TestParams;

static Bool main_loop(TestParams *params)
{
	Bool ret = FALSE;
	int fd;
	
	fd = open(params->devName, O_RDWR);
	if(fd < 0) {
		ERR("open %s failed", params->devName);
		goto exit;
	}

	/* test lum info */
	int err;
	struct hdcam_lum_info lumInfo;

	lumInfo.exposureTime = 2000;
	lumInfo.globalGain = 321;
	lumInfo.apture = 0;
	lumInfo.reserved = 0;

	err = ioctl(fd, IMGCTRL_S_LUM, &lumInfo);
	if(err) {
		ERR("set lum failed");
		goto exit;
	}

	err = ioctl(fd, IMGCTRL_G_LUM, &lumInfo);
	if(err) {
		ERR("get lum failed");
		goto exit;
	}

	usleep(10000);

	DBG("get lum info, exposure: %u, gain: %u", (__u32)lumInfo.exposureTime, (__u32)lumInfo.globalGain);

	struct hdcam_chroma_info chromaInfo;
	chromaInfo.redGain = 27;
	chromaInfo.greenGain = 59;
	chromaInfo.blueGain = 73;
	chromaInfo.reserved = 0;

	err = ioctl(fd, IMGCTRL_S_CHROMA, &chromaInfo);
	if(err) {
		ERR("set chroma failed");
		goto exit;
	}

	err = ioctl(fd, IMGCTRL_G_CHROMA, &chromaInfo);
	if(err) {
		ERR("get chroma failed");
		goto exit;
	}

	usleep(10000);
	
	DBG("get chroma, rgb: %u, %u, %u", 
		(__u32)chromaInfo.redGain, (__u32)chromaInfo.greenGain, (__u32)chromaInfo.blueGain);
	
	struct hdcam_ab_cfg abCfg;

	memset(&abCfg, 0, sizeof(abCfg));
	abCfg.flags = 0x03;
	abCfg.minShutterTime = 22;
	abCfg.maxShutterTime = 4567;
	abCfg.minGainValue = 0;
	abCfg.maxGainValue = 345;
	abCfg.targetValue = 75;
	
	err = ioctl(fd, IMGCTRL_S_ABCFG, &abCfg);
	if(err) {
		ERR("set ab failed");
		goto exit;
	}

	ret = TRUE;
exit:

	if(fd > 0)
		close(fd);

	return ret;
}

static void usage(void)
{
    INFO("imgctrlTest Compiled on %s %s with gcc %s", __DATE__, __TIME__, __VERSION__);
    INFO("Usage:");
    INFO("./osdTest [options]");
    INFO("Options:");
    INFO(" -h get help");
	INFO(" -d dev name, default: %s", DEV_NAME);
	INFO(" -b ab ctrl, 0-enable, 1-disable, default: enable");
	INFO(" -w awb ctrl, 0-enable, 1-disable, default: enable");
    INFO("Example:");
    INFO(" use default params: ./imgctrlTest");
    INFO(" use specific params: ./imgctrlTest -s /dev/imgctrl");
}

int main(int argc, char **argv)
{
	int c;
    char *options = "d:b:w:h";
	TestParams params;
	
	params.devName = DEV_NAME;
	params.abEn = 1;
	params.awbEn = 1;

	while ((c = getopt(argc, argv, options)) != -1) {
		switch (c) {
		case 'd':
			params.devName = optarg;
			break;
		case 'b':
			params.abEn = atoi(optarg);
			break;
		case 'w':
			params.awbEn = atoi(optarg);
			break;
		case 'h':
		default:
			usage();
			return -1;
		}
	}

	Bool ret = main_loop(&params);
	if(ret)
		INFO("test success!");
	else
		INFO("test failed!");

	exit(0);
}


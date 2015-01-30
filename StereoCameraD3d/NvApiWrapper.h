#ifndef	NV_API_WRAPPER_H
#define NV_API_WRAPPER_H 1

#include "stdafx.h"
#include "nvapi\nvapi.h"

class NvApiWrapper
{
	public: 
    static bool NvApiWrapper_Initialize()
	{
		if (NvAPI_Initialize() == 0)
		{
			return true;
		} 
		else
		{
			return false;
		}
    }

    static bool NvApiWrapper_Stereo_Enable()
	{
		if (NvAPI_Stereo_Enable() == 0)
		{
			return true;
		} 
		else
		{
			return false;
		}
    }

    static bool NvApiWrapper_Stereo_Disable()
	{
		if (NvAPI_Stereo_Disable() == 0)
		{
			return true;
		} 
		else
		{
			return false;
		}
    }
};


#endif
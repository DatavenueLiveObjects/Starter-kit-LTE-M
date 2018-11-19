//-------------------------------------------------------------------------------------------------

/**
 * @file GNSSComponent.c
 * get GNSS coordinates
 *
 *
 */
//-------------------------------------------------------------------------------------------------


#include "legato.h"
#include "interfaces.h"
#include "GNSSComponent.h"

//data path for location objects
#define GPS_LAT                             "lwm2m.6.0.0"
#define GPS_LONG                            "lwm2m.6.0.1"
#define GPS_ALTITUDE                        "lwm2m.6.0.2"
#define GPS_RADIUS                          "lwm2m.6.0.3"


typedef enum
{
	POSITION_LOCATION_NO = 0,
	POSITION_LOCATION_2D = 1,
	POSITION_LOCATION_3D =2
} position_location_type_t;

//Global variables
static le_posCtrl_ActivationRef_t   		_posCtrlRef = NULL;

//--------------------------------------------------------------------------------------------------
/**
 * retrieve the current location, automatically get 2D or 3D location depending on the gnss fix state
 *
 */
//--------------------------------------------------------------------------------------------------
position_location_type_t position_GetLocation
(
	double*     			dLatitude,
	double*     			dLongitude,
	int32_t*    			hAccuracy,
	int32_t*    			altitude,
	int32_t*    			vAccuracy,
	le_pos_FixState_t * 	fixStatePtr
)

{

	position_location_type_t	ret = POSITION_LOCATION_NO;    //0=KO, 1=2D, 2=3D
	le_pos_FixState_t			fixState = LE_POS_STATE_UNKNOWN;
	le_result_t					res;
	int32_t						latitude;
	int32_t						longitude;

	if (LE_OK == le_pos_GetFixState(&fixState))
	{
		LE_INFO("position fix state %d", fixState);
		if (LE_POS_STATE_FIX_3D == fixState)
		{
			res = le_pos_Get3DLocation(&latitude, &longitude, hAccuracy, altitude, vAccuracy);
			LE_INFO("le_pos_Get3DLocation %s",
					(LE_OK == res) ? "OK" : (LE_OUT_OF_RANGE == res) ? "parameter(s) out of range":"ERROR");
			LE_INFO("Get3DLocation latitude.%d, longitude.%d, hAccuracy.%d, altitude.%d, vAccuracy.%d", latitude, longitude, *hAccuracy, *altitude, *vAccuracy);

			if (LE_OK == res)
			{
				*dLatitude = (double)latitude/1000000.0;
				*dLongitude = (double)longitude/1000000.0;
				ret = POSITION_LOCATION_3D;
			}
		}

		else
		{
			res = le_pos_Get2DLocation(&latitude, &longitude, hAccuracy);
			LE_INFO("le_pos_Get2DLocation %s",
					(LE_OK == res) ? "OK" : (LE_OUT_OF_RANGE == res) ? "parameter(s) out of range":"ERROR");
			LE_INFO("Get2DLocation latitude.%d, longitude.%d, hAccuracy.%d",
					latitude, longitude, *hAccuracy);
			if (LE_OK == res)
			{
				*dLatitude = (double)latitude/1000000.0;
				*dLongitude = (double)longitude/1000000.0;
				*altitude = 0;
				*vAccuracy = 0;
				ret = POSITION_LOCATION_2D;
			}
		}            
	}
	else
	{
		LE_INFO("Failed to GetFixState");
	}
	if (fixStatePtr)
	{
		*fixStatePtr = fixState;
	}
	return ret;
}

//--------------------------------------------------------------------------------------------------
/**
 * get GNSS lattitude & longitude
 *
 * latitude : double
 * longitude : double
 *
 */
//--------------------------------------------------------------------------------------------------
void GNSS_get(
    double*	latitude,
    double*	longitude
)
{
    double	dlatitude;
    double	dlongitude;
    int32_t	hAccuracy;
    int32_t	altitude;
    int32_t	vAccuracy;
    le_pos_FixState_t 	fixState;
    
    position_location_type_t ret = position_GetLocation(&dlatitude, &dlongitude, &hAccuracy, &altitude, &vAccuracy, &fixState);
    
    *latitude = dlatitude;
    *longitude = dlongitude;
    
    if (POSITION_LOCATION_3D == ret)
	{
		LE_INFO("position 3D");
	}
	else if (POSITION_LOCATION_2D == ret)
	{
		LE_INFO("position 2D");
	}else {
        LE_INFO("position none");
    }
}

//--------------------------------------------------------------------------------------------------
/**
 * The GNSS position is computed and delivered each acquisition rate 
 *
 * rate : int (in milliseconds)
 *
 */
//--------------------------------------------------------------------------------------------------
void GNSS_start(
    int acquisitionRate
)
{
    le_pos_SetAcquisitionRate(acquisitionRate);

    _posCtrlRef = le_posCtrl_Request();
    
    if (NULL == _posCtrlRef)
    {
        LE_INFO("Cannot activate le_pos !");
    } else {
        LE_INFO("GNSS ready !");
        
        double	latitude;
        double	longitude;
        GNSS_get(&latitude, &longitude);
    }
}


COMPONENT_INIT
{

    
}
#include "legato.h"
#include "interfaces.h"
#include "dataProfileComponent.h"

//------
/**
 * Orange France network settings
 */
/*
char                                        _profileAPN[] = "orange.ltem.spec";
char                                        _profileUser[] = "orange";
char                                        _profilePwd[] = "orange";
le_mdc_Auth_t                                _profileAuth = LE_MDC_AUTH_PAP;
int											_dataProfileIndex = 20;
*/


void dataProfile_set(
		int index,
		char* apn,
		le_mdc_Auth_t  auth,
		char* user,
		char* pwd
)
{
	int32_t  profileIndex = le_data_GetCellularProfileIndex();

	char currentApn[10];
	size_t apn_size= 10;

	le_mdc_ProfileRef_t curentProfile = le_mdc_GetProfile((uint32_t) profileIndex);

	le_mdc_GetAPN(curentProfile, currentApn, apn_size);

	if (currentApn != apn) {
		LE_INFO("Set Orange Network profile.");

		le_mdc_ProfileRef_t dataProfile = le_mdc_GetProfile((uint32_t) index);
		le_mdc_SetAPN(dataProfile, apn);
		le_mdc_SetAuthentication(dataProfile, auth, user, pwd);

		le_data_SetCellularProfileIndex(index);
		LE_INFO("Orange Network profile.  profile index: %d", le_data_GetCellularProfileIndex());

	} else {
		LE_INFO("Orange Network profile ok.");
	}
}


COMPONENT_INIT
{

}

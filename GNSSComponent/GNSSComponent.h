#ifndef GNSSCOMPONENT_H_INC
#define GNSSCOMPONENT_H_INC

LE_SHARED void GNSS_get(double* latitude, double*	longitude);
LE_SHARED void GNSS_start(int acquisitionRate);

#endif

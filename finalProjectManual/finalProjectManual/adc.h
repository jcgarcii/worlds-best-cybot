/*
 * adc.h
 *
 *  Created on: Mar 16, 2021
 *      Author: egabriel
 */




#ifndef ADC_H_
#define ADC_H_

#include "REF_tm4c123gh6pm.h"
#include <math.h>


void adc_init(void);

double adc_read(void);


#endif /* ADC_H_ */

#ifndef __FILTER_H
#define __FILTER_H	 

typedef struct{
	double raw_value;
	double xbuf[5];
	double ybuf[5];
	double filtered_value;
}Filter_t;

extern Filter_t mpuYaw50HZ, mpuPitch50HZ;
extern Filter_t PIDOUTPUT50Hz;
void Chebyshev50HzLPF(Filter_t *F);
void mFilterPutData(Filter_t *F, float x);

#endif


#ifndef LOCATE_H
#define LOCATE_H
#include "encoder.h"
typedef struct
{
	float x;
	float y;
}PointTypeDef;

extern PointTypeDef point_now;
void locate_update(void);

#endif


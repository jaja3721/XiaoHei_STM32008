#include "locate.h"
#include "math.h"
#include "IMU_ADIS16xxx.h"
PointTypeDef point_now;
void locate_update()
{
	point_now.x += velocity*sin(yaw)*0.05;
	point_now.y += velocity*cos(yaw)*0.05;
}

#include "KF.h"
void KF_init()
{
		
}
//q1:四元数 Quater 东北天速度
//functionvelnavi(				q1,							vel1,						lati1,					longi1,				h1,					caldata)
void ins_update(double *q, double *vel1, double *latil, double *longil, double *h1, double *caldata)
{
	double wie=7.292115e-5;
	uint32 R=6378137;
	double f=1/298.257223563;
	double e=sqrt(f*(2-f));
	uint8_t Nsam=2;
	double T=1/90;
	double wien[]={0, wie*cos(lati1), wie*sin(lati1)};
	double temp=1-e^2*sin(lati1)*sin(lati1);
	double RN=R*(1-e^2)/temp^1.5;
	double RE=R/temp^0.5;
	double wenn[]={-vel1(2)/(RN+h1), vel1(1)/(RE+h1), vel1(1)*tan(lati1)/(RN+h1); 
	double g0=9.7803267711905*(1+0.00193185138639*sin(lati1)^2)/((1-0.00669537999013*sin(lati1)^2)^0.5*(1+h1/R)^2);
		
	double gln[3][1];
	double cbn[][]={q(0)*q(0)+q(1)*q(1)-q(2)*q(2)-q(3)*q(3), 2*(q(1)*q(2)-q(0)*q(3)), 2*(q(1)*q(3)+q(0)*q(2));
						2*(q(1)*q(2)+q(0)*q(3)), q(0)*q(0)-q(1)*q(1)+q(2)*q(2)-q(3)*q(3), 2*(q(2)*q(3)-q(0)*q(1));
						 2*(q(1)*q(3)-q(0)*q(2)), 2*(q(2)*q(3)+q(0)*q(1)), q(0)*q(0)-q(1)*q(1)-q(2)*q(2)+q(3)*q(3)]};
	double wnbb[2][3]={caldata[0];caldata[3]};//****
			
	//double fbscull=sum( caldata(:,4:6),1)'+1/2*cross( sum(wnbb,1)',sum(caldata(:,4:6),1)')+2/3*(cross(wnbb(1,:)',caldata(Nsam,4:6)')+cross(caldata(1,4:6)',wnbb(Nsam,:)'));
			
			
	
}

get_cbn_from_q()
{}
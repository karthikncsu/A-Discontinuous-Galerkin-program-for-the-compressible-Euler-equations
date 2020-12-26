//Subroutine for calculating the fluxes for reconstructed P0 methods
#include "functions.h"
#include "twod_header.h"

void flux2D_eul_calc_recons()
{
	
int ileft,iright,ip1,ip2;
double u1left,u2xleft,u2yleft,u3left;
double u1right,u2xright,u2yright,u3right;
double rho0,velx0,vely0,pres0,c0,vdotn;
double fiplus0,fiplus1x,fiplus1y,fiplus2;
double fiminus0,fiminus1x,fiminus1y,fiminus2;
double Mach;
double xcl,ycl;

double rhoin,velxin,velyin,u3in,presin,cin;
double velxg,velyg,rhog,u3g,presg;
double velxref,velyref,Machn;
double velnref,veltref,velnb,veltb,cref,r1,r2,r3,r4,velng,veltg,cg;

if(limiter==1)
{
 flux2D_eul_calc_recons_limiter();
 return ;
}

if(limiter==1)
{
	cout<<"entering the subroutine reconstruction without limiter, even though limiter option as been selected"<<endl;
	exit(1);
}


recons_calculation_euler();

for(int iface=1;iface<=naface;iface++)
{
//------------------------------Left cell calculation--------------------------	
	ileft=intface[iface][1];
	iright=intface[iface][2];
	ip1=intface[iface][3];
	ip2=intface[iface][4];
	
	xcl=0.5*(coord[ip1][1]+coord[ip2][1]);
	ycl=0.5*(coord[ip1][2]+coord[ip2][2]);

	u1left=unkel[ileft][0][0]+(gradu[ileft][0][0]*(xcl-centerx[ileft])+gradu[ileft][0][1]*(ycl-centery[ileft]));
	u2xleft=unkel[ileft][1][0]+(gradu[ileft][1][0]*(xcl-centerx[ileft])+gradu[ileft][1][1]*(ycl-centery[ileft]));
	u2yleft=unkel[ileft][2][0]+(gradu[ileft][2][0]*(xcl-centerx[ileft])+gradu[ileft][2][1]*(ycl-centery[ileft]));
	u3left=unkel[ileft][3][0]+(gradu[ileft][3][0]*(xcl-centerx[ileft])+gradu[ileft][3][1]*(ycl-centery[ileft]));
	
	rho0=u1left;
	velx0=u2xleft/u1left;
	vely0=u2yleft/u1left;
	pres0=(gama-1)*(u3left-0.5*(velx0*velx0+vely0*vely0)*rho0);
	c0=sqrt(gama*max(pres0,hrd_lmtr_cin)/rho0);
	
	vdotn=velx0*nx[iface]+vely0*ny[iface];

	Mach=vdotn/c0;
	
	if(Mach<-1.0)
	{
	fiplus0=0;
	fiplus1x=0;
	fiplus1y=0;
	fiplus2=0;
    }
	else if(Mach>=-1.0&&Mach<=1.0)
	{
	double factor;
	
	factor=rho0*c0*(Mach+1)*(Mach+1)*0.25;
	
	fiplus0=factor;
	fiplus1x=factor*(velx0+nx[iface]*(-vdotn+2*c0)/gama);
	fiplus1y=factor*(vely0+ny[iface]*(-vdotn+2*c0)/gama);
	
	fiplus2=0.5*(velx0*velx0+vely0*vely0)-0.5*vdotn*vdotn;
	fiplus2=fiplus2+((gama-1)*vdotn+2.0*c0)*((gama-1)*vdotn+2.0*c0)/(2.0*gama*gama-2.0);
	fiplus2=factor*fiplus2;
    }
	else if(Mach>1.0)
	{
	fiplus0=rho0*vdotn;
	fiplus1x=rho0*vdotn*velx0+pres0*nx[iface];
	fiplus1y=rho0*vdotn*vely0+pres0*ny[iface];
	fiplus2=vdotn*(u3left+pres0);
    }
//------------------------End of Left cell calculation--------------------------	
    
//-------------Fj-(right cell) calcualtion---------------
if(iright<=nelem)
{
	u1right=unkel[iright][0][0]+(gradu[iright][0][0]*(xcl-centerx[iright])+gradu[iright][0][1]*(ycl-centery[iright]));
	u2xright=unkel[iright][1][0]+(gradu[iright][1][0]*(xcl-centerx[iright])+gradu[iright][1][1]*(ycl-centery[iright]));
	u2yright=unkel[iright][2][0]+(gradu[iright][2][0]*(xcl-centerx[iright])+gradu[iright][2][1]*(ycl-centery[iright]));
	u3right=unkel[iright][3][0]+(gradu[iright][3][0]*(xcl-centerx[iright])+gradu[iright][3][1]*(ycl-centery[iright]));
	
	rho0=u1right;
	velx0=u2xright/u1right;
	vely0=u2yright/u1right;
	pres0=(gama-1)*(u3right-0.5*(velx0*velx0+vely0*vely0)*rho0);
	c0=sqrt(gama*max(pres0,hrd_lmtr_cin)/rho0);
	
	vdotn=velx0*nx[iface]+vely0*ny[iface];

	Mach=vdotn/c0;
		
		if(Mach>1.0)
		{
		fiminus0=0;
		fiminus1x=0;
		fiminus1y=0;
		fiminus2=0;
		}
		else if(Mach>=-1.0&&Mach<=1.0)
		{
			double factor;
		factor=-rho0*c0*(Mach-1)*(Mach-1)*0.25;
	
		fiminus0=factor;
		fiminus1x=factor*(velx0+nx[iface]*(-vdotn-2*c0)/gama);
		fiminus1y=factor*(vely0+ny[iface]*(-vdotn-2*c0)/gama);
	
		fiminus2=0.5*(velx0*velx0+vely0*vely0)-0.5*vdotn*vdotn;
		fiminus2=fiminus2+((gama-1)*vdotn-2.0*c0)*((gama-1)*vdotn-2.0*c0)/(2*gama*gama-2.0);
		fiminus2=factor*fiminus2;
		}
		else if(Mach<-1.0)
		{
		fiminus0=rho0*vdotn;
		fiminus1x=rho0*vdotn*velx0+pres0*nx[iface];
		fiminus1y=rho0*vdotn*vely0+pres0*ny[iface];
		fiminus2=vdotn*(u3right+pres0);
		}

}
else
{
		rhoin=u1left;
		velxin=u2xleft/u1left;
		velyin=u2yleft/u1left;
		u3in=u3left;
		
		presin=(gama-1)*(u3in-0.5*(velxin*velxin+velyin*velyin)*rhoin);
		cin=sqrt(gama*max(presin,hrd_lmtr_cin)/rhoin);
	
		vdotn=velxin*nx[iface]+velyin*ny[iface];
		
		if(bound_cond[iface][1]==2.0) //Wall boundary condition
		{
				velxg=velxin-2*vdotn*nx[iface];
				velyg=velyin-2*vdotn*ny[iface];
		
				rhog=rhoin;
				u3g=u3in;
		}
		else if(bound_cond[iface][1]==4.0) //Far field boundary condition
		{
				Machn=vdotn/cin;
			
			    velxref=velref*cos(alpha*3.14159265/180.0);
				velyref=velref*sin(alpha*3.14159265/180.0);
				if(Machn<=1.0)
				{
						velxg=velxref;
						velyg=velyref;
		
						rhog=rhoref;
						u3g=presref/(gama-1)+0.5*rhog*(velxg*velxg+velyg*velyg);
				}
				else if(Machn>=1.0)
				{
						velxg=velxin;
						velyg=velyin;
		
						rhog=rhoin;
						u3g=u3in;
				}
				
				else if(Machn<=0&&Machn>-1.0)
				{
					velnref=velxref*(nx[iface])+velyref*(ny[iface]);
					veltref=velxref*(-ny[iface])+velyref*(nx[iface]);
			
					velnb=velxin*nx[iface]+velyin*ny[iface];
					veltb=velxin*(-ny[iface])+velyin*(nx[iface]);
					cref=sqrt(gama*presref/rhoref);
					
					r1=velnref-2.0*cref/(gama-1);
					r2=veltref;
					r3=presref/pow(rhoref,gama);
					r4=velnb+2.0*cin/(gama-1);
			
					velng=0.5*(r4+r1);
					cg=(gama-1)*0.25*(r4-r1);
					veltg=r2;
			
					rhog=pow(cg*cg/(r3*gama),1.0/(gama-1.0));
					presg=r3*pow(rhog,gama);
			
					velxg=velng*nx[iface]-veltg*ny[iface];
					velyg=velng*ny[iface]+veltg*nx[iface];
			
					u3g=presg/(gama-1)+0.5*rhog*(velxg*velxg+velyg*velyg);
				}
				else if(Machn<1&&Machn>0.0)
				{
					
					velnref=velxref*(nx[iface])+velyref*(ny[iface]);
					veltref=velxref*(-ny[iface])+velyref*(nx[iface]);
			
					velnb=velxin*nx[iface]+velyin*ny[iface];
					veltb=velxin*(-ny[iface])+velyin*(nx[iface]);
			
					cref=sqrt(gama*presref/rhoref);
					
					r1=velnref-2.0*cref/(gama-1);
					r2=veltb;
					r3=presin/pow(rhoin,gama);
					r4=velnb+2*cin/(gama-1);
			
					velng=0.5*(r4+r1);
					cg=(gama-1)*0.25*(r4-r1);
					veltg=r2;
			
					rhog=pow(cg*cg/(r3*gama),1.0/(gama-1.0));
					presg=r3*pow(rhog,gama);
			
					velxg=velng*nx[iface]-veltg*ny[iface];
					velyg=velng*ny[iface]+veltg*nx[iface];
			
					u3g=presg/(gama-1)+0.5*rhog*(velxg*velxg+velyg*velyg);
				 }
		}

	rho0=rhog;
	velx0=velxg;
	vely0=velyg;
	
	pres0=(gama-1)*(u3g-0.5*(velx0*velx0+vely0*vely0)*rho0);
	c0=sqrt(gama*max(pres0,hrd_lmtr_cin)/rho0);
	
	vdotn=velx0*nx[iface]+vely0*ny[iface];

	Mach=vdotn/c0;
		
		if(Mach>1.0)
		{
		fiminus0=0;
		fiminus1x=0;
		fiminus1y=0;
		fiminus2=0;
		}
		else if(Mach>=-1.0&&Mach<=1.0)
		{
			double factor;
		factor=-rho0*c0*(Mach-1)*(Mach-1)*0.25;
	
		fiminus0=factor;
		fiminus1x=factor*(velx0+nx[iface]*(-vdotn-2*c0)/gama);
		fiminus1y=factor*(vely0+ny[iface]*(-vdotn-2*c0)/gama);
	
		fiminus2=0.5*(velx0*velx0+vely0*vely0)-0.5*vdotn*vdotn;
		fiminus2=fiminus2+((gama-1)*vdotn-2.0*c0)*((gama-1)*vdotn-2.0*c0)/(2*gama*gama-2.0);
		fiminus2=factor*fiminus2;
		}
		else if(Mach<-1.0)
		{
		fiminus0=rho0*vdotn;
		fiminus1x=rho0*vdotn*velx0+pres0*nx[iface];
		fiminus1y=rho0*vdotn*vely0+pres0*ny[iface];
		fiminus2=vdotn*(u3g+pres0);
		}
}
		
    
 //---------------------Calculating rhspo------------------------------
    
   	if(ileft<=nelem)
   	{
    rhspo[ileft][0][0]=rhspo[ileft][0][0]-(fiplus0+fiminus0)*length[iface];
	rhspo[ileft][1][0]=rhspo[ileft][1][0]-(fiplus1x+fiminus1x)*length[iface];
    rhspo[ileft][2][0]=rhspo[ileft][2][0]-(fiplus1y+fiminus1y)*length[iface];
    rhspo[ileft][3][0]=rhspo[ileft][3][0]-(fiplus2+fiminus2)*length[iface];
	}
    
    if(iright<=nelem)
    {
    rhspo[iright][0][0]=rhspo[iright][0][0]+(fiplus0+fiminus0)*length[iface];
    rhspo[iright][1][0]=rhspo[iright][1][0]+(fiplus1x+fiminus1x)*length[iface];
    rhspo[iright][2][0]=rhspo[iright][2][0]+(fiplus1y+fiminus1y)*length[iface];
    rhspo[iright][3][0]=rhspo[iright][3][0]+(fiplus2+fiminus2)*length[iface];
	}
	
}



}



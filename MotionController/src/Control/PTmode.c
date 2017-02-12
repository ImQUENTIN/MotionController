
#include "sysTypes.h"
#include "PTmode.h"

#define  pt_segment_start 1
#define  pt_segment_normal 2
#define  pt_segment_stop 3

typedef struct
{
	int accnow;
    int velnow;
	int posnow;
}comandbuf;


int ptmode(int axis, double pos, int time,int type,int period)
{
		uint16_t div_num;
		uint16_t i;

		float acc;
		float vel;
		float divpos;

		comandbuf  combuf;

		static float prevel[8]={0};
		static float pretotalt[8]={0};
		static double prepos[8]={0};

		int cmdpos;
		static float recoup=0;
		  if(type == pt_segment_start)
		     {
		        div_num = time / period;

		        if(pos<0)
		  	  {
		  			acc = -2*pos/(time*time);//求加速度
		  	  }
		  	  else
		  	  {
		  			acc = 2*pos/(time*time);
		        }


		        for(i=0;i<div_num;i++)//分段循环
		        {
		  		vel = acc * i * period;
		  		if(pos<0)
		  		{
		  			divpos = -(vel*period + 0.5*acc*period*period);
		  	    }else
		  		{
		  			divpos = vel*period + 0.5*acc*period*period;
		  		}

		  	    combuf.accnow = (int)(acc_to_cmd * acc *1000000 + 0.5);
		  	    combuf.velnow =(int)(vel_to_cmd * vel * 1000 + 0.5);
		  	  cmdpos = (int)(divpos +recoup+0.5);
		  	    recoup = (float)(divpos +recoup-cmdpos);//算余数
		  	  combuf.posnow = cmdpos;
		  	   // mtputdatatofifo( &stmtcmd, axis);
		        }

		        prevel[axis]=div_num*acc*period;
		        prepos[axis]=pos;
		        pretotalt[axis]= time;
		     }

		    if(type == pt_segment_normal)
		    {
		       div_num = (time-pretotalt[axis])/period;
		      if(pos<0){
		        acc= -((pos-prepos[axis])+prevel[axis]*(time-pretotalt[axis]))*2/((time-pretotalt[axis])*(time-pretotalt[axis]));
		      }else{
		        acc=((pos-prepos[axis])-prevel[axis]*(time-pretotalt[axis]))*2/((time-pretotalt[axis])*(time-pretotalt[axis]));
		      }
		      for(i=0;i<div_num;i++)
		      {
		        vel = prevel[axis]+acc*i*period;
		        if(pos<0){
		  	divpos=-(vel*period+0.5*acc*period*period) ;
		  	cmdpos=(int)(divpos+recoup-0.5);
		        }else{
		  	divpos=vel*period+0.5*acc*period*period ;
		  	cmdpos=(int)(divpos+recoup+0.5);
		        }
		       // stmtcmd.cmdacc = (int)(acc_to_cmd * acc *1000000 + 0.5);
		        combuf.accnow = (int)(acc_to_cmd * acc *1000000 + 0.5);
		        combuf.velnow =(int)(vel_to_cmd * vel * 1000 + 0.5);
		        recoup = (float)(divpos +recoup-cmdpos);
		        combuf.posnow = cmdpos;
		        //mtputdatatofifo(&stmtcmd, axis);
		      }
		      prevel[axis]=prevel[axis]+div_num*acc*period;
		      prepos[axis]=pos;
		      pretotalt[axis]=time;
		    }

		    pos = 640000;
		    time = 20000;

		    if(type == pt_segment_stop)
		    {
		       div_num=(time-pretotalt[axis])/period;
		      if(pos<0){
		        acc=2*(pos-prepos[axis])/((time-pretotalt[axis])*(time-pretotalt[axis]));
		      }else{
		        acc=-2*(pos-prepos[axis])/((time-pretotalt[axis])*(time-pretotalt[axis]));
		      }
		      prevel[axis]= -acc*(time-pretotalt[axis]);
		      for(i=0;i<div_num;i++)
		      {
		        if(pos<0){
		  	vel=prevel[axis]-acc*i*period;
		  	divpos=-vel*period + 0.5*acc*period*period;
		  	cmdpos= (int)(divpos+recoup-0.5);
		        }else{
		  	vel=prevel[axis]+acc*i*period;
		  	divpos=vel*period + 0.5*acc*period*period;
		  	cmdpos= (int)(divpos+recoup+0.5);
		        }
		        combuf.accnow = (int)(acc_to_cmd * acc *1000000 - 0.5);
		        combuf.velnow =(int)(vel_to_cmd * vel * 1000 + 0.5);
		        recoup = (float)(divpos +recoup-cmdpos);
		        combuf.posnow = cmdpos;
		        //mtputdatatofifo(&stmtcmd, axis);
		      }
		    }
		    return div_num;
		  }

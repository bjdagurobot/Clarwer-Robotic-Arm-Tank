
/*==========================定义===========================*/
#include <PS2X_lib.h>  //库版本 v1.6
#include <Servo.h>    
PS2X ps2x;             //创建PS2控制格式
int error;
byte vibrate = 0;
Servo servo1;         //声明舵机servo1
Servo servo2;         //声明舵机servo2
Servo servo3;         //声明舵机servo3
Servo servo4;         //声明舵机servo4
int servoPin1 = 2;    //设置D2引脚名字为servoPin1
int servoPin2 = 3;    //设置D3引脚名字为servoPin2
int servoPin3 = 4;    //设置D4引脚名字为servoPin3
int servoPin4 = 7;    //设置D7引脚名字为servoPin4
int armSp =8;
int green=0;
int one[]={85,180,0};
int two[]={70,155};
int three[]={60,180};
int four[]={75,110};
int a[]={one[0],two[0],three[0],four[0]};  //机器臂初始位置
int b[]={one[0],two[0],three[0],four[1]};  //机器臂初始位置,夹头张开
int c[]={one[0],two[1],three[0],four[1]};  //机器臂下降,夹头张开，准备抓取
int d[]={one[0],two[1],three[0],four[0]};  //机器臂下降,夹头闭合，抓取
int e[]={one[0],two[0],three[0],four[0]};  //机器臂回位,已抓取
int f[]={one[1],two[0],three[0],four[0]};  //机器臂转左，已抓取
int g[]={one[1],two[1],three[0],four[0]};  //机器臂转左，下降，已抓取
int h[]={one[1],two[1],three[0],four[1]};  //机器臂转左，下降，放置
int j[]={one[1],two[0],three[0],four[1]};  //机器臂上升，准备好归位
int k[]={one[2],two[0],three[0],four[0]};     //机器臂转右，已抓取
int l[]={one[2],two[1],three[0],four[0]};    //机器臂转右，下降，已抓取
int m[]={one[2],two[1],three[0],four[1]};   //机器臂转右，下降，放置
int n[]={one[2],two[0],three[0],four[1]};    //机器臂上升，准备好归位
int data[]={one[0],two[0],three[0],four[0]};  //用于控制机器臂动作

const int leftR = 11; //左轮转动方向，引脚D11
const int leftS = 6; //左轮转动速度，引脚D6
const int rightR = 10; //右轮转动方向，引脚D10
const int rightS = 5; //右轮转动方向，引脚D5
int dataSp[]={70,120,170,220};        //小车速度
int sp=0;
int carSp;

int state=0;
/*==========================结束===========================*/

/*==========================初始化===========================*/
void setup() {
error = ps2x.config_gamepad(A0,A2,A1,A3, true, true);   //PS接线定义
servo1.attach(servoPin1);   //设置servo1的引脚为D2
servo2.attach(servoPin2);   //设置servo2的引脚为D3
servo3.attach(servoPin3);   //设置servo3的引脚为D4
servo4.attach(servoPin4);   //设置servo4的引脚为D7
//小车控制脚定义
pinMode(leftR,OUTPUT);
pinMode(leftS,OUTPUT);
pinMode(rightR,OUTPUT);
pinMode(rightS,OUTPUT);
carSp=dataSp[sp];
carStop();
set_arm();
}
/*==========================结束===========================*/

/*==========================主循环===========================*/
void loop() {
if(error ==1 )return;
ps2x.read_gamepad(false, vibrate);
if(ps2x.Analog(PSS_LY)>=170)   
state=1;
if(ps2x.Analog(PSS_LY)<=10)   
state=2;
if(ps2x.Analog(PSS_LX)>=170)   
state=3;
if(ps2x.Analog(PSS_LX)>=170)   
state=4;
if(state==1)  //定距跟随
{}
if(state==2)  //倾倒恢复
{}
if(state==3)  //循迹夹放
{}
if(state==4)  //智能避障
{}

//-------------------------------------------------------------//
if(state==0)   //手动模式
{
/*======================分割线=================================*/
if(ps2x.ButtonPressed(PSB_L1))
{
  if(sp<3)
  { sp++;
    carSp=dataSp[sp];
  }
}
/*======================分割线=================================*/
if(ps2x.ButtonPressed(PSB_L2))
{
  if(sp>0)
  { sp--;
    carSp=dataSp[sp];
  } 
  
  }
/*======================分割线=================================*/
if(ps2x.Button(PSB_PAD_UP))     //小车控制
 {
 carGo(carSp);
 }
 else if(ps2x.Button(PSB_PAD_DOWN))
 {
 carBack(carSp);
 }
 else if(ps2x.Button(PSB_PAD_LEFT))
  {
 carLeft(carSp);
 }
 else if(ps2x.Button(PSB_PAD_RIGHT))
 {
 carRight(carSp);
 }
 else
 {carStop();}
/*======================分割线=================================*/
if(ps2x.ButtonPressed(PSB_GREEN))   //绿色键功能
    {
      if(green==0 && data[1]==two[0] && data[0]==one[0])
      {
     armReady();
     green=1;
     }
     else if(green==1)
     {
      armF();
      green=0;
      }
    }
/*======================分割线=================================*/    
if(ps2x.ButtonPressed(PSB_BLUE))   //蓝色键功能
    {
      if(data[3]==four[0])
      {
       data[3]=four[1];
       armMove();
     }
     else 
     {
      data[3]=four[0];
      armMove();
      }
    }
/*======================分割线=================================*/
if(ps2x.ButtonPressed(PSB_PINK))  
{
  if(data[1]==two[0])
  armL();
  }
/*======================分割线=================================*/
if(ps2x.ButtonPressed(PSB_RED))  
{
  if(data[1]==two[0])
  armR();
  }
/*======================分割线=================================*/
if(ps2x.Button(PSB_R1)) 
{
  if(data[2]<170)
  {
   data[2]=data[2]+2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   delay(5);
    }  
}
/*======================分割线=================================*/
if(ps2x.Button(PSB_R2)) 
{
  if(data[2]>=60)
  {
   data[2]=data[2]-2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   //delay(5);
    }  
}
/*======================分割线=================================*/
if(ps2x.Analog(PSS_RX)<10)                       //R摇杆往左打，舵机往左转
{ if(data[0]<=180)
   {
    data[0]=data[0]+2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   }
}
else if(ps2x.Analog(PSS_RX)>=170)               //R摇杆往右打，舵机往右转
{ if(data[0]>=0)
   {
    data[0]=data[0]-2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   }
}
/*======================分割线=================================*/
if(ps2x.Analog(PSS_RY)<10)
{ if(data[1]<=155)
   {
    data[1]=data[1]+2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   }
}
else if(ps2x.Analog(PSS_RY)>=170)
{ if(data[1]>=70)
   {
    data[1]=data[1]-2;
   servo1.write(data[0]);   //85  180
   servo2.write(data[1]);   //70   155
   servo3.write(data[2]);   //60
   servo4.write(data[3]);   //75  110
   }
}
/*======================分割线=================================*/
}



/*======================分割线=================================*/
if(ps2x.Button(PSB_SELECT))  
 {
  data[1]=70;
  armMove();
  delay(20);
  set_arm();
  delay(20);
  green=0;
  state=0;
  }
/*======================分割线=================================*/
delay(20);
}


/*==========================结束===========================*/


/*==========================小车动作函数===========================*/
void carGo(int carSp)
{
  //前进
  digitalWrite(leftR,HIGH);
  analogWrite(leftS,carSp);
  digitalWrite(rightR,HIGH);
  analogWrite(rightS,carSp);
}
void carBack(int carSp)
{
   //后退
  digitalWrite(leftR,LOW);
  analogWrite(leftS,carSp);
  digitalWrite(rightR,LOW);
  analogWrite(rightS,carSp);
}
void carStop()
{
  //停止
  digitalWrite(leftS,LOW);
  digitalWrite(rightS,LOW);
}
void carLeft(int carSp)
{
  //左转
  digitalWrite(leftR,LOW);
  analogWrite(leftS,carSp);
  digitalWrite(rightR,HIGH);
  analogWrite(rightS,carSp);
}
void carRight(int carSp)
{
  //右转
  digitalWrite(leftR,HIGH);
  analogWrite(leftS,carSp);
  digitalWrite(rightR,LOW);
  analogWrite(rightS,carSp);
}
/*==========================结束===========================*/

/*==========================机器臂动作函数===========================*/
//舵机实现动作
void armMove()
{
 for(int i=0;i<20;i++)       //for循环，用于等待舵机转到位
 {
 servo1.write(data[0]);   //85  180
 servo2.write(data[1]);   //70   155
 servo3.write(data[2]);   //60
 servo4.write(data[3]);   //75  110
 delay(armSp);
 }
 delay(50);
  }

//初始化机器臂位置
void set_arm() 
{
for(int i=0;i<=3;i++)
{
  data[i]=a[i];
  }
armMove();
 }
 
//机器臂准备夹取
void armReady()
{
  for(int i=0;i<=3;i++)
{
  data[i]=c[i];
  }
armMove();
  }
  
//机器臂夹取
void armTake()
{
  for(int i=0;i<=3;i++)
{
  data[i]=d[i];
  }
armMove();
  for(int i=0;i<=3;i++)
{
  data[i]=e[i];
  }
armMove();
  }

//机器臂前方放下
void armF()
{
for(int i=0;i<=3;i++)
{
  data[i]=d[i];
  }
armMove();
 for(int i=0;i<=3;i++)
{
  data[i]=a[i];
  }
armMove();
  }
//机器臂左方放下
void armL()
{
  for(int i=0;i<=3;i++)
{
  data[i]=f[i];
  }
armMove();
    for(int i=0;i<=3;i++)
{
  data[i]=g[i];
  }
armMove();
      for(int i=0;i<=3;i++)
{
  data[i]=h[i];
  }
armMove();
       for(int i=0;i<=3;i++)
{
  data[i]=j[i];
  }
armMove(); 
       for(int i=0;i<=3;i++)
{
  data[i]=a[i];
  }
armMove(); 
  }

void armR()
{
  for(int i=0;i<=3;i++)
{
  data[i]=k[i];
  }
armMove();
    for(int i=0;i<=3;i++)
{
  data[i]=l[i];
  }
armMove();
      for(int i=0;i<=3;i++)
{
  data[i]=m[i];
  }
armMove();
       for(int i=0;i<=3;i++)
{
  data[i]=n[i];
  }
armMove(); 
       for(int i=0;i<=3;i++)
{
  data[i]=a[i];
  }
armMove(); 
  }
/*==========================结束===========================*/

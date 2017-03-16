#include<stdio.h> 
#include<math.h> 

const limit_Nmax = 1000
const limit_a = 1000
const min_a = 100 

double circle(double x, double radius) 
{ 
double y=radius*radius-x*x; 
return y; 
}

int main() 
{ 
double x,y,Pi; 
int a=min_a
i=0;
double Ncirc=0;//to4ek popalo v krug
double Nmax=a; //vsego to4ek
while (a<limit_a)
{ 
Nmax=a; 
 while (Nmax<=limit_Nmax) 
 { 
 Ncirc=0; i=0; 
    while (i<Nmax) 
    { 
    x = (random() % (a);  
    y = (random() % (a);  
        if (y*y<=circle(x,(a/2))) 
        { 
        Ncirc++; 
        } 
    i++; 
    } 
Pi=(Ncirc/Nmax)*4; 
printf(Pi); 
 } 
} 
} 

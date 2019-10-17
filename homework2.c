/*
Name: Bishal Adhikari
ID: 1001664327
->Command:<-
  gcc homework2.c //It compiles the code
 ./a.out < lab2.dat // lab1.dat is a prototype, any data file could be used
                         to run the code as long as it follows the given format
->objective of the program<-
The following program takes two data tables which represents wash time and dry
time of a basket of laundry. It then uses Johnshons rule method for scheduling.
*/
#include <stdio.h>
#include <stdlib.h>
//struct that holds a index, wash time and dry time and lets us know if  it has been scheduled
struct basket{
  int index;
  int wash_time;
  int dry_time;
  int inserted;
};
//function to sort an array
void sort(int*number,int n){
  int i,j,a;
  for (i = 0; i < n; ++i)
  {
    for (j = i + 1; j < n; ++j)
    {
        if (number[i] > number[j])
        {
            a =  number[i];
            number[i] = number[j];
            number[j] = a;
        }
     }
    }
}
//function which schedules the laundry for the fastest completion time.
void schedule_finder(int*a,int*b,struct basket* schedule_basket,int size){
  int i=0,j=0,l=size-1,m=0,k;
  sort(a,size);
  sort(b,size);
  //dynamic memory allocation of struct
  struct basket *scheduled_basket=malloc(size*sizeof(struct basket));
  while(m<=l) {//loop runs until all the elements of wash time and dry time are compared.
    if(a[i]<=b[j]){
      for(k =0 ; k<size;k++){// itereates to find the perticular element in the original struct
        if(a[i]==schedule_basket[k].wash_time){
          if(!schedule_basket[k].inserted){// checking if the basked has already been scheduled
            scheduled_basket[m].index=schedule_basket[k].index;
            scheduled_basket[m].wash_time=schedule_basket[k].wash_time;
            scheduled_basket[m].dry_time=schedule_basket[k].dry_time;
            schedule_basket[k].inserted = 1;
            m++;//increasing the array location counter from the begining.
            break;// breaks the unnecessary loop iteration after finding the right element
          }
        }
      }
      i++;
    }else if(b[j]<a[i]){
      for(k =0 ; k<size;k++){// itereates to find the perticular element in the original struct
        if(b[j]==schedule_basket[k].dry_time){
          if(!schedule_basket[k].inserted){// checking if the basked has already been scheduled
            scheduled_basket[l].index=schedule_basket[k].index;
            scheduled_basket[l].wash_time=schedule_basket[k].wash_time;
            scheduled_basket[l].dry_time=schedule_basket[k].dry_time;
            schedule_basket[k].inserted = 1;
            l--;//decreasing the array location counter from the end.
            break;
          }
        }
      }
      j++;
    }
  }
  //finding the initial washer_time and dryer_time for the starting schedule
  int washer_time = 0;
  int dryer_time = washer_time + scheduled_basket[0].wash_time;
  printf("%d %d %d %d %d\n",scheduled_basket[0].index,scheduled_basket[0].wash_time,scheduled_basket[0].dry_time,washer_time,dryer_time);
  //holding the rest of the calculations of wash time and dry time.
  for ( i = 1; i < size; i++) {
    washer_time = washer_time + scheduled_basket[i-1].wash_time;
    dryer_time = dryer_time + scheduled_basket[i-1].dry_time;
    if(dryer_time<washer_time+scheduled_basket[i].wash_time){//figuring out if there is a lag time.
      int real_dryer_time = washer_time + scheduled_basket[i].wash_time;// finding the actual schedule time.
      printf("dryer gap from %d to %d\n",dryer_time,real_dryer_time);
      dryer_time = real_dryer_time;
    }
    printf("%d %d %d %d %d\n",scheduled_basket[i].index,scheduled_basket[i].wash_time,scheduled_basket[i].dry_time,washer_time,dryer_time);
  }
  //calculating the makespan time and printing.
  int makespan = dryer_time + scheduled_basket[size-1].dry_time;
  printf("makespan is: %d\n",makespan);
}
//main function
int main(int argc, char const *argv[]) {
  int size,i;

  scanf("%d",&size);
  //dynamic allocation of schedule_basket struct type.
  struct basket *schedule_basket=malloc(size*sizeof(struct basket));
  // dynamic allocation of a and b.
  int *a= malloc(size*sizeof(int));
  int *b= malloc(size*sizeof(int));
  //loop to take the input from the file.
  for( i = 0 ; i < size ; i++)
  {
    scanf("%d %d",&a[i],&b[i]);
    schedule_basket[i].index=i;
    schedule_basket[i].wash_time=a[i];
    schedule_basket[i].dry_time=b[i];
    schedule_basket[i].inserted=0;

  }
  //function to apply the Johnshons rule for scheduling
  schedule_finder(a,b,schedule_basket,size);
  return 0;// return 0 from the main function.
}

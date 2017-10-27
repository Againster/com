//written by lissa
//date 2015 07 15
//
//This code is multiple thread P() and V() in computer operation system.please visit http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html


//compile:cc -lpthread <this file name>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
pthread_mutex_t count_mutex          = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var_full   = PTHREAD_COND_INITIALIZER;
pthread_cond_t  condition_var_empty  = PTHREAD_COND_INITIALIZER;

void *functionCount1();
void *functionCount2();
int  count = 50;
#define COUNT_MAX 99 
#define COUNT_MIN 1 
#define COUNT_DONE 1000

main()
{
    pthread_t thread1, thread2;
	pthread_create( &thread1, NULL, &functionCount1, NULL);
	pthread_create( &thread2, NULL, &functionCount2, NULL);
	pthread_join( thread1, NULL);
	pthread_join( thread2, NULL);
	printf("Final count: %d\n",count);
	exit(EXIT_SUCCESS);
}

//producer
void *functionCount1()
{
    for(;;){
	    pthread_mutex_lock( &count_mutex );  
	    if(count>COUNT_MAX)
		    pthread_cond_wait( &condition_var_empty, &count_mutex );
		count++;
		printf("Producer: %d\n",count);
		pthread_cond_signal( &condition_var_full );
		pthread_mutex_unlock( &count_mutex );
		//random sleep time.
		struct timeval tpstart;
		gettimeofday(&tpstart,NULL);
		srand(tpstart.tv_usec);
		int sleep_time=1+(int) (1000000.0*rand()/(RAND_MAX+1.0));
		printf("Producer stop %d us...\n",sleep_time);
		usleep(sleep_time);
    }
}

//consumer
void *functionCount2()
{
    for(;;)
    {
	    pthread_mutex_lock( &count_mutex );
    	if(count<COUNT_MIN)
		  pthread_cond_wait( &condition_var_full, &count_mutex );
        count--;
        printf("Consumer: %d\n",count);
		pthread_cond_signal( &condition_var_empty );
        pthread_mutex_unlock( &count_mutex );
		//random sleep time.
		struct timeval tpstart;
		gettimeofday(&tpstart,NULL);
		srand(tpstart.tv_usec);
		int sleep_time=1+(int) (1000000.0*rand()/(RAND_MAX+1.0));
		printf("Consumer stop %d us...\n",sleep_time);
		usleep(sleep_time);
    }
}

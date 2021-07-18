#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

const int PROGRESS_BAR_LENGTH = 30;
const int NUM_OF_THREADS = 5;

#define ESC "\033"
#define CSI "["
#define PREVIOUSLINE "F"
#define BACKSPACE "D"


typedef struct{
	int count_to_val;
	int progress;
	pthread_t thethread;
} thread_info;


void update_bar(thread_info* tinfo){
	int num_chars = (tinfo->progress * 100 / tinfo->count_to_val) * PROGRESS_BAR_LENGTH / 100;
	printf("[");
	for(int i=0;i<num_chars;i++){
		printf("-");
	}
	if(tinfo->progress < tinfo->count_to_val){
		printf(ESC CSI BACKSPACE ">");
	}
	for(int i=0;i<PROGRESS_BAR_LENGTH-num_chars;i++){
		printf(" ");
	}
	printf("]\n");
}

void *mythreadfunc(void *arg){
	thread_info* tinfo = arg;

	for(tinfo->progress=0;tinfo->progress<tinfo->count_to_val;tinfo->progress++){
		usleep(1000);
	}
	return NULL;
}

int main(){
	thread_info threads[NUM_OF_THREADS];

	for(int i=0;i<NUM_OF_THREADS;i++){
		threads[i].count_to_val = rand()%10000;
		threads[i].progress = 0;
		pthread_create(&threads[i].thethread, NULL, mythreadfunc, &threads[i]);
	}

	bool done = false;

	while(!done){
		done = true;
		for(int i=0;i<NUM_OF_THREADS;i++){
			update_bar(&threads[i]);
			if(threads[i].progress < threads[i].count_to_val){
				done = false;
			}
		}
		if(!done){
			printf(ESC CSI "%d" PREVIOUSLINE, NUM_OF_THREADS);
		}
		usleep(10000);
	}
	printf("Done!\n");
}

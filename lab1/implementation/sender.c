/**
 * @brief:  send random points to a POSIX queue. Type 'q' to terminate. 
 * @author: Anthony Ng (achng), Behroz Saadat (bmsaadat) 
 * @date:   2012/10/09
 * NOTES: 
 */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/time.h>
#include "common.h"
#include "point.h"

//Current number of items in message queue
int queue_count = 0;
int queue_size = 0;

//Number of integers to send from producer to consumer
int number_of_integers = 0;

//Number of integers consumed by the consumer
int consumed_count = 0;

//State of the consumer
bool consumer_running = true;

//Time before fork;
struct timeval time_before_fork;

//Time before first item is produced;
struct timeval time_before_first_item;

//Time after all items have been consumed
struct timeval time_after_all_consumed;

void sig_handler(int sig){
	consumer_running = false;
	exit(1);
}

/**
 * Calculates the time difference between two time instances
 */
double calculate_elapsed_time(struct timeval *t1, struct timeval *t2)
{
	//Elapsed time between the two time instances
	double elapsed_time = 0;

	//Obtain seconds
	double seconds = t2->tv_sec - t1->tv_sec;

	//Obtain microseconds
	double microseconds = t2->tv_usec - t1->tv_usec;

	elapsed_time = seconds + microseconds*(1e-6);

	return elapsed_time;
}

int spawn_consumer()
{
	pid_t child_pid;
	struct timespec ts = {time(0) + 5, 0};
	
	child_pid = fork();
	
	if (child_pid != 0)
		return child_pid;
	else
	{
		mqd_t qdes;
		char  qname[] = "/mailbox_achng2";
		mode_t mode = S_IRUSR | S_IWUSR;
		struct mq_attr attr;

		attr.mq_maxmsg  = QUEUE_SIZE;
		attr.mq_msgsize = sizeof(int);
		attr.mq_flags   = 0;	// blocking queue 

		qdes  = mq_open(qname, O_RDONLY, mode, &attr);
		if (qdes == -1 ) 
		{
			perror("mq_open()");
		}
		else
		{
			srand(time(0));
			
			while(consumer_running)
			{
				//Integer received in the mailbox from producer
				int integer_received;
				struct timespec ts = {time(0) + 5, 0};
				if(!consumer_running) break;
				if (mq_timedreceive(qdes, (char *) &integer_received,
					sizeof(int), 0, &ts) == -1) {
					perror("mq_timedreceive() failed");
					printf("Type Ctrl-C and wait for 5 seconds to terminate. \n");
				} 
				else {
					printf("%d is consumed \n", integer_received);

					//Increment the number of integers consumed
					consumed_count++;
					
					if(consumed_count == number_of_integers)
					{
						//Obtain time after all integers have been consumed
						gettimeofday(&time_after_all_consumed, NULL);

						//Print the initialization time
						printf("Time to initialize system: %f \n", 
							calculate_elapsed_time(&time_before_fork, &time_before_first_item));

						//Print the time needed to transmit data from producer to consumer
						printf("Time to transmit data: %f \n", 
							calculate_elapsed_time(&time_before_first_item, &time_after_all_consumed));
					}
				}

			}
		}
		exit(1);
	}
}	

int main(int argc, char *argv[])
{	
	//Set number of integers to produce to argument 1
	number_of_integers = atoi(argv[1]);

	//Set queue_size to argument 2
	queue_size = atoi(argv[2]);

	mqd_t qdes;
	char  quit = '\0';
	char  qname[] = "/mailbox_achng2";	//queue name must start with '/'. man mq_overview
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = queue_size;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		// blocking queue 

	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}
	
	signal(SIGINT, sig_handler);

	srand(time(0));

	printf("Press a key to send a random point.\n");
	getchar();

	//Obtain time before fork
	gettimeofday(&time_before_fork, NULL);

	//Generate the specified number of random integers and send to consumer
	int i = 0;
	for(i = 0; i < number_of_integers; i++)
	{	
		//Randomly generated number to send to consumer
		int random_number = rand() % 80;

		if (mq_send(qdes, (char *)&random_number, sizeof(int), 0) == - 1) {
			perror("mq_send() failed");
		}
		else
		{
			//Spawn the consumer once there is atleast one item in the queue
			if(i == 0)
			{
				//Obtain time before first item was produced
				gettimeofday(&time_before_first_item, NULL);

				spawn_consumer();
			}

			queue_count++;
		}

		//printf("Sending a random integer: %d \n", random_number);
		//quit = getchar();
	}

	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
	}

	while(consumer_running){}
	
	return 0;
}

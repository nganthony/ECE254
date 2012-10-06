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
#include <time.h>
#include "common.h"
#include "point.h"

//Current number of items in message queue
int queue_count = 0;
int queue_size = 0;

//State of the consumer
bool consumer_running = true;

void sig_handler(int sig){
	consumer_running = false;
	exit(1);
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
				int integer_received;
				struct timespec ts = {time(0) + 5, 0};
				if(!consumer_running) break;
				if (mq_timedreceive(qdes, (char *) &integer_received,
					sizeof(int), 0, &ts) == -1) {
					perror("mq_timedreceive() failed");
					printf("Type Ctrl-C and wait for 5 seconds to terminate. \n");
				} else {
					printf("Received a random integer: %d \n", integer_received);
				}

			}
		}
		exit(1);
	}
}	

int main(int argc, char *argv[])
{
	//printf("ARGC: %d", argc);

	//printf("FIRST ARGUMENT: %d", arg1);
	//printf("SECOND ARG: %d", arg2);
	
	if(argc == 0){
		printf("FATAL ERROR NO ARGS DETECTED.. EXITING");
		exit(1);
	}
	
	int arg1 = atoi(argv[1]);
	int arg2 = atoi(argv[2]);


	//Number of integers to send to consumer
	int number_of_integers = arg1;
	queue_size = arg2;

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
			if(i == 1)
				spawn_consumer();

			queue_count++;
		}

		printf("Sending a random integer: %d \n", random_number);
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

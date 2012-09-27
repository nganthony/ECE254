/**
 * @file:   os/lab1/mqueue/sender.c
 * @brief:  send random points to a POSIX queue. Type 'q' to terminate. 
 * @author: ece254/mte241 lab staff 
 * @date:   2012/09/20
 * NOTES: 
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <time.h>
#include "common.h"
#include "point.h"

int main(void)
{
	//Number of integers to send to consumer
	int number_of_integers = 10;

	mqd_t qdes;
	char  quit = '\0';
	char  qname[] = "/mailbox_achng1";	//queue name must start with '/'. man mq_overview
	mode_t mode = S_IRUSR | S_IWUSR;
	struct mq_attr attr;

	attr.mq_maxmsg  = QUEUE_SIZE;
	attr.mq_msgsize = sizeof(int);
	attr.mq_flags   = 0;		// blocking queue 

	qdes  = mq_open(qname, O_RDWR | O_CREAT, mode, &attr);
	if (qdes == -1 ) {
		perror("mq_open() failed");
		exit(1);
	}

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

		printf("Sending a random integer: %d", random_number);
		quit = getchar();
	}

	if (mq_close(qdes) == -1) {
		perror("mq_close() failed");
		exit(2);
	}

	if (mq_unlink(qname) != 0) {
		perror("mq_unlink() failed");
		exit(3);
	}

	return 0;
}

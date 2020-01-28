#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
//Name: Charles Pelton
//Lab: Lab06
//Date: 10/07/2018
#define DELAY ( (rand() % 5 + 1) * 1000)

void *philosopher(void *id);

pthread_mutex_t *chopstick;

int numOfSeats, numOfTurns;

int *state;
pthread_cond_t *cond;

pthread_mutex_t monitor_mutex = PTHREAD_MUTEX_INITIALIZER;
//psudocode implimentation 
enum {THINKING, HUNGRY, EATING};

void pickup (int i, int time)
{
      pthread_mutex_lock(&monitor_mutex); // critical area below, it can kill people if not safe
      printf("Philsopher no. %d gets hungry for the %d time!\n", i, time);
      state[i] = HUNGRY; 
      printf("Philsopher no. %d tries to grab chopstick %d\n", i, i);
      printf("Philsopher no. %d tries to grab chopsticK %d\n", i, (i + 1) % numOfSeats);
      test(i); 
     if (state[i] != EATING)
     pthread_cond_wait(&cond[i],&chopstick[i]); // set a condition to wait a philosopher
	pthread_mutex_unlock(&monitor_mutex); // allow the next hungry one to come in
} 

void putdown (int i)
{
state[i] = THINKING; 
// test left and right neighbors 
test((i + (numOfSeats-1)) % numOfSeats); 
test((i + 1) % numOfSeats); 
printf("Philsopher no. %d has returned chopstick %d\n", i, i);
printf("Philsopher no. %d has returned chopstick %d\n", i, (i + 1) % numOfSeats);
} 

void test (int i)
{   
  if ( (state[(i + (numOfSeats-1)) % numOfSeats] != EATING) &&
     (state[i] == HUNGRY) && (state[(i + 1) % numOfSeats] != EATING) ) // if its free, NOM!
{ 
         printf("Philsopher no. %d has grabbed chopstick %d\n", i, i);
         printf("Philsopher no. %d grabbed chopstick %d\n", i, (i + 1) % numOfSeats);
	 printf("Philsopher no. %d eating\n", i);
         state[i] = EATING;
	 usleep(DELAY * 10); // random eating time
	 printf("Philsopher no. %d stopped eating\n", i);
	 pthread_cond_signal(&cond[i]); // signal the philosopher to continue
   } 
} 

initialization_code() // initalize
{ 
 for (int i = 0; i < numOfSeats; i++) 
     state[i] = THINKING; 
} 
// previous code
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: philosophers <number of seats> <number of turns>");
        return 1;
    }

    numOfSeats = strtod(argv[1], NULL);
    numOfTurns = strtod(argv[2], NULL);
    state = malloc(numOfSeats * sizeof(int)); // new variable slots of thonk, hangry and ate for each thonker
    cond = calloc(numOfSeats, sizeof(pthread_cond_t)); // set of wait condition variables, calloc set 0
    chopstick = calloc(numOfSeats, sizeof(pthread_mutex_t)); // for pthreads
    initialization_code(); //sets philosophers, because I didn't calloc to 0 enum think
    srand(time(NULL));  // set the see for random number generator

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    printf("Start a dinner for %d diners\n", numOfSeats);

    pthread_t philosopher_tid[numOfSeats];

    long i;
    
    for (i = 0; i < numOfSeats; i++)
        pthread_mutex_init(chopstick + i, NULL);

    for (i = 0; i < numOfSeats; i++)
        pthread_create(&philosopher_tid[i], NULL, philosopher, (void *) i);

    for (i = 0; i < numOfSeats; i++)
        pthread_join(philosopher_tid[i], NULL);

    for (i = 0; i < numOfSeats; i++)
        pthread_mutex_destroy(chopstick + i);

    printf("Dinner is no more.\n");

    return 0;
}

void *philosopher(void *num)
{
    int id = (long) num;
    usleep(DELAY*4);//philosophers arrive at the table at various times
    printf("Philsopher no. %d sits at the table.\n", id);
    usleep(DELAY*4);//philosophers arrive at the table at various times

    int i;
    for (i = 0; i < numOfTurns; i++)
    {
	pickup(id, i);
        // YEEEAAAAH !!!
 	usleep(DELAY * 10); // random eating time
	putdown(id);
        printf("Philsopher no. %d finished turn %d\n", id, i + 1);
    }

    printf(">>>>>> Philsopher no. %d finished meal. <<<<<<\n", id);

    pthread_exit(NULL);
}


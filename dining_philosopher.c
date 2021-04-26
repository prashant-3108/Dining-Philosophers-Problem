#include <stdio.h>     /* Input/Output */
#include <stdlib.h>    /* General Utilities */
#include <unistd.h>    /* Symbolic Constants */
#include <sys/types.h> /* Primitive System Data Types */
#include <sys/wait.h>  /* Wait for Process Termination */
#include <errno.h>     /* Errors */
#include <string.h>    /* Strings */
#include <pthread.h>   /* pthreads library */

int num_phil;
int freq;
pthread_t *philosophers = NULL;
pthread_cond_t *phil_cond = NULL;
pthread_mutex_t ensure_mutex;
int *pstate = NULL;
int *times_eaten = NULL;

/* 
    State 0 - Thinking 
    State 1 - Hungry 
    State 2 - Eating
*/

void Can_Eat(int i)
{
    /* If Left and Right Neighbours are not Eating and current philosopher is Hungry then Eat */
    if ((pstate[(i + 1) % num_phil] != 2) && (pstate[(i + num_phil - 1) % num_phil] != 2) && (pstate[i] == 1))
    {
        pstate[i] = 2;
        /* Philosophers Condtional Variable has been signal to think again */
        pthread_cond_signal(&phil_cond[i]);
    }
}

/* Program such that Chopsticks are picked up from the philosopher to eat */
void Pickup_Chopsticks(int i)
{
    /* Lockin Mutex to ensure exclusive access of shared resources like state array and condition variable array */
    pthread_mutex_lock(&ensure_mutex);

    /* Hungry State Achieved */
    pstate[i] = 1;
    Can_Eat(i);

    while (pstate[i] != 2)
    {
        /* If neighbours are busy eating and chopsticks are not free  then wait until signal to eat */
        pthread_cond_wait(&phil_cond[i], &ensure_mutex);
    }

    /* UnLocking Mutex to ensure allowing shared resources to be used by other threads */
    pthread_mutex_unlock(&ensure_mutex);
}

void Putdown_Chopsticks(int i)
{
    /* Locking Mutex to ensure exclusive access of shared resources like state array and condition variable array */
    pthread_mutex_lock(&ensure_mutex);

    pstate[i] = 0; /* Goes to Thinking */

    Can_Eat((i + 1) % num_phil);                /* Checking if left Neighbour is in wait then it could eat */
    Can_Eat((i + num_phil - 1) % num_phil);     /* Checking if right Neighbour is in wait then it could eat */

    /* Unlocking Mutex to ensure allowing shared resources to be used by other threads */
    pthread_mutex_unlock(&ensure_mutex);
}

void *dining(int philo)
{
    /* When Philosopher has not eaten enough times Enter Loop*/
    while (times_eaten[philo] < freq)
    {
        sleep(1); /* State changing time for additional safety*/

        Pickup_Chopsticks(philo);
        printf("Philosopher %d eating\n", (philo + 1));
        times_eaten[philo]++;
        sleep(3);   /* Eating Time */

        Putdown_Chopsticks(philo);
        printf("Philosopher %d thinking\n", (philo + 1));
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Too Few Arguments.\n");
        return 0;
    }

    if (argv[1] != NULL)
    {
        num_phil = atoi(argv[1]);
    }
    if (argv[2] != NULL)
    {
        freq = atoi(argv[2]);
    }

    long i = 0;
    int status = 0;
    void *message;
    pthread_attr_t attr;

    /* Initializing the variables' arrays */
    philosophers = (pthread_t *)calloc((num_phil), sizeof(pthread_t));
    phil_cond = (pthread_cond_t *)calloc((num_phil), sizeof(pthread_cond_t));
    times_eaten = (int *)calloc((num_phil), sizeof(int));
    pstate = (int *)calloc((num_phil), sizeof(int));
    pthread_mutex_init(&ensure_mutex, NULL);


    /* Intializing Condition Vriable array */
    for (i = 0; i < num_phil; i++)
    {
        status = pthread_cond_init(&phil_cond[i], NULL);
        if (status == -1)
        {
            free(philosophers);
            free(times_eaten);
            free(pstate);
            free(phil_cond);
            printf("\nCondition Variable's initialization failed\n");
            exit(1);
        }
    }

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /*  Initially all Philosophers are thinking  */
    int j = 0;
    for (j = 0; j < num_phil; j++)
    {
        printf("Philosopher %d thinking\n", (j + 1));
    }

    for (i = 0; i < num_phil; i++)
    {
        status = pthread_create(&philosophers[i], &attr, (void *)dining, (void *)(i));
        if (status)
        {
            free(philosophers);
            free(times_eaten);
            free(pstate);
            free(phil_cond);
            printf("ERROR: Return code from pthread_create() is %d\n", status);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    for (i = 0; i < num_phil; i++)
    {
        status = pthread_join(philosophers[i], &message);
        if (status)
        {
            free(philosophers);
            free(times_eaten);
            free(pstate);
            free(phil_cond);
            printf("ERROR: Return code from pthread_join() is %d\n", status);
            exit(-1);
        }
    }

    for (i = 0; i < num_phil; i++)
    {
        pthread_cond_destroy(&phil_cond[i]);
    }

    free(philosophers);
    free(times_eaten);
    free(pstate);
    free(phil_cond);

    pthread_exit(NULL);
    // return 0;
}
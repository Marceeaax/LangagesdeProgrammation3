#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <limits.h>

#include <signal.h>

int MAX_INFECTIONS;
int DELAY;
int AGENTS_AMOUNT;
int infection_count = 0;
int time_count = 0;
pthread_mutex_t infection_info_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t *threads_id;
int *time_infections_per_agent;
int max_time = 0;

void calculate_time_infections()
{
  int new_max_time = INT_MIN;
  for (int i = 0; i < AGENTS_AMOUNT; i++)
  {
    int time_actual = time_infections_per_agent[i];
    if (time_actual > new_max_time)
      new_max_time = time_actual;
  }
  time_count = new_max_time;
}

void kill_all_threads()
{

  for (int i = 0; i < AGENTS_AMOUNT; i++)
  {
    // We cancel all threads except to the current thread.
    if(pthread_self() != threads_id[i]){
      pthread_cancel(threads_id[i]);
    }
  }
  // Call the exit function so the main function can continue
  pthread_exit(NULL);
}

void *infect(void *params)
{
  int agent_number = *((int *)params);
  printf("Agente #%d inicializado \n", agent_number);
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  while (1)
  {

    int timeToWait = 1 + (rand() % DELAY);
    sleep(timeToWait);

    pthread_mutex_lock(&infection_info_mutex);

    time_infections_per_agent[agent_number - 1] += timeToWait;
    infection_count++;
    printf("Agente #%d infecto a una persona luego de %d seg. (total contagios: %d)\n", agent_number, timeToWait, infection_count);
    
    if (infection_count == MAX_INFECTIONS)
    {
      kill_all_threads();    
    }

    
    pthread_mutex_unlock(&infection_info_mutex);
  }
}

int main(int argc, char *const argv[])
{
  srand(time(NULL));

  AGENTS_AMOUNT = atoi(argv[1]);
  DELAY = atoi(argv[2]);
  MAX_INFECTIONS = atoi(argv[3]);

  threads_id = (pthread_t *)malloc(AGENTS_AMOUNT * sizeof(pthread_t));
  int *agents_number = (int *)malloc(AGENTS_AMOUNT * sizeof(int));
  time_infections_per_agent = (int *)calloc(AGENTS_AMOUNT, sizeof(int));

  time_t start = time(NULL);

  // Create all the threads
  for (int i = 0; i < AGENTS_AMOUNT; i++)
  {
    agents_number[i] = i + 1;
    pthread_create((threads_id + i), NULL, &infect, &agents_number[i]);
  }

  // Join all the threads
  for (int i = 0; i < AGENTS_AMOUNT; i++)
  {
    pthread_join(threads_id[i], NULL);
  }
  // After achieve the max amount of infections, we need to calculate the exactly time
  calculate_time_infections();

  printf("Total de %d contagios alcanzados en %d segundos.\n", infection_count, time_count);
  
  time_t end = time(NULL); 
  printf("Calculated time: %lds \n", (end - start));
  return 0;
}

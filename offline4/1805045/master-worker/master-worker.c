#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include <assert.h>
void print_produced(int num, int master) {

  printf("Produced %d by master %d\n", num, master);
}

void print_consumed(int num, int worker) {

  printf("Consumed %d by worker %d\n", num, worker);
  
}
void debug(int i,int val,int val2,int thread_id)
{
  if(thread_id==0)
    printf("%d %d %d %d\n",thread_id,i,val,val2);
}

pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty=PTHREAD_COND_INITIALIZER;
pthread_cond_t fill=PTHREAD_COND_INITIALIZER;
int item_to_produce, curr_buf_size;
int total_items, max_buf_size, num_workers, num_masters;
int *buffer;
//consumer thread
void * worker_consumer(void* data)
{
  int thread_id = *((int *)data);
  while(1){
    while(pthread_mutex_lock(&lock)){}
    if(item_to_produce>=total_items&&curr_buf_size==0)
    {
      assert(!pthread_cond_broadcast(&empty));
      assert(!pthread_mutex_unlock(&lock));
      break;
    }
    while(!curr_buf_size){
      while(pthread_cond_wait(&fill,&lock)){}
      if(item_to_produce>=total_items)
        break;
    }
    if(item_to_produce>=total_items&&curr_buf_size==0)
    {
      assert(!pthread_cond_broadcast(&empty));
      assert(!pthread_mutex_unlock(&lock));
      break;
    }
    --curr_buf_size;
    print_consumed(buffer[curr_buf_size],thread_id);
    if(!curr_buf_size)
    {
      int res=pthread_cond_signal(&empty);
      if(res)
      {
        printf("Worker thread %d: Something went wrong,cannot signal\n",thread_id);
        exit(1);
      }
    }
    int res=pthread_mutex_unlock(&lock);
    if(res)
    {
      printf("Worker thread %d: Something went wrong,cannot unlock mutex lock\n",thread_id);
      exit(1);
    }
  }
}
//produce items and place in buffer
//modify code below to synchronize correctly
void *generate_requests_loop(void *data)
{
  int thread_id = *((int *)data);

  while(1)
  {
    while(pthread_mutex_lock(&lock)){}
    if(item_to_produce >= total_items) {
      assert(!pthread_cond_broadcast(&fill));
      assert(!pthread_mutex_unlock(&lock));
	    break;
    }
    while(curr_buf_size==max_buf_size)
    {
      while(pthread_cond_wait(&empty,&lock)){}
      if(item_to_produce>=total_items)
        break;
    }
    if(item_to_produce >= total_items) {
      assert(!pthread_cond_broadcast(&fill));
      assert(!pthread_mutex_unlock(&lock));
	    break;
    }
    buffer[curr_buf_size++] = item_to_produce;
    print_produced(item_to_produce, thread_id);
    item_to_produce++;
    int res=pthread_cond_signal(&fill);
    if(res)
    {
      printf("Master thread %d: Something went wrong,cannot signal\n",thread_id);
      exit(1);
    }
    res=pthread_mutex_unlock(&lock);
    if(res)
    {
      printf("Master thread %d: Something went wrong,cannot unlock mutex lock\n",thread_id);
      exit(1);
    }
  }
  return 0;
}

//write function to be run by worker threads
//ensure that the workers call the function print_consumed when they consume an item

int main(int argc, char *argv[])
{
  int *master_thread_id;
  pthread_t *master_thread;
  item_to_produce = 0;
  curr_buf_size = 0;
  
  int i;
  
   if (argc < 5) {
    printf("./master-worker #total_items #max_buf_size #num_workers #masters e.g. ./exe 10000 1000 4 3\n");
    exit(1);
  }
  else {
    num_masters = atoi(argv[4]);
    num_workers = atoi(argv[3]);
    total_items = atoi(argv[1]);
    max_buf_size = atoi(argv[2]);
  }
    

   buffer = (int *)malloc (sizeof(int) * max_buf_size);


   //create master producer threads
   master_thread_id = (int *)malloc(sizeof(int) * num_masters);
   master_thread = (pthread_t *)malloc(sizeof(pthread_t) * num_masters);
  for (i = 0; i < num_masters; i++){
    master_thread_id[i] = i;
  }

  for (i = 0; i < num_masters; i++){
    pthread_create(&master_thread[i], NULL, 
    generate_requests_loop, (void *)&master_thread_id[i]);
  }
  
  //create worker consumer threads
  int *worker_thread_id=(int*)malloc(sizeof(int)*num_workers);
  pthread_t* worker_thread=(pthread_t*)malloc(sizeof(pthread_t)*num_workers);
  for(int i=0;i<num_workers;++i){
    worker_thread_id[i]=i;
  }
  for(int i=0;i<num_workers;++i)
  {
    
    pthread_create(&worker_thread[i],NULL,
    worker_consumer,(void*)&worker_thread_id[i]);
  }


  
  //wait for all threads to complete
  for (i = 0; i < num_masters; i++)
  {
    pthread_join(master_thread[i], NULL);
    printf("master %d joined\n", i);
  }
  for(int i=0;i<num_workers;++i)
  {
    pthread_join(worker_thread[i],NULL);
    printf("worker %d joined\n",i);
  }
  printf("end of main\n");
  /*----Deallocating Buffers---------------------*/
  free(buffer);
  free(master_thread_id);
  free(master_thread);
  
  return 0;
}

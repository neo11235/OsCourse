#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <assert.h>
#include "zemaphore.h"
void zem_init(zem_t *s, int value) {
  s->value=value;
  assert(!pthread_cond_init(&s->cond,NULL));
  assert(!pthread_mutex_init(&s->lock,NULL));
}

void zem_down(zem_t *s) {
    assert(!pthread_mutex_lock(&s->lock));
    while(s->value<=0)
        assert(!pthread_cond_wait(&s->cond,&s->lock));
    s->value--;
    assert(!pthread_mutex_unlock(&s->lock));
}

void zem_up(zem_t *s) {
    assert(!pthread_mutex_lock(&s->lock));
    s->value++;
    assert(!pthread_cond_signal(&s->cond));
    assert(!pthread_mutex_unlock(&s->lock));
}

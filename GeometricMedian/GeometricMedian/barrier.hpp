//
//  barrier.hpp
//  GeometricMedianADMM
//
//  Created by Maziar Sanjabi on 11/7/17.
//  Borrowed the code from here: http://blog.albertarmea.com/post/47089939939/using-pthreadbarrier-on-mac-os-x as the pthread barrier is not implemented on OSX
//
//

#ifndef barrier_hpp
#define barrier_hpp


#ifdef __APPLE__

#ifndef PTHREAD_BARRIER_H_
#define PTHREAD_BARRIER_H_

#include <pthread.h>
#include <errno.h>

typedef int pthread_barrierattr_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int tripCount;
} pthread_barrier_t;


int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count);
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_wait(pthread_barrier_t *barrier);
#endif //PTHREAD_BARRIER_H_

#endif //__APPLE__

#endif /* barrier_hpp */

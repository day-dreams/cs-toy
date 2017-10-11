#ifndef LOCKFREEQUEUE
#define LOCKFREEQUEUE

#include <mutex>
#include <queue>
#include <semaphore.h>
using std::queue;
using std::mutex;

/*
    无锁队列,简单封装std::queue

*/
template <typename T> class LockFreeQueue {
private:
  queue<T> _queue;
  mutex lock;
  sem_t client_arrived;

public:
  LockFreeQueue() { sem_init(&client_arrived, 0, 0); }

  /*
      push 向队列中加入一个元素
  */
  void push(const T &ele) {

    lock.lock();
    _queue.push(ele);
    lock.unlock();

    sem_post(&client_arrived);
  }

  /*
    pop 从队列中取出一个元素,如果队列为空则陷入等待
  */
  T pop() {
    sem_wait(&client_arrived);

    lock.lock();
    auto result = _queue.front();
    _queue.pop();
    lock.unlock();

    return result;
  }

  /*
    size 返回队列元素个数
  */
  size_t size() {
    lock.lock();
    auto size = _queue.size();
    lock.unlock();
    return size;
  }
};

#endif

/* A class to handle multi-threading
 * Created by Tong Tin Yi (2023)
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>
#include <thread>

/**
 * @brief A class to handle multi-threading
 */
class ThreadPool {
public:
    ThreadPool();
    virtual ~ThreadPool();
    void Start();
    void QueueJob(const std::function<void()>& job);
    void Stop();
    bool busy();

private:
    void ThreadLoop();

    bool should_terminate = false;           // Tells threads to stop looking for
    std::mutex queue_mutex;                  // Prevents data races to the job queue
    std::condition_variable mutex_condition; // Allows threads to wait on new jobs or termination
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> jobs;
};

#endif // THREADPOOL_H

#include <sys/epoll.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
class TimerNode {
public:
    friend class Timer;
    TimerNode(uint64_t timeout, std::function<void()> cb) : timeout_(timeout), cb_(std::move(cb)) {}

private:
    uint64_t timeout_;
    std::function<void()> cb_;
};

class Timer {
public:
    using TimerNodeUP = std::unique_ptr<TimerNode>;
    using Handle = std::multimap<uint64_t, TimerNodeUP>::iterator;

    static uint64_t GetCurrentTimeMs() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }

    Timer::Handle AddTimer(uint64_t diff, std::function<void()> cb) {
        const uint64_t timeout = GetCurrentTimeMs() + diff;
        auto pos = timer_map_.lower_bound(timeout);  // 第一个 >= timeout 的位置
        auto node = std::make_unique<TimerNode>(timeout, std::move(cb));
        return timer_map_.emplace_hint(pos, timeout, node);
    }

    void DelTimer(Handle handle) {
        if (handle != timer_map_.end()) {
            timer_map_.erase(handle);
        }
    }
    int WaitTime() {
        if (timer_map_.empty()) {
            return -1;
        }
        uint64_t diff = timer_map_.begin()->first - GetCurrentTimeMs();
        return diff > 0 ? static_cast<int>(diff) : 0;
    }
    void HandleTimeout() {
        auto iter = timer_map_.begin();
        while (iter != timer_map_.end() && iter->first <= GetCurrentTimeMs()) {
            iter->second->cb_();
            iter = timer_map_.erase(iter);
        }
    }

private:
    std::multimap<uint64_t, std::unique_ptr<TimerNode>> timer_map_;
};

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "epoll_create1 failed" << std::endl;
        return -1;
    }
    Timer timer;
    int i = 0;
    timer.AddTimer(1000, [&]() { std::cout << "Timer 1 second :" << i++ << std::endl; });
    timer.AddTimer(2000, [&]() { std::cout << "Timer 2 second :" << i++ << std::endl; });
    auto node = timer.AddTimer(3000, [&]() { std::cout << "Timer 3 second :" << i++ << std::endl; });
    timer.DelTimer(node);  // 删除 3 秒定时器

    epoll_event events[512];

    while (true) {
        int wait_time = timer.WaitTime();
        int n = epoll_wait(epoll_fd, events, 512, wait_time);
        if (n == -1) {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }
        timer.HandleTimeout();
    }
    close(epoll_fd);
    return 0;
}
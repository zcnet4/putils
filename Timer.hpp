#pragma once

#include <chrono>
#include <utility>

namespace putils
{
    //
    // Timer class
    //
    class Timer
    {
    public:
        using t_duration = std::chrono::system_clock::duration;
        using t_clock = std::chrono::system_clock;

        // Constructors
    public:
        Timer() : _duration(0), _start(t_clock::now()) {}

        template<typename Duration>
        Timer(Duration &&duration) noexcept
                : _duration(std::forward<Duration>(duration)), _start(t_clock::now()) {}

        // Coplien
    public:
        Timer(const Timer &other) noexcept = default;

        Timer &operator=(const Timer &other) noexcept = default;

        // Move
    public:
        Timer(Timer &&other) noexcept = default;

        Timer &operator=(Timer &&other) noexcept = default;

        // Is the timer ringing
    public:
        bool isDone() const noexcept { return t_clock::now() - _start >= _duration; }
        std::size_t getTimesDone() const noexcept { return (t_clock::now() - _start) / _duration; }

        // How long ago did I start the timer
    public:
        auto getTimeSinceStart() const noexcept { return t_clock::now() - _start; }
        auto getTimeSinceDone() const noexcept
        {
            const auto done = t_clock::now() - _start;

            if (_duration == _duration.zero())
                return done - std::size_t(0) * _duration;

            return done - (getTimesDone() * _duration);
        }

        // How long until it rings
    public:
        auto getTimeLeft() const noexcept { return _start + _duration - t_clock::now(); }

        // Restart the timer
    public:
        void restart() noexcept { _start = t_clock::now(); }

        void setStart(t_clock::time_point start) { _start = start; }

        // Duration getters and setters
    public:
        const auto &getDuration() const noexcept { return _duration; }

        template<typename Duration>
        void setDuration(Duration &&duration) noexcept { _duration = duration; }

        // Attributes
    private:
        t_duration _duration;
        t_clock::time_point _start;
    };
}

# [Timer](Timer.hpp)

Simple timer class based on std::chrono.

### Members

##### Constructor

```
Timer(Duration &&duration);
```

Sets the time before the timer "beeps".

##### isDone

```
bool isDone() const;
```

Is the timer ringing?

##### getTimeSinceStart

```
t_duration getTimeSinceStart() const;
```

How long ago did I start the timer?

##### getTimeLeft

```
t_duration getTimeLeft() const;
```

How long until it rings?

##### restart

```
void restart();
```

##### Duration getters and setters

```
const t_duration &getDuration() const noexcept { return _duration; }

template<typename Duration>
void setDuration(Duration &&duration) noexcept { _duration = duration; }

void setDuration(double duration) noexcept { _duration = t_duration(duration); }
```

##### Helper types

```
using t_duration = std::chrono::duration<double>;
using t_clock = std::chrono::system_clock;
```

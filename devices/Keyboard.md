# [Keyboard](Keyboard.hpp)

An abstraction to a keyboard. Each key is an observable object that will notify its observers when it changes state (pressed or unpressed).
A public `Key` enum is available, which contains a set of default keys (feel free to add whatever other keys you need).

### Members

##### addObserver

```cpp
void addObserver(Key key, std::function<void()> &&observer);
```

Adds an observer to a specific key.

##### isKeyPressed

```cpp
bool isKeyPressed(Key key) const;
```

Lets observers know what new state a key is in.

##### setKeyPressed

```cpp
void setKeyPressed(Key key, bool pressed);
```

Change a key's state and notify its observers.

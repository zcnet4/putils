# Mediator

Manages various [Modules](Module.md) and lets them send each other `DataPackets`.

Also lets `Modules` run asynchronous tasks.

### Members

##### addModule

```
void addModule(BaseModule *m);
```

Adds a new module to be managed. Note that the `Mediator` is not responsible for the `Module`'s lifetime.

##### removeModule

```
void removeModule(BaseModule *m);
```

Removes a module.

##### send

```
void send(T &&data);
```

Sends `data` to all `Modules` "subscribed" to `T` (see [Module](Module.md)).

##### runTask

```
void runTask(const std::function<void()> &f);
```

Runs `f` asyncrhonously.

##### completeTasks

```
void completeTasks();
```

Wait for all current tasks to finish.

##### running

```
std::atomic<bool> running { true };
```

Gives `Modules` a common value to check to make sure the program is still running.
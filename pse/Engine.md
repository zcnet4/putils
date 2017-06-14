# Engine

Manages and renders `ViewItems`.

### Members

##### Constructor

```
Engine(size_t screenWidth, size_t screenHeight,
    const std::string &screenName,
    double refreshRate = 120);
```

Creates a new window with specified properties.

##### addItem

```
void addItem(ViewItem *item, size_t height = 0);
```

Adds a new `ViewItem` to manage, at the specified `height` (determines whether it should be drawn over others).

##### removeItem

```
void removeItem(ViewItem *item);
```

Removes a ViewItem

##### update

```
void update(bool clear = true);
```

Updates the window, updating all `ViewItems`' positions according to their movement.

##### pollEvent

```
bool pollEvent(sf::Event &e);
```

Lets the user poll events straight from the SFML window.

##### displayColor

```
void displayColor(const sf::Color &color);
```

Splashes a color on the window.

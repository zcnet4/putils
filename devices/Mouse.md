# Mouse

An abstraction to a mouse. Each button is an observable object that will notify its observers when it changes state (pressed or unpressed).

The last click coordinates can then be recovered using `getCoordinates()`.

A public `Button` enum is available, which contains a set of default buttons (feel free to add whatever other buttons you need).

### Members

##### leftButton, rightButton
```
putils::Observable<> leftButton;
putils::Observable<> rightButton;
```
Will notify their observers when clicked.

##### addObserver
```
void addObserver(Button button, F &&observer);
```
Add an observer to a specific button.

##### isKeyPressed
```
bool isKeyPressed(Button button) const;
```
Lets observers know what state a button is in`

##### setKeyPressed
```
void setKeyPressed(Button button, bool pressed);
```
Change a button's state and notify its observers.

##### leftClick, rightClick
```
void leftClick(Pair &&p);
void rightClick(Pair &&p);
```
Indicate the mouse has been clicked at the given coordinates, and notify observers.

##### getCoordinates
```
const std::pair<int, int> &getCoordinates() const;
```
Lets observers get the last click coordinates.

##### setScreenSize
```
void setScreenSize(Pair &&screenSize);
```
Indicate the screen size (so that users can find out the mouse's position relative to its coordinates).

##### getScreenSize
```
const std::pair<int, int> &getScreenSize() const;
```
Lets observers know the size of the screen.

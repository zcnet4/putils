# QuadTree<Contained, Precision, MaxChildren>

Space partitioner optimized for searching for an object's closest neighbors.

### Members

##### Contained object structure

```
struct Obj
{
    Point<Precision> pos;
    Point<Precision> size;
    Contained obj;
};
```

##### Constructor

```
QuadTree(const Point<Precision> &topLeft, const Point<Precision> &size);
```

##### Getters and setters

```
const Point<Precision> &getSize() const;
const Point<Precision> &getPos() const;
const std::vector<Obj> &getItems() const;
const std::vector<QuadTree> &getChildren() const;
```

```
std::vector<const Obj *> getAllItems(const std::function<bool(const Obj &)> condition = nullptr) const;
```

Returns a vector of all the items for which `condition` is true.

##### add

```
void add(auto &&obj, const Point<Precision> &pos, const Point<Precision> &size = {0, 0});
```

Adds an object to the tree.

##### contains

```
bool contains(const Contained &obj) const;
```

##### find

```
const Point<Precision> &find(const Contained &obj) const;
```

Returns the position of the object.

May throw std::out_of_range

##### move

```
void move(const Contained &obj, const Point<Precision> &pos);
```

Moves an object in the tree.

##### remove

```
void remove(const Contained &obj);
```

Removes an object from the tree.

##### getObjects

```
const std::vector<Obj> &getObjects(const Point<Precision> &pos) const;
```

Returns the set of objects closest to `pos`.

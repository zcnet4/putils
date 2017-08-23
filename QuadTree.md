# [QuadTree<Contained, Precision, MaxChildren>](QuadTree.hpp)

Space partitioner optimized for searching for an object's closest neighbors.

### Members

##### Contained object structure

```cpp
struct Obj
{
    Rect<Precision> boundingBox;
    Contained obj;
};
```

##### Constructor

```cpp
QuadTree(const Rect<Precision> &boundingBox);
```

##### add

```cpp
bool add(auto &&obj, const Rect<Precision> &boundingBox) noexcept;
```

Adds an object to the tree.

##### remove

```cpp
void remove(const Contained &obj);
```

Removes an object from the tree.

##### move

```cpp
void move(const Contained &obj, const Rect<Precision> &boundingBox) noexcept;
```

Moves an object in the tree.

##### query

```cpp
std::vector<Obj> query(const Rect<Precision> &area) const noexcept;
```

Returns the set of objects overlapping with `area`.

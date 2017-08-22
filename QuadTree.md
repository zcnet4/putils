# [QuadTree<Contained, Precision, MaxChildren>](QuadTree.hpp)

Space partitioner optimized for searching for an object's closest neighbors.

### Members

##### Contained object structure

```
struct Obj
{
    Rect<Precision> boundingBox;
    Contained obj;
};
```

##### Constructor

```
QuadTree(const Rect<Precision> &boundingBox);
```

##### add

```
bool add(auto &&obj, const Rect<Precision> &boundingBox) noexcept;
```

Adds an object to the tree.

##### remove

```
void remove(const Contained &obj);
```

Removes an object from the tree.

##### move

```
void move(const Contained &obj, const Rect<Precision> &boundingBox) noexcept;
```

Moves an object in the tree.

##### query

```
std::vector<Obj> query(const Rect<Precision> &area) const noexcept;
```

Returns the set of objects overlapping with `area`.

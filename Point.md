# Point<Precision, Dimensions>, Rect

Abstraction to a 2D or 3D point/rectangle.

## Point

### Members

##### Coordinates

```
Precision x;
Precision y;
{ Precision z; } // If Dimensions == 3
```

##### Operators

```
bool operator==(const Point<P> &rhs) const;
bool operator!=(const Point<P> &rhs) const;
Point operator+(const Point<P> &rhs) const;
Point &operator+=(const Point<P> &rhs);
Point operator-(const Point<P> &rhs) const;
Point &operator-=(const Point<P> &rhs);
```

##### distanceTo

```
Precision distanceTo(const Point<P> &rhs) const;
```
Returns the distance to another point.

## Rect

### Members

##### Coordinates

```
Point<Precision> topLeft;
Point<Precision> size;
```

##### Operators

```
bool operator==(const Rect<P> &rhs) const;
bool operator!=(const Rect<P> &rhs) const;
```

##### intersect

```
bool intersect(const Rect &other) const
```
Returns whether this rectangle intersects with `other`.

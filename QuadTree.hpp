#pragma once

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstddef>
#include "Point.hpp"
#include "fwd.hpp"

namespace putils
{
    template<typename Contained, typename Precision = int, std::size_t MaxChildren = 4>
    class QuadTree
    {
    public:
        struct Obj
        {
            Rect<Precision> boundingBox;
            Contained obj;
        };

    public:
        QuadTree(const Rect<Precision> &boundingBox)
                  : _items(), _children(), _boundingBox(boundingBox)
        {}

    public:
        template<typename Obj>
        bool add(Obj &&obj, const Rect<Precision> &boundingBox) noexcept
        {
            if (!_boundingBox.intersect(boundingBox))
                return false; // object isn't in this area

            if (_items.size() < MaxChildren || _boundingBox.size.x < 2 || _boundingBox.size.y < 2)
            {
                _items.push_back({ boundingBox, obj });
                return true;
            }

            if (!_children.size())
                divideIntoChildren();

            bool good = false;
            for (auto &c : _children)
                good |= c.add(obj, boundingBox);

            if (!good)
            {
                std::cerr << "[QuadTree] Couldn't add object. This should never happen." << std::endl;
                return false;
            }

            return true;
        }

    public:
        void remove(const Contained &obj) noexcept
        {
            auto it = std::find_if(_items.begin(), _items.end(),
                                   [&obj](const Obj &item) { return item.obj == obj; });
            while (it != _items.end())
            {
                _items.erase(it);
                it = std::find_if(_items.begin(), _items.end(),
                                       [&obj](const Obj &item) { return item.obj == obj; });
            }

            for (auto &c : _children)
                c.remove(obj);
        }

    public:
        void move(const Contained &obj, const Rect<Precision> &boundingBox) noexcept
        {
            remove(obj);
            add(obj, boundingBox);
        }

    public:
        std::vector<Obj> query(const Rect<Precision> &area) const noexcept
        {
            std::vector<Obj> ret;

            if (!_boundingBox.intersect(area))
                return ret;

            for (const auto &i : _items)
                if (area.intersect(i.boundingBox))
                    ret.push_back(i);

            for (const auto &c : _children)
                for (const auto &obj : c.query(area))
                    ret.push_back(obj);

            return ret;
        }

    private:
        void divideIntoChildren() noexcept
        {
            const Point<Precision> childSize = { _boundingBox.size.x / 2, _boundingBox.size.y / 2};
            _children.push_back(QuadTree({ _boundingBox.topLeft, childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x, _boundingBox.topLeft.y + childSize.y }, childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x, _boundingBox.topLeft.y }, childSize }));
            _children.push_back(QuadTree({ { _boundingBox.topLeft.x + childSize.x, _boundingBox.topLeft.y + childSize.x }, childSize }));
        }

    private:
        std::vector<Obj> _items;
        std::vector<QuadTree> _children;
        Rect<Precision> _boundingBox;
    };

    namespace test
    {
        inline void quadTree()
        {
            putils::QuadTree<int> tree({ { 0, 0 }, { 16, 16 } });

            int id = 0;
            for (int x = 0; x < 64; ++x)
                for (int y = 0; y < 64; ++y)
                    tree.add(++id, { { x, y }, { 1, 1 } });

            tree.add(4242, { { 3, 3 }, { 2, 2 } });

            const auto objects = tree.query({ { 0, 0 }, { 4, 4 } });
            for (const auto &obj : objects)
                std::cout << obj.obj << " -- " << obj.boundingBox << std::endl;
        }
    }
}
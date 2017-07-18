#pragma once

#include <memory>
#include <limits>

#ifdef _WIN32
#undef max
#endif

namespace putils
{
    namespace
    {
        template<std::size_t Size>
        class Chunk
        {
            static inline constexpr unsigned char NumBlocks = std::numeric_limits<unsigned char>::max();

        public:
            Chunk() : _mem(Size * NumBlocks)
            {
                unsigned char i = 0;
                for (unsigned char *p = _mem.data(); i < NumBlocks; p += Size)
                    *p = ++i;
            }

            void *allocate()
            {
                auto ptr = _mem.data() + Size * _firstAvailableBlock;
                _firstAvailableBlock = *ptr;
                --available;
                return ptr;
            }

            void release(void *ptr)
            {
                auto p = static_cast<unsigned char *>(ptr);
                *p = _firstAvailableBlock;
                _firstAvailableBlock = static_cast<unsigned char>((p - _mem.data()) / Size);
                ++available;
            }

            bool contains(void *ptr) const { return ptr >= _mem.data() && ptr < _mem.data() + Size * NumBlocks; }

            unsigned char available = NumBlocks;

        private:
            unsigned char _firstAvailableBlock = 0;
            std::vector<unsigned char> _mem;
        };

        template<typename T>
        class Pool
        {
        public:
            T &get()
            {
                if (_lastAlloc == nullptr)
                {
                    if (_lastRelease != nullptr && _lastRelease->available)
                        _lastAlloc = _lastRelease;
                }
                else if (!_lastAlloc->available)
                {
                    _lastAlloc = nullptr;
                    for (auto &c : _chunks)
                        if (c.available)
                        {
                            _lastAlloc = &c;
                            break;
                        }
                }

                if (_lastAlloc == nullptr)
                {
                    _chunks.push_back(Chunk<sizeof(T)>{});
                    _lastAlloc = &_chunks.back();
                    _lastRelease = _lastAlloc;
                }

                return *static_cast<T*>(_lastAlloc->allocate());
            }

            void release(const T &obj)
            {
                const auto ptr = (void*)(&obj);
                if (_lastRelease != nullptr && _lastRelease->contains(ptr))
                {
                    _lastRelease->release(ptr);
                    return;
                }

                for (auto &c : _chunks)
                {
                    if (c.contains(ptr))
                    {
                        _lastRelease = &c;
                        c.release(ptr);
                        return;
                    }
                }
            }

        private:
            std::vector<Chunk<sizeof(T)>> _chunks;
            Chunk<sizeof(T)> *_lastAlloc = nullptr;
            Chunk<sizeof(T)> *_lastRelease = nullptr;
        };
    }

    template<typename CRTP>
    class PooledObject
    {
    public:
        void *operator new(std::size_t) { return &_allocator.get(); }
        void operator delete(void *ptr) { &_allocator.release(*static_cast<const CRTP*>(ptr)); }

    private:
        static inline Pool<CRTP> _allocator;
    };
}
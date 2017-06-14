#pragma once

#include <memory>
#include <limits>

#ifdef _WIN32
#undef max
#endif

namespace putils
{
    template<typename CRTP>
    class PooledObject
    {
    public:
        void *operator new(std::size_t size);
        void operator delete(void *ptr);
    };

    /*
     * Implementation details
     */

    namespace
    {
        template<std::size_t Size>
        class Chunk
        {
            static constexpr std::size_t NumBlocks = std::numeric_limits<unsigned char>::max();

        public:
            Chunk() : _mem(std::make_unique<unsigned char[]>(Size * NumBlocks))
            {
                unsigned char i = 0;
                available = NumBlocks;
                for (unsigned char *p = _mem.get(); i < NumBlocks - 1; p += Size)
                    *p = ++i;
            }

            void *allocate()
            {
                auto ptr = _mem.get() + Size * _firstAvailableBlock;
                _firstAvailableBlock = *ptr;
                --available;
                return ptr;
            }

            void release(void *ptr)
            {
                auto p = static_cast<unsigned char *>(ptr);
                *p = _firstAvailableBlock;
                _firstAvailableBlock = (p - _mem.get()) / Size;
                ++available;
            }

            bool contains(void *ptr) const { return ptr >= _mem.get() && ptr < _mem.get() + Size * NumBlocks; }

            unsigned char available;

        private:
            unsigned char _firstAvailableBlock = 0;
            std::unique_ptr<unsigned char[]> _mem;
        };

        template<typename T>
        class Pool
        {
        public:
            static void *allocate()
            {
                if (_lastAlloc == nullptr)
                {
                    if (_lastRelease != nullptr && _lastRelease->available)
                        _lastAlloc = _lastRelease;
                }
                else if (!_lastAlloc->available)
                {
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

                return _lastAlloc->allocate();
            }

            static void release(void *ptr)
            {
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
                    }
                }
            }

        private:
            static std::vector<Chunk<sizeof(T)>> _chunks;
            static Chunk<sizeof(T)> *_lastAlloc;
            static Chunk<sizeof(T)> *_lastRelease;
        };
    }

    template<typename T>
    std::vector<Chunk<sizeof(T)>> Pool<T>::_chunks;

    template<typename T>
    Chunk<sizeof(T)> *Pool<T>::_lastAlloc = nullptr;

    template<typename T>
    Chunk<sizeof(T)> *Pool<T>::_lastRelease = nullptr;

    template<typename CRTP>
    void *PooledObject<CRTP>::operator new(std::size_t size) { return Pool<CRTP>::allocate(); }

    template<typename CRTP>
    void PooledObject<CRTP>::operator delete(void *ptr) { Pool<CRTP>::release(ptr); }
}
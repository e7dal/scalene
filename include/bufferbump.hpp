#ifndef BUFFERBUMP_H
#define BUFFERBUMP_H

#include "classwarfare.hpp"
#include "buffer.hpp"

template <unsigned long NumClasses,
	  unsigned long Size = 1024UL * 1048576UL,
	  unsigned long Multiple = 8>
class BufferBump {
public:

  BufferBump() {
    for (auto i = 0; i < NumClasses; i++) {
      _bump[i] = (char *) _buf.getBuffer(i);
    }
  }
  
  __attribute__((noinline)) void * malloc(size_t sz) {
    size_t rounded;
    unsigned long sizeClass;
    ClassWarfare<Multiple>::getSizeAndClass(sz, rounded, sizeClass);
    auto ptr = _bump[sizeClass];
    _bump[sizeClass] += rounded;
    return reinterpret_cast<void *>(ptr);
  }

  inline size_t getSize(void * ptr) {
    size_t sz;
    auto cl = _buf.getClass(ptr);
#if 1
    if (unlikely((cl < 0) || (cl >= NumClasses))) {
      return 0;
    }
#endif
    ClassWarfare<Multiple>::getSizeFromClass(cl, sz);
    return sz;
  }
  
private:
  Buffer<NumClasses, Size> _buf;
  char * _bump[NumClasses];
};

#endif

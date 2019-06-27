#ifndef DGSTATICARRAY_H
#define DGSTATICARRAY_H

namespace Dg
{
  template<typename T, int SIZE>
  class StaticArray
  {
  public:

    T const & operator[](size_t i) const { return m_data[i]; }
    T & operator[](size_t i) { return m_data[i]; }

  private:
    T m_data[SIZE];
  };
}

#endif
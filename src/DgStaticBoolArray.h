//@group Collections

#ifndef DGSTATICBOOLARRAY_H
#define DGSTATICBOOLARRAY_H

#include <cstring>
#include <stdint.h>
#include <type_traits>

#include "DgError.h"

namespace Dg
{
  template<size_t ELEMENT_COUNT, typename INT_TYPE = uint32_t>
  class StaticBoolArray
  {
    static_assert(std::is_unsigned<INT_TYPE>::value, "Bucket type must be unsigned integer type!");
  public:

    StaticBoolArray() : m_data{} {}
    ~StaticBoolArray() {}

    StaticBoolArray(StaticBoolArray const &);
    StaticBoolArray & operator= (StaticBoolArray const &);

    ErrorCode Toggle(size_t);
    ErrorCode SetOn(size_t);
    ErrorCode SetOff(size_t);
    ErrorCode Set(size_t, bool);
    ErrorCode Get(size_t, bool &) const;
    bool operator[](size_t) const; // No error check

    void SetAll(bool);
    void SetAllOn();
    void SetAllOff();

    size_t Size() const;

  private:

    static size_t const s_nBits = (sizeof(INT_TYPE) * CHAR_BIT);
    static size_t const s_nContainers = (ELEMENT_COUNT + sizeof(INT_TYPE) - 1) / s_nBits;

    INT_TYPE m_data[s_nContainers];
  };

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::StaticBoolArray(StaticBoolArray const & a_other)
    : m_data{}
  {
    memcpy(m_data, a_other.m_data, sizeof(m_data));
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  StaticBoolArray<ELEMENT_COUNT, INT_TYPE> & StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::operator=(StaticBoolArray const & a_other)
  {
    if (this != &a_other)
      memcpy(m_data, a_other.m_data, sizeof(m_data));
    return *this;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  ErrorCode StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::Toggle(size_t a_index)
  {
    ErrorCode result;

    bool isOn;
    DG_ERROR_CHECK(Get(a_index, isOn));
    DG_ERROR_CHECK(Set(a_index, !isOn));

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  ErrorCode StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::SetOn(size_t a_index)
  {
    ErrorCode result;

    DG_ERROR_IF(a_index >= ELEMENT_COUNT, ErrorCode::OutOfBounds);

    size_t const bucket = a_index / s_nBits;
    size_t const shf = a_index - (bucket * s_nBits);

    m_data[bucket] = m_data[bucket] | (static_cast<INT_TYPE>(1) << shf);

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  ErrorCode StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::SetOff(size_t a_index)
  {
    ErrorCode result;

    DG_ERROR_IF(a_index >= ELEMENT_COUNT, ErrorCode::OutOfBounds);

    size_t const bucket = a_index / s_nBits;
    size_t const shf = a_index - (bucket * s_nBits);

    m_data[bucket] = m_data[bucket] & ~(static_cast<INT_TYPE>(1) << shf);

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  ErrorCode StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::Set(size_t a_index, bool a_on)
  {
    ErrorCode result;

    DG_ERROR_IF(a_index >= ELEMENT_COUNT, ErrorCode::OutOfBounds);

    size_t const bucket = a_index / s_nBits;
    size_t const shf = a_index - (bucket * s_nBits);

    if (a_on)
      m_data[bucket] = m_data[bucket] | (static_cast<INT_TYPE>(1) << shf);
    else
      m_data[bucket] = m_data[bucket] & ~(static_cast<INT_TYPE>(1) << shf);

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  ErrorCode StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::Get(size_t a_index, bool & a_isOn) const
  {
    ErrorCode result;

    DG_ERROR_IF(a_index >= ELEMENT_COUNT, ErrorCode::OutOfBounds);

    size_t const bucket = a_index / s_nBits;
    size_t const shf = a_index - (bucket * s_nBits);

    a_isOn = (m_data[bucket] & (static_cast<INT_TYPE>(1) << shf)) != static_cast<INT_TYPE>(0);

    result = ErrorCode::None;
  epilogue:
    return result;
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  bool StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::operator[](size_t a_index) const
  {
    size_t const bucket = a_index / s_nBits;
    size_t const shf = a_index - (bucket * s_nBits);
    return (m_data[bucket] & (static_cast<INT_TYPE>(1) << shf)) != static_cast<INT_TYPE>(0);
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  void StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::SetAll(bool a_val)
  {
    if (a_val)
      SetAllOn();
    else
      SetAllOff();
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  void StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::SetAllOn()
  {
    memset(m_data, -1, s_nContainers * sizeof(INT_TYPE));
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  void StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::SetAllOff()
  {
    memset(m_data, 0, s_nContainers * sizeof(INT_TYPE));
  }

  template<size_t ELEMENT_COUNT, typename INT_TYPE>
  size_t StaticBoolArray<ELEMENT_COUNT, INT_TYPE>::Size() const
  {
    return ELEMENT_COUNT;
  }
};
#endif
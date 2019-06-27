//! @file DgIDManager.h
//!
//! @author: Frank B. Hart
//! @date 22/07/2016
//!
//! Class declaration: IDManager

#ifndef DGIDSERVER_H
#define DGIDSERVER_H

#include "DgDoublyLinkedList.h"

namespace Dg
{
  //! @ingroup DgUtility
  //!
  //! @class IDManager
  //!
  //! The IDManager serves unique ID's with a certain range. It does this by storing a set of ranges
  //! of available IDs. If an ID is taken from the a set, the set bounds may be incremented or decremented,
  //! or borken, creating a new set.
  //!
  //! @author Frank Hart
  //! @date 23/07/2016
  template<typename T>
  class IDManager
  {
  public:

    //! The default range will be simply 1
    IDManager();
    
    //! Construct the manager with a lower and upper limit to the ID pool
    IDManager(T lower, T upper);

    ~IDManager() {}
    IDManager(IDManager<T> const &);
    IDManager & operator=(IDManager<T> const &);

    //! Initialize the ID manager with a lower and upper bound.
    void Init(T lower, T upper);
    
    //! Get the next available ID.
    //!
    //! @return 0 if no more IDs are available.
    T GetID();
    
    //! Mark an ID as available.
    void ReturnID(T);

    //! Mark an ID as in use.
    //!
    //! @return false if id already in use.
    bool MarkAsUsed(T);
    
    //! Check to see if an ID is in use.
    bool IsUsed(T) const;

  private:

    //! Intervals are ranges of available numbers
    class Interval
    {
    public:
      Interval(T a_lower, T a_upper)
        : m_lower(a_lower)
        , m_upper(a_upper)
      {}

      bool IsOne() const { return m_lower == m_upper; }

      bool operator<(Interval const &) const;
      bool operator>(Interval const &) const;

    public:
      T m_lower;
      T m_upper;
    };
    
    typedef DoublyLinkedList<Interval>                           list;
    typedef typename DoublyLinkedList<Interval>::iterator        iterator;
    typedef typename DoublyLinkedList<Interval>::const_iterator  const_iterator;

    list         m_intervals;
    Interval     m_bounds;
  };


  //-------------------------------------------------------------------------------
  //		@ IDManager<T>::Interval::operator<()
  //-------------------------------------------------------------------------------
  template<typename T>
  bool IDManager<T>::Interval::operator<(typename IDManager<T>::Interval const & a_other) const
  {
    return m_upper < a_other.m_lower;
  } //End: IDManager::Interval::operator<()


  //-------------------------------------------------------------------------------
  //		@ IDManager::Interval::operator>()
  //-------------------------------------------------------------------------------
  template<typename T>
  bool IDManager<T>::Interval::operator>(typename IDManager<T>::Interval const & a_other) const
  {
    return m_upper > a_other.m_lower;
  } //End: IDManager::Interval:operator>()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::IDManager()
  //-------------------------------------------------------------------------------
  template<typename T>
  IDManager<T>::IDManager()
    : m_bounds(static_cast<T>(1), static_cast<T>(1))
  {
    m_intervals.push_back(m_bounds);
  } //End: IDManager::IDManager()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::IDManager()
  //-------------------------------------------------------------------------------
  template<typename T>
  IDManager<T>::IDManager(T a_lower, T a_upper)
    : m_bounds(a_lower, a_upper)
  {
    Init(a_lower, a_upper);
  } //End: IDManager::IDManager()


  //-------------------------------------------------------------------------------
  //		@ IDManager::IDManager()
  //-------------------------------------------------------------------------------
  template<typename T>
  IDManager<T>::IDManager(IDManager<T> const & a_other)
    : m_intervals(a_other.m_intervals)
    , m_bounds(a_other.m_bounds)
  {
  } //End: IDManager::IDManager()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::operator=()
  //-------------------------------------------------------------------------------
  template<typename T>
  IDManager<T> & IDManager<T>::operator=(IDManager<T> const & a_other)
  {
    if (this != &a_other)
    {
      m_intervals = a_other.m_intervals;
      m_bounds = a_other.m_bounds;
    }
    return *this;
  } //End: IDManager::operator=()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::Init()
  //-------------------------------------------------------------------------------
  template<typename T>
  void IDManager<T>::Init(T a_lower, T a_upper)
  {
    m_intervals.clear();
    if (a_lower > a_upper) a_lower = a_upper;
    m_bounds.m_lower = a_lower;
    m_bounds.m_upper = a_upper;
    m_intervals.push_front(m_bounds);
  } //End: IDManager::Init()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::GetID()
  //-------------------------------------------------------------------------------
  template<typename T>
  T IDManager<T>::GetID()
  {
    if (m_intervals.empty())
    {
      return static_cast<T>(0);
    }
 
    iterator it = m_intervals.begin();
    T result = it->m_lower;
    it->m_lower++;
    if (it->m_lower > it->m_upper)
    {
      m_intervals.erase(it);
    }

    return result;
  } //End: IDManager::GetID()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::ReturnID()
  //-------------------------------------------------------------------------------
  template<typename T>
  void IDManager<T>::ReturnID(T a_val)
  {
    //Check bounds
    if (a_val < m_bounds.m_lower || a_val > m_bounds.m_upper)
    {
      return;
    }

    iterator it = m_intervals.begin();
    bool found = false;
    for (it; it != m_intervals.end(); it++)
    {
      if (a_val <= it->m_upper)
      {
        found = true;
        if (it == m_intervals.begin())
        {
          if (a_val == (it->m_lower - static_cast<T>(1)))
          {
            it->m_lower--;
          }
          else
          {
            m_intervals.insert(it, Interval(a_val, a_val));
          }
        }
        else
        {
          iterator itp(it); --itp;
          if (a_val == (itp->m_upper + static_cast<T>(1)))
          {
            itp->m_upper++;
            if (itp->m_upper == it->m_lower - static_cast<T>(1))
            {
              it->m_lower = itp->m_lower;
              m_intervals.erase(itp);
            }
          }
          else if (a_val == (it->m_lower - static_cast<T>(1)))
          {
            it->m_lower--;
            if (itp->m_upper == it->m_lower - static_cast<T>(1))
            {
              it->m_lower = itp->m_lower;
              m_intervals.erase(itp);
            }
          }
          else
          {
            m_intervals.insert(it, Interval(a_val, a_val));
          }
        }
        break;
      }
    }

    //We're at the end
    if (!found)
    {
      if (m_intervals.empty() || a_val != (m_intervals.back().m_upper + static_cast<T>(1)))
      {
        m_intervals.push_back(Interval(a_val, a_val));
      }
      else
      {
        m_intervals.back().m_upper++;
      }
    }
  } //End: IDManager::ReturnID()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::MarkAsUsed()
  //-------------------------------------------------------------------------------
  template<typename T>
  bool IDManager<T>::MarkAsUsed(T a_val)
  {
    iterator it = m_intervals.begin();
    bool good = false;
    for (it; it != m_intervals.end(); it++)
    {
      if (a_val <= it->m_upper)
      {
        good = true;
        if (a_val == it->m_lower)
        {
          if (it->IsOne()) it = m_intervals.erase(it);
          else it->m_lower++;
        }
        else if (a_val == it->m_upper)
        {
          if (it->IsOne()) it = m_intervals.erase(it);
          else it->m_upper--;
        }
        else if (a_val > it->m_lower)
        {
          m_intervals.insert(it, Interval(it->m_lower, a_val - static_cast<T>(1) ));
          it->m_lower = a_val + static_cast<T>(1);
        }
        else //Already marked as used
        {
          good = false;
        }
        break;
      }
    }
    return good;
  } //End: IDManager::MarkAsUsed()

  
  //-------------------------------------------------------------------------------
  //		@ IDManager::IsUsed()
  //-------------------------------------------------------------------------------
  template<typename T>
  bool IDManager<T>::IsUsed(T a_val) const
  {
    //Check bounds
    if (a_val < m_bounds.m_lower || a_val > m_bounds.m_upper)
    {
      return false;
    }

    const_iterator it = m_intervals.cbegin();
    for (it; it != m_intervals.cend(); it++)
    {
      if (a_val <= it->m_upper)
      {
        return a_val < it->m_lower;
      }
    }
    return true;
  } //End: IDManager::IsUsed()
}


#endif
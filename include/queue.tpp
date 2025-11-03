// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "queue.h"

//======================================================================//
//                                                                      //
//                           queue<T> methods                           //
//                                                                      //
//======================================================================//

template <class T>
queue<T>::queue(std::initializer_list<T> list)
{
  this->list_ = list;
  this->size_ = this->list_.size();
}

template <class T>
queue<T>& queue<T>::enqueue(const T& val)
{
  this->list_.prepend(val);
  this->size_++;
  return *this;
}

template <class T>
T queue<T>::dequeue()
{
  assert(!this->is_empty() && "dequeue() called on empty queue");
  T ret = this->list_.tail()->val();
  this->list_.delete_tail();
  this->size_--;
  return ret;
}

template <class T>
queue<T>& queue<T>::clear()
{
  this->list_.clear();
  this->size_ = 0;
  return *this;
}

template <class T>
bool queue<T>::operator == (const queue<T>& other) const
{
  return this->list_ == other.list_;
}

template <class T>
bool queue<T>::operator != (const queue<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool queue<T>::operator < (const queue<T>& other) const
{
  return this->list_ < other.list_;
}

template <class T>
bool queue<T>::operator > (const queue<T>& other) const
{
  return this->list_ > other.list_;
}

template <class T>
bool queue<T>::operator <= (const queue<T>& other) const
{
  return this->list_ <= other.list_;
}

template <class T>
bool queue<T>::operator >= (const queue<T>& other) const
{
  return this->list_ >= other.list_;
}

template <class T>
queue<T>& queue<T>::operator = (std::initializer_list<T> list)
{
  this->clear();
  this->list_ = list;
  this->size_ = this->list_.size();
  return *this;
}

template <class T>
queue<T>& queue<T>::operator = (const linked_list<T, true>& source)
{
  this->clear();
  if (source.is_empty())
    return *this;
  this->list_ = source;
  this->size_ = this->list_.size();
  return *this;
}

template <class T>
queue<T>& queue<T>::operator = (const dyn_array<T>& source)
{
  this->clear();
  if (source.is_empty())
    return *this;
  this->list_ = source;
  this->size_ = this->list_.size();
  return *this;
}

template <class T>
std::ostream& operator << (std::ostream& os, const queue<T>& que)
{
  os << que.list_;
  return os;
}
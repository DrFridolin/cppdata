// The header is included primarily to get rid of IntelliSense squigglies.
// The class should build and run properly without it. Header guard protection
// prevents the apparent circular inclusion from causing any problems.
#include "stack.h"

//======================================================================//
//                                                                      //
//                           stack<T> methods                           //
//                                                                      //
//======================================================================//

template <class T>
stack<T>::stack(std::initializer_list<T> list)
{
  this->list_ = node<T, false>::create(T());
  *(this->list_) = list;
  this->size_ = this->list_->size();
}

template <class T>
stack<T>::stack(const stack<T>& source)
{
  if (!source.is_empty()) {
    this->list_ = source.list_->clone();
    this->size_ = source.size_;
  }
  else {
    this->list_ = nullptr;
    this->size_ = 0;
  }
}

template <class T>
stack<T>::~stack()
{
  if (this->list_)
    delete this->list_;
}

template <class T>
stack<T>& stack<T>::push(const T& val)
{
  if (this->list_)
    this->list_ = this->list_->prepend(val);
  else
    this->list_ = node<T, false>::create(val);
  this->size_++;
  return *this;
}

template <class T>
T stack<T>::pop()
{
  assert(!this->is_empty() && "pop() called on empty stack");
  T ret = this->list_->val();
  this->list_ = this->list_->delete_head();
  this->size_--;
  return ret;
}

template <class T>
stack<T>& stack<T>::clear()
{
  if (this->list_)
    delete this->list_;
  this->list_ = nullptr;
  this->size_ = 0;
  return *this;
}

template <class T>
bool stack<T>::operator == (const stack<T>& other) const
{
  return *(this->list_) == *(other.list_);
}

template <class T>
bool stack<T>::operator != (const stack<T>& other) const
{
  return !(*this == other);
}

template <class T>
bool stack<T>::operator < (const stack<T>& other) const
{
  return *(this->list_) < *(other.list_);
}

template <class T>
bool stack<T>::operator > (const stack<T>& other) const
{
  return *(this->list_) > *(other.list_);
}

template <class T>
bool stack<T>::operator <= (const stack<T>& other) const
{
  return *(this->list_) <= *(other.list_);
}

template <class T>
bool stack<T>::operator >= (const stack<T>& other) const
{
  return *(this->list_) >= *(other.list_);
}

template <class T>
stack<T>& stack<T>::operator = (const stack<T>& source)
{
  this->clear();
  if (source.is_empty())
    return *this;
  this->list_ = source.list_->clone();
  this->size_ = source.size_;
  return *this;
}

template <class T>
stack<T>& stack<T>::operator = (std::initializer_list<T> list)
{
  this->clear();
  this->list_ = node<T, false>::create(T());
  *(this->list_) = list;
  this->size_ = this->list_->size();
  return *this;
}

template <class T>
stack<T>& stack<T>::operator = (const node<T, false>* source)
{
  this->clear();
  if (source == nullptr)
    return *this;
  this->list_ = source->clone();
  this->size_ = this->list_->size();
  return *this;
}

template <class T>
stack<T>& stack<T>::operator = (const dyn_array<T>& source)
{
  this->clear();
  if (source.is_empty())
    return *this;
  this->list_ = node<T, false>::create(T());
  *(this->list_) = source;
  this->size_ = this->list_->size();
  return *this;
}

template <class T>
std::ostream& operator << (std::ostream& os, const stack<T>& st)
{
  os << *st.list_;
  return os;
}
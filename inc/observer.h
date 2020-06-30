#pragma once

#include <iostream>
#include <memory>
#include <list>

namespace otus {

template<typename T>
class IObserver 
{
public:
  virtual ~IObserver() = default;

  virtual void Update(const T&) = 0;
};

template<typename T>
class IObservable 
{
public:
  virtual ~IObservable() = default;

  virtual void Subscribe(const std::shared_ptr< IObserver<T> >& a_ptrObserver) = 0;
};

template<typename T>
class BaseObservable : public IObservable<T> 
{
public:
  void Subscribe(const std::shared_ptr< IObserver<T> >& a_ptrObserver) override
  {
    m_subscribers.emplace_back(a_ptrObserver);
  }

  void Notify(const T& a_data) {
    auto iter = m_subscribers.begin();
    while (iter != m_subscribers.end()) {
      auto ptr = iter->lock();
      if (ptr) {
          ptr->Update(a_data);
          ++iter;
      } else {
          m_subscribers.erase(iter++);
      }
    }
  }

private:
  std::list< std::weak_ptr<IObserver<T> > > m_subscribers;
};

} // otus::

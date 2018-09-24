#include <algorithm>
#include <vector>
#include <stdio.h>

/*
 *
 * 事先声明
 */
class Observable;

class Observer
{
 public:
  /*
   * 虚函数
   */
  virtual ~Observer();
  /*
   * 纯虚函数
   */
  virtual void update() = 0;

  void observe(Observable* s);

 protected:
  Observable* subject_;
};

/*
 *
 * 非线程安全的设计
 */
class Observable
{
 public:
  void register_(Observer* x);
  void unregister(Observer* x);

  /*
   * 通知观察者
   *
   */
  void notifyObservers()
  {
    for (size_t i = 0; i < observers_.size(); ++i)
    {
      Observer* x = observers_[i];
      if (x) {
        x->update(); // (3)
      }
    }
  }

 private:
  std::vector<Observer*> observers_;
};

/*
 *
 * 析构时，进行解注册
 */
Observer::~Observer()
{
  subject_->unregister(this);
}

void Observer::observe(Observable* s)
{
  s->register_(this);
  subject_ = s;
}

void Observable::register_(Observer* x)
{
  observers_.push_back(x);
}

void Observable::unregister(Observer* x)
{
  std::vector<Observer*>::iterator it = std::find(observers_.begin(), observers_.end(), x);
  if (it != observers_.end())
  {
    std::swap(*it, observers_.back());
    observers_.pop_back();
  }
}

// ---------------------

/*
 *
 * 具体的观察者，子类
 */
class Foo : public Observer
{
  virtual void update()
  {
    printf("Foo::update() %p\n", this);
  }
};

int main()
{
  Foo* p = new Foo;
  Observable subject;
  p->observe(&subject);
  subject.notifyObservers();
  /*
   *
   * 这里删除对象，触发析构
   */
  delete p;
  subject.notifyObservers();
}

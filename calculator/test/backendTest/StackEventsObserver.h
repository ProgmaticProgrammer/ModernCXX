#ifndef STACK_EVENTS_OB_HEADER
#define STACK_EVENTS_OB_HEADER

#include "src/backend/Stack.hpp"
#include "src/utilities/Observer.hpp"

using namespace calculator::model;
using namespace calculator::utility;
using ErrorConditions = StackEvent::ErrorConditions;
using ErrorConditions = StackEvent::ErrorConditions;


class StackChangedObserver : public Observer {
 public:
  StackChangedObserver(std::string name) : Observer{name} {}
  size_t changeCount() const { return changeCount_; }

  void notifyImpl(std::shared_ptr<EventData>) { ++changeCount_; }

 private:
  size_t changeCount_ = 0;
};

// Observer for error information
class StackErrorObserver : public Observer {
 public:
  StackErrorObserver(string name) : Observer{name} {}

  const vector<string>& errorMessages() const { return messages_; }
  const vector<ErrorConditions>& errors() const {
    return errors_;
  }

  void notifyImpl(std::shared_ptr<EventData> data) {
    std::shared_ptr<StackEvent> p =
        std::dynamic_pointer_cast<StackEvent>(data);
    if (p) {
      messages_.push_back(p->message());
      errors_.push_back(p->error());
    }
  }

 private:
  vector<string> messages_;
  vector<ErrorConditions> errors_;
};

#endif

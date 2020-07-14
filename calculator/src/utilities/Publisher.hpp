// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#ifndef PUBLISHER_H
#define PUBLISHER_H

// The Publisher class is a class capable of receiving observers. Note that it
// is assumed that a real publisher may publish multiple separate events. These
// are stored by string name in a table. Since each event may have multiple
// observers, the table stores a collection of observers.

// NOTE: This is a push model meaning it's the publisher that sents the event data

#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Exception.hpp"
#include "Observer.hpp"

using std::ostringstream;
using std::set;
using std::string;
using std::vector;
using std::weak_ptr;
using std::shared_ptr;

namespace calculator {
namespace utility {

class EventData {
 public:
  virtual ~EventData() = default;
};

class Publisher {
  using ObserversList = std::unordered_map<string, weak_ptr<Observer>>;
  using Events = std::unordered_map<string, ObserversList>;

 public:
  Publisher() = default;

  void attach(const std::string& eventName,
              std::shared_ptr<Observer> observer) {
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;

    auto name = observer->name();

    auto obs = obsList.find(name);
    if (obs != obsList.end())
      throw Exception("Observer already attached to publisher");

    obsList.insert(std::make_pair(std::move(name), weak_ptr<Observer>(observer)));
  }

  void detach(const std::string& eventName,
                                   const std::string& observer) {
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;

    auto obs = obsList.find(observer);
    if (obs == obsList.end())
      throw Exception("Cannot detach observer because observer not found");

    obsList.erase(obs);
  }

  std::set<std::string> listEvents() const {
    set<string> tmp;
    for (const auto& i : events_) tmp.insert(i.first);

    return tmp;
  }

  std::set<std::string> listEventObservers(const std::string& eventName) const {
    auto ev = findCheckedEvent(eventName);

    set<string> tmp;
    for (const auto& kvp : ev->second) tmp.insert(kvp.first);

    return tmp;
  }

 protected:
  ~Publisher() = default;

  void notify(const string& eventName, const shared_ptr<EventData> d) const {
    auto ev = findCheckedEvent(eventName);
    auto& obsList = ev->second;

    for (auto& obs : obsList)
    {
        if (auto observe = obs.second.lock())
            observe->onNotify(d);
    }
  }

  void registerEvent(const std::string& eventName) {
    auto i = events_.find(eventName);
    if (i != events_.end()) throw Exception{"Event already registered"};

    events_[eventName] = ObserversList{};
  }
  void registerEvents(const std::vector<std::string>& eventNames) {
    for (auto i : eventNames) registerEvent(i);
  }

  Events::const_iterator findCheckedEvent(const string& eventName) const {
    auto ev = events_.find(eventName);
    if (ev == events_.end()) {
      ostringstream oss;
      oss << "Publisher does not support event '" << eventName << "'";
      throw Exception{oss.str()};
    }

    return ev;
  }

  Events::iterator findCheckedEvent(const string& eventName)
  {
      auto ev = events_.find(eventName);
      if( ev == events_.end() )
      {
          ostringstream oss;
          oss << "Publisher does not support event '" << eventName << "'";
          throw Exception( oss.str() );
      }

      return ev;
  }

 private:
  Events events_;
};

}  // namespace utility
}  // namespace calculator

#endif

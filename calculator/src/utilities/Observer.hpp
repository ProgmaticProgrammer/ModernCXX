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

#ifndef OBSERVER_H
#define OBSERVER_H

// The Observer class serves as the base class for all observer objects. The
// Observer class is a mediator between the Publisher and the actual class that
// performs the observation. A concrete observer should inherit from observer
// and contain a pointer to the actual observing object. This decouples the
// observing object from knowing about publishers. Note that publishers call
// Observer's notify, which must be implemented in the concrete observer to
// respond to event notifications.

#include <memory>
#include <string>

namespace calculator {
namespace utility {

class EventData;

class Observer {
 public:
  explicit Observer(const std::string& name) : name_(name) {}
  virtual ~Observer() = default;

  void onNotify(std::shared_ptr<EventData> d) { notifyImpl(d); }

  const std::string name() const { return name_; }

 private:
  virtual void notifyImpl(std::shared_ptr<EventData>) = 0;
  std::string name_;
};

}  // namespace utility
}  // namespace calculator

#endif

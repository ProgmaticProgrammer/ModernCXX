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

#ifndef COMMAND_REPOSITORY_H
#define COMMAND_REPOSITORY_H

// The CommandRepository class is responsible for returning a Command by name. New commands
// can be dynamically added at runtime (to support) plugins, and commands can also be
// deregistered (if desired if a plugin is removed). New commands are returned as clones
// of the registered Command. This makes use of the Prototype pattern.

#include <unordered_map>
#include <memory>
#include <string>
#include <set>
#include <sstream>
#include "utilities/Command.hpp"
#include "utilities/Exception.hpp"

namespace calculator {
namespace controller {

using calculator::utility::Exception;
using calculator::utility::CommandPtr;
using std::unordered_map;
using std::string;

class CommandRepository
{
public:
    CommandRepository(){}
    // register a new command for the factory: throws if a command with the
    // same name already exists...deregister first to replace a command
    void registerCmd(const std::string& name, CommandPtr c)
    {
        if( contains(name) )
        {
            std::ostringstream oss;
            oss << "Command " << name << " already registered";
            throw Exception{ oss.str() };
        }
        else
            repository_.emplace( name, std::move(c) );
    }

    // unregister a command: returns Number of elements removed.
    size_t unregisterCmd(const std::string& name){ return repository_.erase(name); }

    // returns the number of commands currently registered
    size_t size() const { return repository_.size(); }

    // returns a pointer to a command without deregistering the command...returns
    // a nullptr if the command does not exist
    CommandPtr clone(const std::string& name) const {
        if( contains(name) )
        {
            const auto& command = repository_.find(name)->second;
            return MakeCommandPtr( command->clone().release() );
        }
        else {
            throw Exception{ "Command doesn't exist!" };
        }
    }

    // returns true if the command is present, false otherwise
    bool contains(const std::string& s) const { return repository_.find(s) != repository_.end(); }

    // returns a set of all the commands
    std::set<std::string> allCmdNames() const {
        std::set<string> tmp;

        for(const auto& e : repository_)
            tmp.emplace(e.first);

        return tmp;
    }

    // prints help for command
    void printHelp(const std::string& command, std::ostream&) const;

    // clears all commands; mainly needed for testing
    void clear() { repository_.clear(); }
    ~CommandRepository()=default;
private:

    using Repository = unordered_map<string, CommandPtr>;
    Repository repository_;

    CommandRepository(CommandRepository&) = delete;
    CommandRepository(CommandRepository&&) = delete;
    CommandRepository& operator=(CommandRepository&) = delete;
    CommandRepository& operator=(CommandRepository&&) = delete;

};

}
}
#endif

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

#include "CommandManager.hpp"
#include <deque>
#include <vector>
#include <list>
#include "utilities/Command.hpp"

using std::unique_ptr;
using std::make_unique;
using std::deque;
using std::vector;
using std::list;

namespace calculator {
namespace controller {

class CommandManager::CommandManagerImpl
{
public:
    virtual ~CommandManagerImpl(){}

    virtual size_t getUndoSize() const = 0;
    virtual size_t getRedoSize() const = 0;

    virtual void executeCommand(CommandPtr c) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

class CommandManager::UndoRedoStackStrategy : public CommandManager::CommandManagerImpl
{
public:
    size_t getUndoSize() const override { return undoStack_.size(); }
    size_t getRedoSize() const override { return redoStack_.size(); }

    void executeCommand(CommandPtr c) override
    {
        c->execute();

        undoStack_.push_back( std::move(c) );
        flush();
    }

    void undo() override
    {
        if( getUndoSize() == 0 ) return;

        auto& c = undoStack_.back();
        c->undo();

        redoStack_.push_back( std::move(c) );
        undoStack_.pop_back();
    }

    void redo() override
    {
        if( getRedoSize() == 0 ) return;

        auto& c = redoStack_.back();
        c->execute();

        undoStack_.push_back( std::move(c) );
        redoStack_.pop_back();
    }

private:
    void flush() { redoStack_.clear(); }

    deque<CommandPtr> undoStack_;
    deque<CommandPtr> redoStack_;
};


class CommandManager::UndoRedoListStrategyVector : public CommandManager::CommandManagerImpl
{
public:
    UndoRedoListStrategyVector() : cur_{-1}, undoSize_{0}, redoSize_{0} { }

    size_t getUndoSize() const override { return undoSize_;}
    size_t getRedoSize() const override { return redoSize_; }

    void executeCommand(CommandPtr c) override
    {
        c->execute();

        flush();
        undoRedoList_.emplace_back( std::move(c) );
        cur_ = undoRedoList_.size() - 1;
        ++undoSize_;
        redoSize_ = 0;
    }

    void undo() override
    {
        if(getUndoSize() == 0) return;

        undoRedoList_[cur_]->undo();
        --cur_;
        --undoSize_;
        ++redoSize_;
    }

    void redo() override
    {
        if(getRedoSize() == 0) return;

        ++cur_;
        undoRedoList_[cur_]->execute();
        --redoSize_;
        ++undoSize_;
    }

private:
    void flush()
    {
        if(!undoRedoList_.empty()) undoRedoList_.erase(undoRedoList_.begin() + cur_ + 1, undoRedoList_.end());
    }
    int cur_;
    size_t undoSize_;
    size_t redoSize_;
    vector<CommandPtr> undoRedoList_;
};


class CommandManager::UndoRedoListStrategy : public CommandManager::CommandManagerImpl
{
public:
    UndoRedoListStrategy() : undoSize_{0}, redoSize_{0}, cur_{undoRedoList_.end()} { }

    size_t getUndoSize() const override { return undoSize_; }
    size_t getRedoSize() const override { return redoSize_; }

    void executeCommand(CommandPtr c) override
    {
        c->execute();

        flush();
        undoRedoList_.emplace_back( std::move(c) );
        ++undoSize_;
        redoSize_ = 0;
        cur_ = undoRedoList_.end();
        --cur_;
    }

    void undo() override
    {
        if(undoSize_ == 0) return;

        --undoSize_;
        ++redoSize_;
        (*cur_)->undo();
        --cur_;
    }

    void redo() override
    {
        if(redoSize_ == 0) return;

        --redoSize_;
        ++undoSize_;
        ++cur_;
        (*cur_)->execute();
    }

private:
    void flush()
    {
        auto i = cur_;
        ++i;
        if(!undoRedoList_.empty()) undoRedoList_.erase(i, undoRedoList_.end());
    }

    size_t undoSize_;
    size_t redoSize_;
    list<CommandPtr> undoRedoList_;
    list<CommandPtr>::iterator cur_;
};


CommandManager::CommandManager(UndoRedoStrategy st)
{
    switch(st)
    {
    case UndoRedoStrategy::ListStrategy:
        pimpl_ = make_unique<UndoRedoListStrategy>();
        break;

    case UndoRedoStrategy::StackStrategy:
        pimpl_ = make_unique<UndoRedoStackStrategy>();
        break;

    case UndoRedoStrategy::ListStrategyVector:
        pimpl_ = make_unique<UndoRedoListStrategyVector>();
        break;
    }
}

CommandManager::~CommandManager()=default;

size_t CommandManager::getUndoSize() const
{
    return pimpl_->getUndoSize();
}

size_t CommandManager::getRedoSize() const
{
    return pimpl_->getRedoSize();
}

void CommandManager::executeCommand(CommandPtr c)
{
    pimpl_->executeCommand( std::move(c) );

}

void CommandManager::undo()
{
    pimpl_->undo();

}

void CommandManager::redo()
{
    pimpl_->redo();

}

}}

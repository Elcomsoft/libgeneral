//
//  TaggedEvent.cpp
//  libgeneral
//
//  Created by erd on 21.11.24.
//  Copyright © 2024 tihmstar. All rights reserved.
//
#include "../include/libgeneral/macros.h"

#include "../include/libgeneral/TaggedEvent.hpp"

using namespace tihmstar;

TaggedEvent::TaggedEvent()
: _members(0), _isDying(false)
{
    
}

TaggedEvent::~TaggedEvent(){
    assert(_members == 0); //we cannot die if members are still waiting
}

void TaggedEvent::registerForTag(uint64_t tag){
    std::unique_lock<std::mutex> lk(_m);

    retassure(!_isDying, "cannot register on dying object");

    _registrations.insert(tag);
}

void TaggedEvent::waitForTagArrival(uint64_t tag){
    std::unique_lock<std::mutex> lk(_m);

    while (_registrations.find(tag) != _registrations.end()) {
        uint64_t nextEvent = _event.getNextEvent();
        ++_members;
        lk.unlock();
        try {
            _event.waitForEvent(nextEvent);
        } catch (...) {
            _isDying = true;
        }
        lk.lock();
        --_members;
        
        retassure(!_isDying, "object died while waiting on it");
    }
}

void TaggedEvent::notifyWithTag(uint64_t tag){
    std::unique_lock<std::mutex> lk(_m);
    
    retassure(!_isDying, "cannot notify on dying object");
    
    _registrations.erase(tag);
    _event.notifyAll();
}

uint64_t TaggedEvent::getNextEvent(){
    return _event.getNextEvent();
}

void TaggedEvent::waitForEvent(uint64_t event){
    _event.waitForEvent(event);
}

uint64_t TaggedEvent::members() const{
    return _members;
}

void TaggedEvent::kill(){
    std::unique_lock<std::mutex> lk(_m);
    _isDying = true;
    _event.kill();
}

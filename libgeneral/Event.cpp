//
//  Event.cpp
//  libgeneral
//
//  Created by tihmstar on 15.08.20.
//  Copyright © 2020 tihmstar. All rights reserved.
//
#include "../include/libgeneral/macros.h"

#include "../include/libgeneral/Event.hpp"

using namespace tihmstar;

Event::Event()
: _members(0), _curEvent(0), _isDying(false)
{
    
}

Event::~Event(){
    while (_members) {
        //drop all waiting members
        std::unique_lock<std::mutex> lk(_m);
        _cm.wait(lk, [&]{return !_members;});
    }
}

uint64_t Event::getNextEvent(){
    std::unique_lock<std::mutex> lk(_m);
    uint64_t nextEvent = _curEvent+1;
    if (nextEvent == 0) nextEvent++;
    return nextEvent;
}

void Event::waitForEvent(uint64_t event){
    std::unique_lock<std::mutex> lk(_m);
    ++_members;

    _cv.wait(lk, [&]{return _curEvent>=event || _isDying;});
    
    --_members;
    _cm.notify_all();
    retassure(!_isDying, "object died while waiting on it");
}

void Event::notifyAll(){
    std::unique_lock<std::mutex> lk(_m);
    if(++_curEvent == 0) ++_curEvent;
    _cv.notify_all();
}

uint64_t Event::members() const{
    return _members;
}

void Event::kill(){
    std::unique_lock<std::mutex> lk(_m);
    _isDying = true;
    _cv.notify_all();
}

//
//  Event.hpp
//  libgeneral
//
//  Created by tihmstar on 15.08.20.
//  Copyright © 2020 tihmstar. All rights reserved.
//

#ifndef Event_hpp
#define Event_hpp

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <stdint.h>

namespace tihmstar {
    class Event{
        std::mutex _m;
        std::condition_variable _cv;
        std::condition_variable _cm;
        std::atomic<uint64_t> _members;
        uint64_t _curEvent;
        bool _isDying;

    public:
        Event();
        ~Event();
        
        uint64_t getNextEvent();
        void waitForEvent(uint64_t event);
        
        void notifyAll();
        uint64_t members() const;
        
        /*
            Releases waiter and hints destruction of the object in near future
            Further calls to wait() or post() are not allowed after calling kill()
         */
        void kill();
    };
};

#endif /* Event_hpp */

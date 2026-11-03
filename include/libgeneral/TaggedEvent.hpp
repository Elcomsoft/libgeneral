//
//  TaggedEvent.hpp
//  libgeneral
//
//  Created by erd on 21.11.24.
//  Copyright © 2024 tihmstar. All rights reserved.
//

#ifndef TaggedEvent_hpp
#define TaggedEvent_hpp

#include <libgeneral/Event.hpp>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <set>

#include <stdint.h>

namespace tihmstar {
    class TaggedEvent{
        tihmstar::Event _event;
        std::mutex _m;
        std::set<uint64_t> _registrations;
        std::atomic<uint64_t> _members;
        bool _isDying;

    public:
        TaggedEvent();
        ~TaggedEvent();
        
        void registerForTag(uint64_t tag);
        void waitForTagArrival(uint64_t tag);
        
        void notifyWithTag(uint64_t tag);

        uint64_t getNextEvent();
        void waitForEvent(uint64_t event);
        
        uint64_t members() const;
        
        /*
            Releases waiter and hints destruction of the object in near future
            Further calls to wait() or post() are not allowed after calling kill()
         */
        void kill();
    };
};
#endif /* TaggedEvent_hpp */

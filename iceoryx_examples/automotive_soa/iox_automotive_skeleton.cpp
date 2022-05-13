// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0


#include "iceoryx_hoofs/posix_wrapper/signal_watcher.hpp"

//#include "owl/com/event_publisher.hpp"
#include "minimal_skeleton.hpp"
#include "owl/runtime.hpp"

#include <iostream>

using namespace owl;

constexpr char APP_NAME[] = "iox-cpp-automotive-skeleton";

int main()
{
    Runtime::GetInstance(APP_NAME);

    kom::InstanceIdentifier instanceIdentifier{iox::cxx::TruncateToCapacity, "Example"};
    MinimalSkeleton skeleton{instanceIdentifier};

    skeleton.OfferService();

    uint32_t counter = 0;
    while (!iox::posix::hasTerminationRequested())
    {
        ++counter;

        // Event
        auto sample = skeleton.m_event.Allocate();
        (*sample).counter = counter;
        (*sample).sendTimestamp = std::chrono::steady_clock::now();
        skeleton.m_event.Send(std::move(sample));
        std::cout << "Event: value " << counter << " sent" << std::endl;

        // Field
        if (counter > 30)
        {
            Topic field{counter};
            skeleton.m_field.Update(field);
            std::cout << "Field: updated value to " << counter << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    skeleton.StopOfferService();

    return 0;
}

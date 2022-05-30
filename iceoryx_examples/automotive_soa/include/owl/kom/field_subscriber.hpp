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

#ifndef IOX_EXAMPLES_AUTOMOTIVE_SOA_FIELD_SUBSCRIBER_HPP
#define IOX_EXAMPLES_AUTOMOTIVE_SOA_FIELD_SUBSCRIBER_HPP

#include "iceoryx_posh/popo/client.hpp"
#include "iceoryx_posh/popo/subscriber.hpp"

#include "owl/types.hpp"

#include <limits>

namespace owl
{
namespace kom
{
template <typename T>
class FieldSubscriber
{
  public:
    using FieldType = T;
    static constexpr uint64_t QUEUE_CAPACITY{1U};
    static constexpr uint64_t HISTORY_REQUEST{1U};

    FieldSubscriber(const core::String& service, const core::String& instance, const core::String& event) noexcept;
    ~FieldSubscriber() noexcept;

    FieldSubscriber(const FieldSubscriber&) = delete;
    FieldSubscriber(FieldSubscriber&&) = delete;
    FieldSubscriber& operator=(const FieldSubscriber&) = delete;
    FieldSubscriber& operator=(FieldSubscriber&&) = delete;

    template <typename Callable>
    owl::core::Result<size_t> GetNewSamples(Callable&& callable,
                                            size_t maxNumberOfSamples = std::numeric_limits<size_t>::max()) noexcept;

    Future<FieldType> Get() noexcept;
    Future<FieldType> Set(const FieldType& value) noexcept;

  private:
    Future<FieldType> receiveResponse() noexcept;

    iox::popo::Subscriber<FieldType> m_subscriber;
    iox::popo::Client<iox::cxx::optional<FieldType>, FieldType> m_client;
    std::atomic<int64_t> m_sequenceId{0};
    iox::popo::WaitSet<> m_waitset;
    static constexpr bool isRecursive{true};
    iox::posix::mutex m_mutex{isRecursive};
};

} // namespace kom
} // namespace owl

#include "owl/kom/field_subscriber.inl"

#endif // IOX_EXAMPLES_AUTOMOTIVE_SOA_FIELD_SUBSCRIBER_HPP

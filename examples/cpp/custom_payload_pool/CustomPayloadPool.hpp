// Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/**
 * @file CustomPayloadPool.hpp
 */

#ifndef _FASTDDS_CUSTOM_PAYLOAD_POOL_HPP_
#define _FASTDDS_CUSTOM_PAYLOAD_POOL_HPP_

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <fastdds/rtps/history/IPayloadPool.hpp>
#include <fastdds/rtps/common/SerializedPayload.hpp>

class CustomPayloadPool : public eprosima::fastdds::rtps::IPayloadPool
{
public:

    ~CustomPayloadPool() = default;

    bool get_payload(
            unsigned int size,
            eprosima::fastdds::rtps::SerializedPayload_t& payload)
    {
        // Reserve new memory for the payload buffer
        unsigned char* payload_buff = new unsigned char[size];

        // Assign the payload buffer to the SerializedPayload and update sizes
        payload.data = payload_buff;
        payload.length = size;
        payload.max_size = size;

        // Tell the SerializedPayload who needs to release its payload
        payload.payload_owner = this;

        return true;
    }

    bool get_payload(
            const eprosima::fastdds::rtps::SerializedPayload_t& data,
            eprosima::fastdds::rtps::SerializedPayload_t& payload)
    {
        // Reserve new memory for the payload buffer
        unsigned char* payload_buff = new unsigned char[data.length];

        // Copy the data
        memcpy(payload_buff, data.data, data.length);

        // Tell the SerializedPayload who needs to release its payload
        payload.payload_owner = this;

        // Assign the payload buffer to the SerializedPayload and update sizes
        payload.data = payload_buff;
        payload.length = data.length;
        payload.max_size = data.length;

        return true;
    }

    bool release_payload(
            eprosima::fastdds::rtps::SerializedPayload_t& payload)
    {
        // Ensure precondition
        if (this != payload.payload_owner)
        {
            std::cerr << "Trying to release a payload buffer allocated by a different PayloadPool." << std::endl;
            return false;
        }

        // Dealloc the buffer of the payload
        delete[] payload.data;

        // Reset sizes and pointers
        payload.data = nullptr;
        payload.length = 0;
        payload.max_size = 0;

        // Reset the owner of the payload
        payload.payload_owner = nullptr;

        return true;
    }

};

#endif  // _FASTDDS_CUSTOM_PAYLOAD_POOL_HPP_

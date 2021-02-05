/*
 * Copyright (C) 2020 taylor.fish <contact@taylor.fish>
 *
 * This file is part of jack-send-midi.
 *
 * jack-send-midi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * jack-send-midi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with jack-send-midi.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "jack-send-midi.h"
#include <algorithm>
#include <atomic>
#include <cinttypes>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <memory>

static constexpr std::size_t max_message_length = 3;

static constexpr bool debug =
    #ifdef NDEBUG
        false
    #else
        true
    #endif
;

struct Message {
    Message* next;
    unsigned char data[max_message_length];
    std::uint_least8_t length;

    friend std::ostream& operator<<(
        std::ostream& stream, const Message& self
    ) {
        stream << "[";
        for (std::size_t i = 0; i < self.length; ++i) {
            if (i > 0) {
                stream << ", ";
            }
            stream << static_cast<int>(self.data[i]);
        }
        stream << "]";
        return stream;
    }
};

class JackClientDeleter {
    public:
    void operator()(jack_client_t* client) const {
        jack_client_close(client);
    }
};

using UniqueJackClient = std::unique_ptr<jack_client_t, JackClientDeleter>;

class Client {
    public:
    Client(UniqueJackClient client, jack_port_t* port) :
        m_client(client.release()),
        m_port(port)
    {
    }

    ~Client() {
        // First, close the JACK client.
        UniqueJackClient{m_client};
        free_messages(m_head.load(std::memory_order_relaxed));
    }

    void send_message(unsigned char* data, std::size_t length) {
        auto message = std::make_unique<Message>(Message {
            /* .next = */ m_head.load(),
            /* .data = */ {},
            /* .length = */ static_cast<std::uint_least8_t>(
                std::min(length, max_message_length)
            ),
        }).release();
        std::memcpy(
            message->data,
            data,
            message->length
        );
        if constexpr (debug) {
            std::cerr << "[jack-send-midi] sending message: ";
            std::cerr << *message << "\n";
        }
        m_head.store(message, std::memory_order_relaxed);
        gc();
    }

    int process(jack_nframes_t nframes) {
        auto head = m_head.load(std::memory_order_relaxed);
        auto end = m_last_processed.load(std::memory_order_relaxed);
        auto buffer = jack_port_get_buffer(m_port, nframes);
        jack_midi_clear_buffer(buffer);

        auto message = head;
        while (message != end) {
            // This function could return an error if there's not enough
            // space in the buffer, but we'll just ignore it.
            jack_midi_event_write(
                buffer,
                0,
                static_cast<jack_midi_data_t*>(message->data),
                message->length
            );
            message = message->next;
        }
        m_last_processed.store(head, std::memory_order_release);
        return 0;
    }

    private:
    jack_client_t* m_client = nullptr;
    jack_port_t* m_port = nullptr;
    std::atomic<Message*> m_head = nullptr;
    std::atomic<Message*> m_last_processed = nullptr;

    void gc() {
        auto message = m_last_processed.load(std::memory_order_acquire);
        if (!message) {
            return;
        }

        // Detach m_last_processed from its next message.
        auto next = message->next;
        message->next = nullptr;
        free_messages(next);
    }

    static void free_messages(Message* head) {
        while (head) {
            auto next = head->next;
            if constexpr (debug) {
                std::cerr << "[jack-send-midi] freeing message: ";
                std::cerr << *head << "\n";
            }
            std::unique_ptr<Message>{head};
            head = next;
        }
    }
};

static int client_process(jack_nframes_t nframes, void* arg) {
    return static_cast<Client*>(arg)->process(nframes);
}

__attribute__((visibility("default")))
Client* client_new(const char* name, Status* status, int* jack_status) {
    // Reset statuses.
    if (status) *status = STATUS_OK;
    if (jack_status) *jack_status = 0;

    // Open the client.
    jack_status_t real_jack_status{};
    jack_client_t* raw_client = jack_client_open(
        name,
        JackNullOption,
        &real_jack_status
    );
    if (jack_status) *jack_status = real_jack_status;
    if (!raw_client) {
        if (status) *status = STATUS_CLIENT_OPEN_FAILED;
        return nullptr;
    }

    // The JACK client will be closed when this object is destroyed.
    UniqueJackClient jack_client(raw_client);

    // Register the MIDI output port.
    jack_port_t* port = jack_port_register(
        raw_client,
        "events-out",
        JACK_DEFAULT_MIDI_TYPE,
        JackPortIsOutput,
        0
    );
    if (!port) {
        if (status) *status = STATUS_PORT_REGISTER_FAILED;
        return nullptr;
    }

    // Set the process callback.
    auto client = std::make_unique<Client>(std::move(jack_client), port);
    if (
        jack_set_process_callback(
            raw_client,
            client_process,
            static_cast<void*>(client.get())
        ) != 0
    ) {
        if (status) *status = STATUS_SET_CALLBACK_FAILED;
        return nullptr;
    }

    // Activate the client.
    if (jack_activate(raw_client) != 0) {
        if (status) *status = STATUS_ACTIVATE_FAILED;
        return nullptr;
    }
    return client.release();
}

__attribute__((visibility("default")))
void client_destroy(Client* client) {
    std::unique_ptr<Client>{client};
}

__attribute__((visibility("default")))
void client_send_message(
    Client* client,
    unsigned char* data,
    std::size_t length
) {
    client->send_message(data, length);
}

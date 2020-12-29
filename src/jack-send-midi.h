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

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Client Client;

typedef enum Status {
    STATUS_OK = 0,
    STATUS_CLIENT_OPEN_FAILED = 1,
    STATUS_PORT_REGISTER_FAILED = 2,
    STATUS_SET_CALLBACK_FAILED = 3,
    STATUS_ACTIVATE_FAILED = 4,
} Status;

Client* client_new(const char* name, Status* status, int* jack_status);

void client_destroy(Client* client);

void client_send_message(
    Client* client,
    unsigned char* data,
    size_t length
);

#ifdef __cplusplus
}
#endif

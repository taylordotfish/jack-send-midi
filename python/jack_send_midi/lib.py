# Copyright (C) 2020 taylor.fish <contact@taylor.fish>
#
# This file is part of jack-send-midi.
#
# jack-send-midi is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# jack-send-midi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with jack-send-midi.  If not, see <https://www.gnu.org/licenses/>.

from typing import Union
import ctypes


class Client:
    def __init__(self, name: Union[bytes, str]):
        if type(name) is str:
            name = name.encode()
        status = ctypes.c_int()
        jack_status = ctypes.c_int()
        self._client = ctypes.c_void_p(
            clib.client_new(
                ctypes.c_char_p(name),
                ctypes.byref(status),
                ctypes.byref(jack_status),
            )
        )
        if self._client is None:
            raise NewClientError(int(status), int(jack_status))

    def send_message(self, data: bytes):
        if len(data) > 3:
            raise ValueError("midi messages cannot be longer than 3 bytes")
        if self._client is None:
            raise ValueError("client was closed")
        clib.client_send_message(
            self._client,
            ctypes.c_char_p(data),
            ctypes.c_size_t(len(data)),
        )

    def close(self):
        if self._client is not None:
            clib.client_destroy(self._client)
            self._client = None

    def __enter__(self):
        return self

    def __exit__(self):
        self.close()

    def __del__(self):
        self.close()


class NewClientError(Exception):
    def __init__(self, status: int, jack_status: int):
        self.status = status
        self.jack_status = jack_status

    def __str__(self):
        MESSAGES = [
            # 0
            lambda: "unknown error",
            # 1
            lambda:  "could not create jack client "
                    f"[jack status: {self.jack_status}]",
            # 2
            lambda: "could not register midi port",
            # 3
            lambda: "could not register process callback",
            # 4
            lambda: "could not activate client",
        ]
        return MESSAGES[
            self.status if 0 <= self.status < len(MESSAGES) else 0
        ]()


clib = ctypes.cdll.LoadLibrary("libjack-send-midi.so.0")

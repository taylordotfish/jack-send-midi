#!/usr/bin/env python3
# To the extent possible under law, the author(s) have dedicated all
# copyright and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty. See
# <http://creativecommons.org/publicdomain/zero/1.0/> for a copy of the
# CC0 Public Domain Dedication.
#
# Note that the above copyright notice applies only to the code in this
# file: jack-send-midi, which this code depends on, is licensed under version
# 3 or later of the GNU General Public License. Thus, any version of this
# code that links to or is otherwise a derived work of jack-send-midi may be
# distributed only in accordance with jack-send-midi's license.

from jack_send_midi import Client
import time


# For more information about MIDI messages, see:
# https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

def note_on(pitch, velocity=64, channel=0):
    return bytes([0b10010000 | channel, pitch, velocity])


def note_off(pitch, velocity=0, channel=0):
    return bytes([0b10000000 | channel, pitch, velocity])


PITCHES = [50, 53, 57, 60]  # Dm7
client = Client(name="example-client")

while True:
    time.sleep(0.8)
    for pitch in PITCHES:
        client.send_message(note_on(pitch, velocity=50))

    time.sleep(0.2)
    for pitch in PITCHES:
        client.send_message(note_off(pitch))

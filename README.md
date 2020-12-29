jack-send-midi
==============

Send realtime-safe MIDI events with JACK, even from non-realtime-safe contexts.

Installation
------------

libjack must be installed with the development files. (On Debian with JACK2,
install `libjack-jackd2-dev`.)

```bash
make
sudo make install
```

To install the Python bindings (requires Python 3.7 or later and pip):

```bash
make install-python
```

Usage
-----

See [example.py](example.py) for an example. For more information, see
[jack-send-midi.h](src/jack-send-midi.h) and
[jack\_send\_midi/lib.py](python/jack_send_midi/lib.py).

License
-------

jack-send-midi is licensed under version 3 or later of the GNU General Public
License.

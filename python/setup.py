#!/usr/bin/env python3
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

from setuptools import setup

setup(
    name="jack-send-midi",
    version="0.1.3",
    description="Send realtime-safe MIDI events with JACK.",
    author="taylor.fish",
    author_email="contact@taylor.fish",
    license="GNU General Public License v3 or later (GPLv3+)",
    packages=["jack_send_midi"],
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Topic :: Multimedia :: Sound/Audio :: MIDI",
        "License :: OSI Approved :: "
        "GNU Lesser General Public License v3 or later (LGPLv3+)",
        "Operating System :: POSIX :: Linux",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
    ],
)

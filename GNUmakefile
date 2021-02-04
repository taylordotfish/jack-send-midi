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

BUILD_DIR = build

# Set to 1 for debug mode. The compiled library may not be real-time-safe.
DEBUG = 0

NAME = jack-send-midi
LIB_NAME = lib$(NAME).so
ABI_VERSION = 0
HEADER = $(NAME).h

BINARY = $(LIB_NAME)
OBJECTS = $(addsuffix .o,$(basename $(shell \
	find src -type f -name '*.cpp' ! -name main.cpp \
)))



CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++17 -fpic -MMD -MP -Isrc \
           -fvisibility=hidden
LDFLAGS = -shared -Wl,--no-undefined,--no-allow-shlib-undefined \
          -Wl,-soname,$(LIB_NAME).$(ABI_VERSION)
LDLIBS = -ljack



ifeq ($(DEBUG), 1)
	CXXFLAGS += -Og -ggdb
else
	CXXFLAGS += -Ofast -flto -fwhole-program -DNDEBUG
	LDFLAGS += -Ofast -flto
endif



BINARY := $(addprefix $(BUILD_DIR)/,$(BINARY))
OBJECTS := $(addprefix $(BUILD_DIR)/,$(OBJECTS))

BUILD_SUBDIRS = $(sort $(dir $(OBJECTS)))
BUILD_DIRS = $(BUILD_DIR) $(BUILD_SUBDIRS)

$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<



.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(OBJECTS:.o=.d)

$(BINARY) $(OBJECTS): | $(BUILD_DIRS)

$(BUILD_DIRS):
	mkdir -p $@

.PHONY: install
install: $(BINARY) uninstall
	mkdir -p /usr/local/lib
	mkdir -p /usr/local/include
	cp $(BINARY) /usr/local/lib/$(LIB_NAME).$(ABI_VERSION)
	ln -s $(LIB_NAME).$(ABI_VERSION) /usr/local/lib/$(LIB_NAME)
	cp -r src/$(HEADER) /usr/local/include/
	ldconfig

.PHONY: uninstall
uninstall:
	rm -f /usr/local/lib/$(LIB_NAME)
	rm -f /usr/local/lib/$(LIB_NAME).*
	rm -f /usr/local/include/$(HEADER)

.PHONY: install-python
install-python:
	pip3 install ./python

.PHONY: uninstall-python
uninstall-python:
	pip3 uninstall $(NAME)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean-binary
clean-binaries:
	rm -rf $(BINARY)

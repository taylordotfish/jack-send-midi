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

from .lib import Client
from .lib import NewClientError

__all__ = ["Client", "NewClientError"]

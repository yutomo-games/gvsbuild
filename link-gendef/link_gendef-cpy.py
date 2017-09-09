#!/usr/bin/python3
#
# Copyright (C) 2017 - Daniele Forghieri
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.

# Simple script to copy the linker wrapper in the correct directory

from sys import argv
from shutil import copyfile
from os.path import join

if len(argv) == 3:
    copyfile(argv[1], join(argv[2], 'link.exe'))


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

#
# Simple link/gendef replacement, integrated in one single program.
#
# You have to compile this program and copy it in your meson build
# directory with the name 'link.exe'
#
# In your meson script for the .DLL definition file instead of using
# the option '/DEF:file_name' use '/_MK_DEF:file_name'
#
# This program scan the link command used, get the dll name, the def
# file name and all the objects used and create the update def file
# using the utility dumpbin.
#
# After this the original link command (searched in the path) is called
# changin the /_MK_DEF option to /DEF
#
# The net result is that all the definitions presents in the various
# object file are exported and there is no need to declare them
# __declspec(dllexport), emulating gcc and the cmake's
# WINDOWS_EXPORT_ALL_SYMBOLS
#

project('link-gendef', 'cpp',
        version: '0.1.0',
        default_options: [
          'buildtype=debugoptimized',
          'warning_level=1',
        ],
        meson_version : '>= 0.42')

my_link = executable('link_gendef',
                     'link_gendef.cpp',
                     install: true,
                    )

install_data('link_gendef-cpy.py',
             install_dir: join_paths(get_option('prefix'), 'bin')
            )

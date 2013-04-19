# Makefile.am - Build logfilter
#
# Copyright (c) 2006 by Educational Technology Resources, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License. 
# You may obtain a copy of the License at 
#
# http://www.apache.org/licenses/LICENSE-2.0 
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
# See the License for the specific language governing permissions and 
# limitations under the License.

# Main targets
bin_PROGRAMS=logfilter

# Sources for logfilter
logfilter_SOURCES=\
	buffer.c buffer.h \
	entry.c entry.h \
	entry_test.c entry_test.h \
	error.c error.h \
	filter.c filter.h \
	filter_test.c filter_test.h \
	main.c \
	minunit.c minunit.h

# Libraries needed by logfilter
logfilter_LDFLAGS=-lpcreposix -lpcre

# Additional files to add to tarball
EXTRA_DIST=bootstrap LICENSE NOTICE


# Utility targets
tags ctags:
	ctags *.[ch]


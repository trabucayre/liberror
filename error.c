/* 
 * Copyright (C) 2010 Chris McClelland
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

// Code inspired by http://linux.die.net/man/3/snprintf
//
const char *renderError(const char *format, ...) {
	// Guess we need no more than 512 bytes
	int returnCode, size = 512;
	char *bufPtr, *newBufPtr;
	va_list argList;

	bufPtr = malloc(size);
	if ( bufPtr == NULL ) {
		// Insufficient memory
		return NULL;
	}
	for ( ; ; ) {
		// Try to print in the allocated space
		va_start(argList, format);
		returnCode = vsnprintf(bufPtr, size, format, argList);
		va_end(argList);

		// If that worked, return the string
		if ( returnCode != -1 && returnCode < size ) {
			return bufPtr;
		}

		// Else try again with more space
		if ( returnCode == -1 ) {
			// glibc 2.0 and MSVC do not comply with C99 and return -1 if there's not enough space
			size *= 2;
		} else {
			// glibc 2.1 has a C99-compliant vsnprintf(), so it returns the number of bytes needed
			size = returnCode + 1;
		}
		newBufPtr = realloc(bufPtr, size);
		if ( newBufPtr == NULL ) {
			free(bufPtr);
			return NULL;
		} else {
			bufPtr = newBufPtr;
		}
	}
}
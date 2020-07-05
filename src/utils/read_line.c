/*	$NetBSD: getline.c,v 1.2 2014/09/16 17:23:50 christos Exp $	*/

/*-
 * Copyright (c) 2011 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long
read_line(char **lineptr, size_t *n, FILE *stream)
{
	assert(lineptr);
	assert(n);
#ifdef __unix__
	return getline(lineptr, n, stream);
#else
#define BUFSIZE 32
	char *ptr, *eptr;
	if (*lineptr == NULL || *n == 0) {
		*n = BUFSIZE;
		if ((*lineptr = malloc(*n)) == NULL)
			return -1;
	}
	for (ptr = *lineptr, eptr = *lineptr + *n;;) {
		int c = fgetc(stream);
		if (c == -1) {
			if (feof(stream)) {
				long diff = ptr - *lineptr;
				if (diff != 0) {
					*ptr = '\0';
					return diff;
				}
			}
			return -1;
		}
		*ptr++ = c;
		if (c == '\n') {
			*ptr = '\0';
			return ptr - *lineptr;
		}
		if (ptr + 2 >= eptr) {
			char *nbuf;
			size_t nbufsiz = *n * 2;
			long d = ptr - *lineptr;
			if ((nbuf = realloc(*lineptr, nbufsiz)) == NULL)
				return -1;
			*lineptr = nbuf;
			*n = nbufsiz;
			eptr = nbuf + nbufsiz;
			ptr = nbuf + d;
		}
	}
#endif
}

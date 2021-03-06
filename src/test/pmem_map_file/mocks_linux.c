/*
 * Copyright 2015-2017, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * mocks_linux.c -- mocked functions used in pmem_map_file.c (Linux-specific)
 */

#define _GNU_SOURCE
#include "unittest.h"
#include <dlfcn.h>

#define MAX_LEN (4 * 1024 * 1024)

/*
 * posix_fallocate -- interpose on libc posix_fallocate()
 */
int
posix_fallocate(int fd, os_off_t offset, off_t len)
{
	UT_OUT("posix_fallocate: off %ju len %ju", offset, len);

	static int (*posix_fallocate_ptr)(int fd, os_off_t offset, off_t len);

	if (posix_fallocate_ptr == NULL)
		posix_fallocate_ptr = dlsym(RTLD_NEXT, "posix_fallocate");

	if (len > MAX_LEN) {
		errno = ENOSPC;
		return -1;
	}
	return (*posix_fallocate_ptr)(fd, offset, len);
}

/*
 * ftruncate -- interpose on libc ftruncate()
 */
int
ftruncate(int fd, os_off_t len)
{
	UT_OUT("ftruncate: len %ju", len);

	static int (*ftruncate_ptr)(int fd, os_off_t len);

	if (ftruncate_ptr == NULL)
		ftruncate_ptr = dlsym(RTLD_NEXT, "ftruncate");

	return (*ftruncate_ptr)(fd, len);
}

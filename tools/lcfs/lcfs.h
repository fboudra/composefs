/* lcfs
   Copyright (C) 2021 Giuseppe Scrivano <giuseppe@scrivano.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef _LCFS_H
#define _LCFS_H
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define LCFS_VERSION 1

#define LCFS_USE_TIMESPEC 0

typedef uint32_t lcfs_off_t;

typedef lcfs_off_t lcfs_c_str_t;

struct lcfs_vdata_s {
	lcfs_off_t off;
	lcfs_off_t len;
} __attribute__((packed));

struct lcfs_header_s {
	uint8_t version;
	uint8_t unused1;
	uint16_t unused2;
	uint32_t unused3;
} __attribute__((packed));

struct lcfs_inode_data_s {
	uint32_t st_mode; /* File type and mode.  */
	uint32_t st_nlink; /* Number of hard links.  */
	uint32_t st_uid; /* User ID of owner.  */
	uint32_t st_gid; /* Group ID of owner.  */
	uint32_t st_rdev; /* Device ID (if special file).  */
} __attribute__((packed));

struct lcfs_inode_s {
	/* Index of struct lcfs_inode_data_s. */
	lcfs_off_t inode_data_index;

	/* stat data.  */
	union {
		/* Offset and length to the content of the directory.  */
		struct {
			lcfs_off_t off;
			lcfs_off_t len;
		} dir;

		struct {
			/* Total size, in bytes.  */
			uint64_t st_size;
			lcfs_c_str_t payload;
		} file;
	} u;

#if LCFS_USE_TIMESPEC
	struct timespec st_mtim; /* Time of last modification.  */
	struct timespec st_ctim; /* Time of last status change.  */
#else
	uint64_t st_mtim; /* Time of last modification.  */
	uint64_t st_ctim; /* Time of last modification.  */
#endif

	/* Variable len data.  */
	struct lcfs_vdata_s xattrs;

} __attribute__((packed));

struct lcfs_dentry_s {
	/* Index of struct lcfs_inode_s */
	lcfs_off_t inode_index;

	/* Variable len data.  */
	lcfs_c_str_t name;

} __attribute__((packed));

/* xattr representation.  */
struct lcfs_xattr_header_s {
	struct lcfs_vdata_s key;
	struct lcfs_vdata_s value;
} __attribute__((packed));

#endif
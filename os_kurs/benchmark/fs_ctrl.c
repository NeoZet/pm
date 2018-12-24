#define _GNU_SOURCE
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>

#include "fs_ctrl.h"


static char *create_name(struct fs_dir *d, const char* key)
{
	char *buf = NULL;
	assert(d != NULL && key != NULL);
	if (-1 == sprintf(buf, "%s/%s", d->path, key))
		return NULL;
	return buf;
}

int fs_open(struct fs_dir *d)
{
	int ret = 0;
	if (d == NULL) {
		ret = -1;
		goto out;
	}
	d->fd = open(d->path, O_DIRECTORY);

	if (d->fd == -1) {
		ret = -2;
		goto out;
	}
out:
	return ret;	
}

int fs_close(struct fs_dir *d)
{
	int ret = 0;
	if (d == NULL) {
		ret = -1;
		goto out;
	}

	ret = close(d->fd);
	if (ret == -1) {
		ret = -2;
		goto out;
	}
out:
	return ret;
}

int fs_start(struct fs_dir *d) {
	int ret = 0;
	ret = fs_open(d);
	return ret;
}

int fs_stop(struct fs_dir *d) {
	int ret = 0;
	ret = fs_close(d);
	return ret;
}


static int mkpath(const char* path, mode_t mode)
{
	int ret = 0;
	struct stat stat_buffer;
	
	if (path == NULL) {
		ret = -1;
		return ret;
	}
	if (stat(path, &stat_buffer) == 0) {
		return ret;
	}

	mkpath(dirname(strdupa(path)), mode);

	if (mkdir(path, mode) == -1) {
		ret = -2;
	}	
	return ret;
}

int fs_put(struct fs_dir *d, const char *k, const char *v)
{
	int ret = 0;
	struct stat sb;
	char * key_file_name = NULL;
	int value_fd = -1;
	
	key_file_name = create_name(d, k);
	if (key_file_name == NULL) {
		ret = -3;
		goto out;
	}
	if (d == NULL || k == NULL || v == NULL) {
		ret = -1;
		goto out;
	}

	if (stat(dirname(strdupa(key_file_name)), &sb) == -1) {
		ret = mkpath(dirname(strdupa(key_file_name)), d->mode);
		if (ret != 0) {
			goto out;
		}
	}
	value_fd = open(key_file_name, O_WRONLY | O_TRUNC | O_CREAT,
			d->mode);

	if (value_fd == -1) {
		ret = -2;
		goto out;
	}
	ssize_t need = strlen(v);

	if (v[need-1] != '\n') {
		char *buf = alloca(need+2);
		memcpy(buf, v, need);
		buf[need] = '\n';
		buf[need+1] = '\0';
		v = buf;
		++need;
	}
	ssize_t written = write(value_fd, v, need);
	if ( written != need) {
		ret = -2;
		goto close_file;
	}

close_file:
	close(value_fd);
out:
	free(key_file_name);
	return ret;
}


int fs_get(struct fs_dir *d, const char *k, char *v)
{
	int ret = 0;
	char * key_file_name = NULL;
	int value_fd = -1;

	key_file_name = create_name(d,k);
	if (key_file_name == NULL) {
		ret = -3;
		goto out;
	}

	if (d == NULL || k == NULL || v == NULL) {
		ret = -1;
		goto out;
	}

	value_fd = open(key_file_name, S_IRUSR | S_IWUSR);

	if (value_fd == -1) {
		ret = -2;
		goto out;
	}
	ssize_t bytes_read = read(value_fd, v, MAX_BYTES_READ);

	if (bytes_read == -1) {
			ret = -2;
			goto close_file;
	}
	if (bytes_read && v[bytes_read - 1] == '\n') {
		v[bytes_read - 1] = '\0';
	}

close_file:
	close(value_fd);
out:
	free(key_file_name);
	return ret;
}


int fs_rm(struct fs_dir *d, const char *k, int depth)
{
	int ret = 0;
	struct stat sb;
	char * key_file_name = NULL;
	char * dir_name = NULL;

	if (d == NULL || k == NULL) {
		ret = -1;
		goto out;
	}

        key_file_name = create_name(d, k);

	if (key_file_name == NULL) {
		ret = -3;
		goto out;
	}

	stat(key_file_name, &sb);
	if (S_ISREG(sb.st_mode)) {
		if (remove(key_file_name) == -1) {
			ret = -2;
			goto out;
		}
		--depth;
	}
	dir_name = S_ISDIR(sb.st_mode) ? key_file_name : strdupa(dirname(strdupa(key_file_name)));

	while(depth>0) {
		if (stat(dir_name, &sb) == 0) {
			if (rmdir(dir_name) == -1) {
				ret = -2;
				goto out;
			}
			dir_name = strdupa(dirname(strdupa(dir_name)));
		}
		--depth;
	}

out:
	free(key_file_name);
	return ret;
}

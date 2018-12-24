#pragma once

struct fs_dir {
	const char *path;
	int fd;
	mode_t mode;
};

#define MAX_BYTES_READ 1024

int fs_start(struct fs_dir *d);
int fs_stop(struct fs_dir *d);
int fs_open(struct fs_dir *d);
int fs_close(struct fs_dir *d);
int fs_put(struct fs_dir *d, const char *k, const char *v);
int fs_get(struct fs_dir *d, const char *k, char *v);
int fs_rm(struct fs_dir *d, const char *k, int depth);

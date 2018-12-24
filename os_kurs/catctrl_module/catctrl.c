#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/pagemap.h> 	/* PAGE_CACHE_SIZE */
#include <linux/fs.h>     	/* This is where libfs stuff is declared */
#include <asm/atomic.h>
#include <asm/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MAXIM SMIRNOV");

#define LFS_MAGIC 0x20180928


static struct inode *lfs_make_inode(struct super_block *sb, int mode)
{
	struct inode *ret = new_inode(sb);

	if (ret) {
		ret->i_mode = mode;
		ret->i_uid.val = ret->i_gid.val = 0;
		ret->i_blocks = 0;
		ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
	}
	return ret;
}


static int lfs_open(struct inode *inode, struct file *filp)
{
	filp->private_data = inode->i_private;
	return 0;
}

#define TMPSIZE 20

static ssize_t lfs_read_cpus_file(struct file *filp, char *buf,
		size_t count, loff_t *offset)
{
	char *counter = (char*) filp->private_data;
	int len;
	char tmp[TMPSIZE];
	len = snprintf(tmp, TMPSIZE, "%s", counter);
	if (count > len - *offset)
		count = len - *offset;

	if (copy_to_user(buf, tmp, count))
		return -EFAULT;
	*offset += count;
	return count;
}

static ssize_t lfs_read_cache_file(struct file *filp, char *buf,
		size_t count, loff_t *offset)
{
	char *counter = (char*) filp->private_data;
	int len;
	char tmp[TMPSIZE];
	len = snprintf(tmp, TMPSIZE, "%s", counter);
	if (count > len - *offset)
		count = len - *offset;

	if (copy_to_user(buf, tmp, count))
		return -EFAULT;
	*offset += count;
	return count;
}


static ssize_t lfs_read_tasks_file(struct file *filp, char *buf,
		size_t count, loff_t *offset)
{
	atomic_t *counter = (atomic_t*) filp->private_data;
	int len, v;
	char tmp[TMPSIZE];
	v = atomic_read(counter);
	len = snprintf(tmp, TMPSIZE, "%d\n", v);
	if (count > len - *offset)
		count = len - *offset;

	if (copy_to_user(buf, tmp, count))
		return -EFAULT;
	*offset += count;
	return count;
}

static ssize_t lfs_write_cpus_file(struct file *filp, const char *buf,
		size_t count, loff_t *offset)
{
	char tmp[TMPSIZE];
	char *counter = (char*) filp->private_data;

	long int cpus_number = 0;
	memset(tmp, 0, TMPSIZE);
	if (copy_from_user(tmp, buf, count)) {
		return -EFAULT;
	}
	if (kstrtoul(tmp, 10, &cpus_number)) {
		return -EFAULT;
	}

	/*
	                    cpus mask: 
	  cpu_id  : 3     2     1     0 
	  mask    : 1     1     1     1
	  hex_mask: f
	 */
	if (cpus_number > ((1 << nr_cpu_ids) - 1)) {
		return -EINVAL;
	}
	
	if (*offset != 0) {
		return -EINVAL;
	}
	
	if (count >= TMPSIZE) {
		return -EINVAL;
	} 	

	memcpy(counter, tmp, TMPSIZE);
	return count;
}


static ssize_t lfs_write_cache_file(struct file *filp, const char *buf,
		size_t count, loff_t *offset)
{
	char tmp[TMPSIZE];
	char *counter = (char*) filp->private_data;

	memset(tmp, 0, TMPSIZE);
	if (copy_from_user(tmp, buf, count)) {
		return -EFAULT;
	}

	if (cpus_number > ((1 << nr_cpu_ids) - 1)) {
		return -EINVAL;
	}
	
	if (*offset != 0) {
		return -EINVAL;
	}
	
	if (count >= TMPSIZE) {
		return -EINVAL;
	} 	

	memcpy(counter, tmp, TMPSIZE);
	return count;
}


static ssize_t lfs_write_tasks_file(struct file *filp, const char *buf,
		size_t count, loff_t *offset)
{
	atomic_t *counter = (atomic_t *) filp->private_data;
	char tmp[TMPSIZE];
	long int task;
	if (*offset != 0)
		return -EINVAL;

	if (count >= TMPSIZE)
		return -EINVAL;
	memset(tmp, 0, TMPSIZE);
	if (copy_from_user(tmp, buf, count))
		return -EFAULT;

	if (kstrtoul(tmp, 10, &task)) {
		return -EFAULT;
	}
	atomic_set(counter, task);
	return count;
}


static struct file_operations lfs_cpus_file_ops = {
	.open	= lfs_open,
	.read 	= lfs_read_cpus_file,
	.write  = lfs_write_cpus_file
};

static struct file_operations lfs_cache_file_ops = {
	.open	= lfs_open,
	.read 	= lfs_read_cache_file,
	.write  = lfs_write_cache_file
};

static struct file_operations lfs_tasks_file_ops = {
	.open	= lfs_open,
	.read 	= lfs_read_tasks_file,
	.write  = lfs_write_tasks_file
};


static struct dentry *lfs_create_file (struct super_block *sb,
		struct dentry *dir, const char *name,
		char *value)
{
	struct dentry *dentry;
	struct inode *inode;
	struct qstr qname;

	qname.name = name;
	qname.len = strlen (name);
	qname.hash = full_name_hash(name, qname.len);

	dentry = d_alloc(dir, &qname);
	if (! dentry)
		goto out;
	inode = lfs_make_inode(sb, S_IFREG | 0644);
	if (! inode)
		goto out_dput;
	if (!strncmp(name, "cpus", 4)) {
		inode->i_fop = &lfs_cpus_file_ops;
		inode->i_private = value;
	}

	if (!strncmp(name, "cache", 5)) {
		inode->i_fop = &lfs_cache_file_ops;
		inode->i_private = value;
	}

	if (!strncmp(name, "tasks", 5)) {
		inode->i_fop = &lfs_tasks_file_ops;
		inode->i_private = (atomic_t*)value;
	}

	d_add(dentry, inode);
	return dentry;

out_dput:
	dput(dentry);
  out:
	return 0;
}


static char cpus[TMPSIZE];
static char cache[TMPSIZE];
static atomic_t tasks;

static void lfs_create_files (struct super_block *sb, struct dentry *root)
{
	memcpy(cpus, "0\0", 2);
	memcpy(cache, "ff\0", 3);
	atomic_set(&tasks, 0);
	lfs_create_file(sb, root, "cpus", &(*cpus));
	lfs_create_file(sb, root, "cache", &(*cpus));
	lfs_create_file(sb, root, "tasks", (char*)(&tasks));
}


static struct super_operations lfs_s_ops = {
	.statfs		= simple_statfs,
	.drop_inode	= generic_delete_inode,
};


static int lfs_fill_super (struct super_block *sb, void *data, int silent)
{
	struct inode *root;
	struct dentry *root_dentry;

	sb->s_blocksize = PAGE_CACHE_SIZE;
	sb->s_blocksize_bits = PAGE_CACHE_SHIFT;
	sb->s_magic = LFS_MAGIC;
	sb->s_op = &lfs_s_ops;

	root = lfs_make_inode (sb, S_IFDIR | 0755);
	if (! root)
		goto out;
	root->i_op = &simple_dir_inode_operations;
	root->i_fop = &simple_dir_operations;

	root_dentry = d_make_root(root);
	if (! root_dentry)
		goto out_iput;
	sb->s_root = root_dentry;

	lfs_create_files (sb, root_dentry);
	return 0;
	
  out_iput:
	iput(root);
  out:
	return -ENOMEM;
}


static struct dentry *lfs_get_super(struct file_system_type *fst,
		int flags, const char *devname, void *data)
{
	return mount_bdev(fst, flags, devname, data, lfs_fill_super);
}

static struct file_system_type lfs_type = {
	.owner 		= THIS_MODULE,
	.name		= "catctrl",
	.mount		= lfs_get_super,
	.kill_sb	= kill_litter_super,
};



static int __init lfs_init(void)
{
	return register_filesystem(&lfs_type);
}

static void __exit lfs_exit(void)
{
	unregister_filesystem(&lfs_type);
}

module_init(lfs_init);
module_exit(lfs_exit);

#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/string.h>

#define len 200
#define MY_ID "823daa4ac90a"

int file_value;
char ker_buf[len];
struct dentry *dirret, *fileret;

static ssize_t myreader(struct file *fp, char __user *user_buffer,
		size_t count, loff_t *position)
{
	strncpy(ker_buf, MY_ID, strlen(MY_ID));

	return simple_read_from_buffer(user_buffer, count, position,
			ker_buf, len);
}

/* write file operation */
static ssize_t mywriter(struct file *fp, const char __user *user_buffer,
		size_t count, loff_t *position)
{
	ssize_t ret_val;

	if (*position > count)
		ret_val = -EINVAL;

		ret_val = simple_write_to_buffer(ker_buf, count, position,
				user_buffer, count);

		if (strncmp(ker_buf, MY_ID, strlen(MY_ID)) != 0)
			ret_val = -EINVAL;

		return ret_val;
}

static const struct file_operations fops_debug = {
		.read = myreader,
		.write = mywriter,
};

static int debug_init(void)
{
	pr_debug("Initilizing Debugfs Entry\n");

	dirret = debugfs_create_dir("eudyptula", NULL);

	if (!dirret) {
		pr_err("Cannot create directory in debugfs\n");
		return -ENOENT;
	}

	fileret = debugfs_create_file("id", 0666, dirret, &file_value,
			&fops_debug);
	if (!fileret) {
		pr_err("Cannot create file in debugfs\n");
		return -ENOENT;
	}

	return 0;
}

static void debug_exit(void)
{
	pr_debug("Module Exiting\n");
	debugfs_remove_recursive(dirret);
}

module_init(debug_init);
module_exit(debug_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Task 08");
MODULE_AUTHOR("823daa4ac90a Gaurav Pathak");
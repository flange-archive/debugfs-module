#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/debugfs.h>

#define SIZE 8192

MODULE_AUTHOR("Ranger");
MODULE_LICENSE("GPL");

struct dentry *dfs;
struct debugfs_blob_wrapper *myblob;

u32 *stats; /* our blob of data */

int my_init(void)
{
	int i;
	int stats_size;   /* size of our data */
	int struct_size;  /* size of the blob struct */

	struct_size = sizeof(struct debugfs_blob_wrapper);
	stats_size  = SIZE * sizeof(u32);


	/* get mem for data */
	stats = kmalloc(stats_size, GFP_KERNEL);

	if (stats == NULL) {
		printk("Could not allocate mem for data\n");
		return -ENOMEM;
	}


	/* fill datablob with dummy data */
	for (i = 0; i < SIZE; i++)
		stats[i] = i;


	/* get mem for blob struct and init */
	myblob = (struct debugfs_blob_wrapper *) kmalloc(struct_size, GFP_KERNEL);

	if (myblob == NULL) {
		printk("Could not allocate mem for blob\n");
		kfree(stats);
		return -ENOMEM;
	}


	/* only set data pointer and data size */
	myblob->data = (void *) stats;
	myblob->size = (unsigned long) stats_size;


	/* create pseudo file under /sys/kernel/debug/ with name 'test' */
	dfs = debugfs_create_blob("test", 0644, NULL, myblob);

	if (dfs == NULL) {
		printk("Could not create debugfs blob\n");
		kfree(stats);
		kfree(myblob);
		return -EINVAL;
	}

	printk("DebugFS file created\n");

	return 0;
}


void my_exit(void)
{
	printk("DebugFS file deleted\n\n");

	kfree(myblob);
	kfree(stats);

	debugfs_remove(dfs);
}


module_init(my_init);
module_exit(my_exit);

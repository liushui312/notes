
struct block_device_operations {
	int (*open) (struct block_device *, fmode_t);
	int (*release) (struct gendisk *, fmode_t);
	int (*locked_ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	int (*ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	int (*compat_ioctl) (struct block_device *, fmode_t, unsigned, unsigned long);
	int (*direct_access) (struct block_device *, sector_t, void **, unsigned long *);
	int (*media_changed) (struct gendisk *); //介质改变, 可移动设备的驱动
	int (*revalidate_disk) (struct gendisk *);//使介质有效
	int (*getgeo)(struct block_device *, struct hd_geometry *);//获得驱动器信息
	struct module *owner;
};


struct block_device {
	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
	struct inode *		bd_inode;	/* will die */
	struct mutex		bd_mutex;	/* open/close mutex */
	struct semaphore	bd_mount_sem;
	struct list_head	bd_inodes;
	struct block_device *	bd_contains;
	unsigned		bd_block_size;
	struct hd_struct *	bd_part;
	unsigned long		bd_private;
    ...
};




struct gendisk {
	/* major, first_minor and minors are input parameters only,
	 * don't use directly.  Use disk_devt() and disk_max_parts().
	 */
	int major;		/* major number of driver */
	int first_minor;
	int minors;     /* maximum number of minors, =1 for disks that can't be partitioned. */

	char disk_name[DISK_NAME_LEN];	/* name of major driver */

	/* Array of pointers to partitions indexed by partno.
	 * Protected with matching bdev lock but stat and other
	 * non-critical accesses use RCU.  Always access through
	 * helpers.
	 */
	struct disk_part_tbl *part_tbl;
	struct hd_struct part0;

	struct block_device_operations *fops;
	struct request_queue *queue;
	void *private_data;

	int flags;
	struct device *driverfs_dev;  // FIXME: remove
	struct kobject *slave_dir;

	struct timer_rand_state *random;

	atomic_t sync_io;		/* RAID */
	struct work_struct async_notify;
#ifdef  CONFIG_BLK_DEV_INTEGRITY
	struct blk_integrity *integrity;
#endif
	int node_id;
};

struct gendisk *alloc_disk(int minors);
void add_disk(struct gendisk *gd);
void del_gendisk(struct gendisk *gd);



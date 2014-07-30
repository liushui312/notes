#include <linux/module.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/fs.h>		/* everything... */
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>

struct device_name_dev {
    const char *name;
    struct cdev cdev;
    ...
};

#include "device_name.h"

struct device_dev *device;
static struct class *dev_name_class;
static struct device *dev_name_class_dev;

static int dev_name_open(struct inode *inode, struct file *filp)
{
    struct device_dev *device;

    device = container_of(inode->i_cdev, struct device_dev, cdev);
    filp->private_data = device;
    
    if ( request_mem_region(device->gpio_base_phys, SZ_4K, device->name) == NULL ) {
        printk("request iomem failed!\n"); 
        return -1;
    }
    device->gpio_base_virt = ioremap(device->gpio_base_phys, SZ_4K );

	return 0;
}


int dev_name_release(struct inode *, struct file *)
{
    
}

static ssize_t dev_name_read(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}

static ssize_t dev_name_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	return 0;
}


int dev_name_ioctl(struct inode *, struct file *, unsigned int, unsigned long)
{
     
}

static struct file_operations dev_name_fops = {
    .owner  =   THIS_MODULE,    
    .open   =   dev_name_open,     
    .read   =   dev_name_read,
	.write	=	dev_name_write,	   
    .ioctl  =   dev_name_ioctl,
    .release =  dev_name_release, 
};


static int dev_name_init(void)
{
    alloc_chrdev_region(); 

	dev_name_class = class_create(THIS_MODULE, "dev_name");

	dev_name_class_dev = device_create(dev_name_class, NULL, MKDEV(major, 0), NULL, "xyz"); /* /dev/xyz */

	return 0;
}

static void dev_name_exit(void)
{
	unregister_chrdev(major, "dev_name"); 

	device_unregister(dev_name_class_dev);
	class_destroy(dev_name_class);
	iounmap(gpfcon);
}

module_init(dev_name_init);
module_exit(dev_name_exit);


MODULE_LICENSE("GPL");


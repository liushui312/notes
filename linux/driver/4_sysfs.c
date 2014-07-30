
sysfs文件系统中，普通文件对应于kobject中的属性。用sysfs_create_file()


1. 创建kset  
struct kset *kset_create_and_add(const char *name,
				 const struct kset_uevent_ops *uevent_ops,
				 struct kobject *parent_kobj)

2. 创建kobject 
struct kobject *kobject_create_and_add(const char *name, struct kobject *parent)

3. 定义kobject 属性
struct kobj_attribute {
	struct attribute attr;
	ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);
	ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);
};

eg:
    struct kobj_attribute case_attr = {
    	.attr = {
    		.name = "test_case",
    		.mode = 0666,
    	},
    	.show = case_show,
    	.store = case_store,
    };

4. 
int sysfs_create_file(struct kobject * kobj, const struct attribute * attr)


5. 实现属性的回调函数
    echo  <==> store
    cat   <==> show 

---------------------------------------------------------------------------------

static DEVICE_ATTR(name, mode, xxx_show, xxx_store); 
//宏DEVICE_ATTR 定义了 struct device_attribute dev_attr_##_name

static struct attribute *xxx[] = {
    &dev_attr_name.attr,
    NULL
};




-------------------------------------------------------------------------------
eg:
    struct kobj_attribute *attr_group[] = {
    	&case_attr,
    	&result_attr,
    	&size_attr,
    	&nr_thread_attr,
    	&nr_chan_attr,
    	&times_thread_attr,
    	NULL,
    };

	struct kobj_attribute **gropu_ptr = NULL;
	test_kset = kset_create_and_add("sunxi", NULL, NULL);
	if (!test_kset)
		return -ENOMEM;
	dmatest_obj = kobject_create_and_add("dmatest", &test_kset->kobj);
	if (!dmatest_obj) {
		ret = -ENOMEM;
		goto err;
	}
	for (gropu_ptr = attr_group; (*gropu_ptr) != NULL; gropu_ptr++) {
		ret = sysfs_create_file(dmatest_obj, &(*gropu_ptr)->attr);
		if (ret)
			goto err;
	}

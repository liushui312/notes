//MTD设备通知结构体 

struct mtd_notifier {
    void(*add)(struct mtd_info *mtd); //加入MTD原始字符/块设备时执行
    void(*remove)(struct mtd_info *mtd); //移除MTD原始字符/块设备时执行
    struct list_head list;
};

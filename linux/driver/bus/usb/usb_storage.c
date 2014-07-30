
struct bus_type -> struct bus_type_private -> struct klist klist_devices
                                              struct klist klist_drivers

struct device -> struct bus_type
                 struct device_driver

struct device_driver -> struct bus_type
                        struct driver_private -> struct klist klist_devices

usb core 的代码会进行整个 usb 系统的初始化: 建立 devices 链表
    申请 struct bus_type -> 扫描 usb 总线 -> usb 设备 -> 申请 struct device -> 插入总线 devices 链表
                                             hub -> 申请 struct device -> 扫描hub -> usb 设备 -> ...
USB 驱动程序 
    申请 struct usb_driver -> 调用 usb_register() -> 插入总线 drivers 链表
    遍历 devices 链表 -> device_bind_driver() -> 把 struct device 中的 struct device_driver 指向这个driver
                                              -> 把 struct device 插入自己的 klist_devices         
热拔插
  每当一个 struct device 诞生, 它就会去 bus 的 drivers 链表中寻找自己的driver     |
  每当一个 struct device_driver 诞生, 它就去 bus 的 devices 链表中寻找它的那些设备| -> 调用device_bind_driver()

----------------------------------------------------------------------------------------------------------------

storage_probe() -> 申请 struct us_data 并分配内存 
                   associate_dev()    -> 为 us 的各成员赋值
                   get_device_info()
                   get_protocol()
                   get_transport()
                   get_pipes()



网络设备驱动

1. 数据结构
    net_device 
    sk_buff

struct net_device {
	char			name[IFNAMSIZ];       //网络设备名
	const struct ethtool_ops *ethtool_ops;
	unsigned		mtu;	              //最大传输单元
	void			*priv;	              //私有数据指针
    /* 上层协议栈回调, 发送数据 */
	int	    (*hard_start_xmit)(struct sk_buff *skb, struct net_device *dev);
	unsigned long		trans_start;      //发送时间戳, jiffies
	int			watchdog_timeo;           //超时时间
	int			(*open)(struct net_device *dev);//ifconfig eth0 up
	int			(*stop)(struct net_device *dev);//ifconfig eth0 down
	int			(*set_mac_address)(struct net_device *dev, void *addr);
	void			(*tx_timeout) (struct net_device *dev);//watchdog_timeo超时回调
	struct device		dev;
    ....
};

struct sk_buff {
	struct net_device	*dev;            //设备指针
	unsigned int		len, data_len;   //数据包的长度
	__u16			mac_len, hdr_len;
	__be16			protocol;i           //协议编号
	sk_buff_data_t		transport_header;//传输层协议头地址
	sk_buff_data_t		network_header;  //网络层协议头地址
	sk_buff_data_t		mac_header;      //链路层协议头地址
	sk_buff_data_t		tail;            //指向有效载荷的结束位置
	sk_buff_data_t		end;            
	unsigned char		*head, *data;    //head 指向数据包头, data 指向有效载荷的起始位置
	unsigned int		truesize;        //sizeof(struct sk_buff) + 数据包
	atomic_t		users;
};



2. 函数接口
/* 分配和释放以太网设备 */
struct net_device *alloc_etherdev(int sizeof_priv)
void free_netdev(struct net_device *dev)

/* 网络设备注册和注销 */
int register_netdev(struct net_device *dev)
void unregister_netdev(struct net_device *dev)



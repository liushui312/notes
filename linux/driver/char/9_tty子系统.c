
数据结构 serial_core.h

struct uart_port {
	struct uart_state	*state;			/* pointer to parent state */
	struct uart_icount	icount;			/* statistics */
	struct console		*cons;			/* struct console, if any */
	const struct uart_ops	*ops;
	struct device		*dev;			/* parent device */

	unsigned int (*serial_in)(struct uart_port *, int);
	void (*serial_out)(struct uart_port *, int, int);
	void (*set_termios)(struct uart_port *, struct ktermios *new,struct ktermios *old);
	int (*handle_irq)(struct uart_port *);
	void (*pm)(struct uart_port *, unsigned int state, unsigned int old);

	spinlock_t		lock;			    /* port lock */
	unsigned long		iobase;			/* in/out[bwl] */
	unsigned char __iomem	*membase;	/* read/write[bwl] */
	unsigned int		irq;			/* irq number */
	unsigned long		irqflags;		/* irq flags  */
	unsigned int		uartclk;		/* base uart clock */
	unsigned int		fifosize;		/* tx fifo size */
	unsigned char		x_char;			/* xon/xoff char */
	unsigned char		regshift;		/* reg offset shift */
	unsigned char		iotype;			/* io access style */
	unsigned char		unused1;
	unsigned int		read_status_mask;	/* driver specific */
	unsigned int		ignore_status_mask;	/* driver specific */
	upf_t			flags;
	unsigned int		mctrl;			/* current modem ctrl settings */
	unsigned int		timeout;		/* character-based timeout */
	unsigned int		type;			/* port type */
	unsigned int		custom_divisor;
	unsigned int		line;			/* port index */
	resource_size_t		mapbase;		/* for ioremap */
	unsigned char		hub6;			/* this should be in the 8250 driver */
	unsigned char		suspended;
	unsigned char		irq_wake;
	unsigned char		unused[2];
	void			*private_data;		/* generic platform data pointer */
    ...
};


/* This is the state information which is persistent across opens. */
struct uart_state {
	struct uart_port	*uart_port;
	struct tty_port		port;
	struct circ_buf		xmit;
	int			        pm_state;
};


struct uart_driver {
	const char		*driver_name;
	const char		*dev_name;
	int			 major;
	int			 minor;
	int			 nr;
	struct console		*cons;
	struct module		*owner;

	/* these are private; the low level driver should not
	 * touch these; they should be initialised to NULL     */
	struct uart_state	*state;
	struct tty_driver	*tty_driver;
};
------------------------------------------------------------------------------------
函数接口 serial_core.c 
/**
 *	uart_get_baud_rate - return baud rate for a particular port
 *	@port: uart_port structure describing the port in question.
 *	@termios: desired termios settings.
 *	@old: old termios (or NULL)
 *	@min: minimum acceptable baud rate
 *	@max: maximum acceptable baud rate
 *
 *	Decode the termios structure into a numeric baud rate,
 *	taking account of the magic 38400 baud rate (with spd_*
 *	flags), and mapping the %B0 rate to 9600 baud.
 *
 *	If the new baud rate is invalid, try the old termios setting.
 *	If it's still invalid, we try 9600 baud.
 *
 *	Update the @termios structure to reflect the baud rate
 *	we're actually going to be using. Don't do this for the case
 *	where B0 is requested ("hang up").
 */
unsigned int uart_get_baud_rate (struct uart_port *port, struct ktermios *termios,
            struct ktermios *old, unsigned int min, unsigned int max);

/**
 *	uart_get_divisor - return uart clock divisor
 *	@port: uart_port structure describing the port.
 *	@baud: desired baud rate
 *
 *	Calculate the uart clock divisor for the port.
 */
unsigned int uart_get_divisor(struct uart_port *port, unsigned int baud);

/**
 *	uart_update_timeout - update per-port FIFO timeout.
 *	@port:  uart_port structure describing the port
 *	@cflag: termios cflag value
 *	@baud:  speed of the port
 *
 *	Set the port FIFO timeout value.  The @cflag value should
 *	reflect the actual hardware settings.
 */
void uart_update_timeout(struct uart_port *port, unsigned int cflag,
            unsigned int baud)


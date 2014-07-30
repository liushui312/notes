
struct mtd_info {
	u_char type; //内存技术类型，例如MTD_RAM,MTD_ROM,MTD_NORFLASH,MTD_NAND_FLASH,MTD_PEROM等
	u_int32_t flags; //标志位
	u_int32_t size;	 // Total size of the MTD

	/* "Major" erase size for the device. Na飗e users may take this
	 * to be the only erase size available, or may use the more detailed
	 * information below if they desire
	 */
	u_int32_t erasesize; //最小的擦除块大小

	u_int32_t oobblock;  // Size of OOB blocks (e.g. 512)
	u_int32_t oobsize;   // Amount of OOB data per block (e.g. 16)
	u_int32_t oobavail;  // Number of bytes in OOB area available for fs 
	u_int32_t ecctype;
	u_int32_t eccsize;
	

	// Kernel-only stuff starts here.
	char *name;
	int index;

	// oobinfo is a nand_oobinfo structure, which can be set by iotcl (MEMSETOOBINFO)
	struct nand_oobinfo oobinfo;

	/* Data for variable erase regions. If numeraseregions is zero,
	 * it means that the whole device has erasesize as given above. 
	 */
	int numeraseregions;
	struct mtd_erase_region_info *eraseregions; 

	/* This really shouldn't be here. It can go away in 2.5 */
	u_int32_t bank_size;

	int (*erase) (struct mtd_info *mtd, struct erase_info *instr);

	/* This stuff for eXecute-In-Place */
	int (*point) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char **mtdbuf);

	/* We probably shouldn't allow XIP if the unpoint isn't a NULL */
	void (*unpoint) (struct mtd_info *mtd, u_char * addr, loff_t from, size_t len);


	int (*read) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);
	int (*write) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf);

	int (*read_ecc) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf, u_char *eccbuf, struct nand_oobinfo *oobsel);
	int (*write_ecc) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf, u_char *eccbuf, struct nand_oobinfo *oobsel);

	int (*read_oob) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);
	int (*write_oob) (struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf);

	/* 
	 * Methods to access the protection register area, present in some 
	 * flash devices. The user data is one time programmable but the
	 * factory data is read only. 
	 */
	int (*read_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

	int (*read_fact_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

	/* This function is not yet implemented */
	int (*write_user_prot_reg) (struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf);

	/* kvec-based read/write methods. We need these especially for NAND flash,
	   with its limited number of write cycles per erase.
	   NB: The 'count' parameter is the number of _vectors_, each of 
	   which contains an (ofs, len) tuple.
	*/
	int (*readv) (struct mtd_info *mtd, struct kvec *vecs, unsigned long count, loff_t from, size_t *retlen);
	int (*readv_ecc) (struct mtd_info *mtd, struct kvec *vecs, unsigned long count, loff_t from, 
		size_t *retlen, u_char *eccbuf, struct nand_oobinfo *oobsel);
	int (*writev) (struct mtd_info *mtd, const struct kvec *vecs, unsigned long count, loff_t to, size_t *retlen);
	int (*writev_ecc) (struct mtd_info *mtd, const struct kvec *vecs, unsigned long count, loff_t to, 
		size_t *retlen, u_char *eccbuf, struct nand_oobinfo *oobsel);

	/* Sync */
	void (*sync) (struct mtd_info *mtd);

	/* Chip-supported device locking */
	int (*lock) (struct mtd_info *mtd, loff_t ofs, size_t len);
	int (*unlock) (struct mtd_info *mtd, loff_t ofs, size_t len);

	/* Power Management functions */
	int (*suspend) (struct mtd_info *mtd);
	void (*resume) (struct mtd_info *mtd);

	/* Bad block management functions */
	int (*block_isbad) (struct mtd_info *mtd, loff_t ofs);
	int (*block_markbad) (struct mtd_info *mtd, loff_t ofs);

	void *priv;

	struct module *owner;
	int usecount;
};


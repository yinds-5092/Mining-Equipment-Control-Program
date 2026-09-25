/*************  技术支持与购买说明    **************
产品主页：http://twen51.com
淘宝搜索：天问51，可购买。基础版，带彩屏标准版，旗舰版
技术支持QQ群二：915765308
******************************************/
#ifndef __SDCARD_H
#define __SDCARD_H

#include <STC8HX.h>
#include "delay.h"
#include "hardspi.h"
#include "pffconf.h"

#define _USE_WRITE 1

#ifndef 	SDCARD_CS_PIN
#define     SDCARD_CS_PIN   P2_6        //片选脚
#endif

#ifndef 	SDCARD_CS_PIN_mode
#define		SDCARD_CS_PIN_mode	P2M1&=~0x40;P2M0|=0x40;     //P26推挽输出
#endif 

//SD卡的命令
#define     CMD0            (0x40+0)     	/* GO_IDLE_STATE */
#define     CMD1            (0x40+1)     	/* SEND_OP_COND */
#define     CMD8            (0x40+8)     	/* SEND_IF_COND */
#define     CMD9            (0x40+9)     	/* SEND_CSD */
#define     CMD10           (0x40+10)    	/* SEND_CID */
#define     CMD12           (0x40+12)    	/* STOP_TRANSMISSION */
#define     CMD16           (0x40+16)    	/* SET_BLOCKLEN */
#define     CMD17           (0x40+17)    	/* READ_SINGLE_BLOCK */
#define     CMD18           (0x40+18)    	/* READ_MULTIPLE_BLOCK */
#define     CMD23           (0x40+23)    	/* SET_BLOCK_COUNT */
#define     CMD24           (0x40+24)    	/* WRITE_BLOCK */
#define     CMD25           (0x40+25)    	/* WRITE_MULTIPLE_BLOCK */
#define     CMD41           (0x40+41)    	/* SEND_OP_COND (ACMD) */
#define     CMD55           (0x40+55)    	/* APP_CMD */
#define     CMD58           (0x40+58)    	/* READ_OCR */
#define     ACMD41          (0xC0+41)      

/* MMC card type flags (MMC_GET_TYPE) */
#define     CT_MMC		    0x01		/* MMC ver 3 */
#define     CT_SD1		    0x02		/* SD ver 1 */
#define     CT_SD2		    0x04		/* SD ver 2 */
#define     CT_SDC		    0x06		/* SD */
#define     CT_BLOCK	    0x08		/* Block addressing */

#define     STA_NOINIT		0x01	/* Drive not initialized */
#define     STA_NODISK		0x02	/* No medium in the drive */

/*-----------------------------------------------------------------------*/
/* diskio.h                                                                 */
/*-----------------------------------------------------------------------*/
/* Status of Disk Functions */
typedef  uint8	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Function succeeded */
	RES_ERROR,		/* 1: Disk error */
	RES_NOTRDY,		/* 2: Not ready */
	RES_PARERR		/* 3: Invalid parameter */
} DRESULT;

DSTATUS disk_initialize ();
DRESULT disk_readp (uint8* buff, uint32 sector, uint16 offser, uint16 count);
DRESULT disk_writep (const uint8* buff, uint32 sc);

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */

static void SELECT(){SDCARD_CS_PIN = 0;}
static void DESELECT(){SDCARD_CS_PIN = 1;}
#define	    SELECTING       SDCARD_CS_PIN	/* CS status (true:CS low) */

static xdata uint8 CardType;

static uint8 send_cmd55(uint8 cmd,uint32 arg)
{
	uint8 n, res;
	/* Select the card */
	DESELECT();
	hardspi_wr_data(0xff);
	SELECT();
	hardspi_wr_data(0xff);

	/* Send a command packet */
	hardspi_wr_data(cmd);			/* Start + Command index */
	hardspi_wr_data((uint8)(arg >> 24));	/* Argument[31..24] */
	hardspi_wr_data((uint8)(arg >> 16));	/* Argument[23..16] */
	hardspi_wr_data((uint8)(arg >> 8));	/* Argument[15..8] */
	hardspi_wr_data((uint8)arg);		/* Argument[7..0] */
	n = 0x01;				/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;		/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;		/* Valid CRC for CMD8(0x1AA) */
	hardspi_wr_data(n);

	/* Receive a command response */
	n = 10;	/* Wait for a valid response in timeout of 10 attempts */
	do {
		res = hardspi_wr_data(0xff);
	} while ((res & 0x80) && --n);

	return res;/* Return with the response value */
}

static uint8 send_cmd(uint8 cmd,uint32 arg)
{
	BYTE n, res;
	if (cmd & 0x80) { /* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd55(CMD55, 0);
		if (res > 1) return res;
	}
	/* Select the card */
	DESELECT();
	hardspi_wr_data(0xff);
	SELECT();
	hardspi_wr_data(0xff);

	/* Send a command packet */
	hardspi_wr_data(cmd);			        /* Start + Command index */
	hardspi_wr_data((uint8)(arg >> 24));	/* Argument[31..24] */
	hardspi_wr_data((uint8)(arg >> 16));	/* Argument[23..16] */
	hardspi_wr_data((uint8)(arg >> 8));	    /* Argument[15..8] */
	hardspi_wr_data((uint8)arg);		    /* Argument[7..0] */
	n = 0x01;				            /* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;		    /* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;		    /* Valid CRC for CMD8(0x1AA) */
	hardspi_wr_data(n);
	/* Receive a command response */
	n = 10;	/* Wait for a valid response in timeout of 10 attempts */
	do {
		res = hardspi_wr_data(0xff);
	} while ((res & 0x80) && --n);
	return res;/* Return with the response value */
}

DSTATUS disk_initialize()
{
	uint8  n, cmd, ty, ocr[4];
	uint16  tmr;

    SDCARD_CS_PIN_mode;     //推挽输出
    SDCARD_CS_PIN = 1;   
	hardspi_init(0);       /* Initialize ports to control MMC */

#if _USE_WRITE
	if (CardType && SELECTING) disk_writep(0, 0);	/* Finalize write process if it is in progress */
#endif

	DESELECT();
	for (n = 10; n; n--) hardspi_wr_data(0xff);	    /* 80 dummy clocks with CS=H */
	hardspi_init(0);                             /* Initialize ports to control MMC */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* GO_IDLE_STATE */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2 */
			for (n = 0; n < 4; n++) ocr[n] = hardspi_wr_data(0xff);/* Get trailing return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {			/* The card can work at vdd range of 2.7-3.6V */
				for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) delay100us();/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = hardspi_wr_data(0xff);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 (HC or SC) */
				}
			}
		} else {							/* SDv1 or MMCv3 */
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--) delay100us();	/* Wait for leaving idle state */
			if (!tmr || send_cmd(CMD16, 512) != 0)			/* Set R/W block length to 512 */
				ty = 0;
		}
	}
	CardType = ty;
	DESELECT();
	hardspi_wr_data(0xff);
	return ty ? 0 : STA_NOINIT;
}

DRESULT disk_readp (
	uint8 *buff,	/* Pointer to the read buffer (NULL:Forward to the stream) */
	UINT32 sector,	/* Sector number (LBA) */
	uint16 offset,	/* Byte offset to read from (0..511) */
	uint16 count	/* Number of bytes to read (ofs + cnt mus be <= 512) */
)
{
	DRESULT res;
	uint8 rc;
	uint16  bc;
	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */
	res = RES_ERROR;
	if (send_cmd(CMD17, sector) == 0) {	/* READ_SINGLE_BLOCK */
		bc = 40000;	/* Time counter */
		do {	/* Wait for data packet */
			rc = hardspi_wr_data(0xff);
		} while (rc == 0xFF && --bc);
		if (rc == 0xFE) {/* A data packet arrived */
			bc = 512 + 2 - offset - count;	/* Number of trailing bytes to skip */
			/* Skip leading bytes */
			while (offset--) hardspi_wr_data(0xff);
			/* Receive a part of the sector */
			if (buff) {	/* Store data to the memory */
				do {
					*buff++ = hardspi_wr_data(0xff);
				} while (--count);
			} else {	/* Forward data to the outgoing stream */
				do {
					hardspi_wr_data(0xff);
				} while (--count);
			}
			/* Skip trailing bytes and CRC */
			do hardspi_wr_data(0xff); while (--bc);
			res = RES_OK;
		}
	}
	DESELECT();
	hardspi_wr_data(0xff);
	return res;
}
/*-----------------------------------------------------------------------*/
/* Write partial sector                                                  */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_writep (
	const uint8 *buff,	/* Pointer to the bytes to be written (NULL:Initiate/Finalize sector write) */
	uint32 sc		/* Number of bytes to send, Sector number (LBA) or zero */
)
{
	DRESULT res;
	uint16  bc;
	static uint16 xdata wc;	/* Sector write counter */
	res = RES_ERROR;
	if (buff) {		/* Send data bytes */
		bc = sc;
		while (bc && wc) {/* Send data bytes to the card */
			hardspi_wr_data(*buff++);
			wc--; bc--;
		}
		res = RES_OK;
	} else {
		if (sc) {	/* Initiate sector write process */
			if (!(CardType & CT_BLOCK)) sc *= 512;	/* Convert to byte address if needed */
			if (send_cmd(CMD24, sc) == 0) {			/* WRITE_SINGLE_BLOCK */
				hardspi_wr_data(0xFF); hardspi_wr_data(0xFE);		/* Data block header */
				wc = 512;	/* Set byte counter */
				res = RES_OK;
			}
		} else {	/* Finalize sector write process */
			bc = wc + 2;
			while (bc--) hardspi_wr_data(0);	/* Fill left bytes and CRC with zeros */
			if ((hardspi_wr_data(0xff) & 0x1F) == 0x05) {	/* Receive data resp and wait for end of write process in timeout of 500ms */
				for (bc = 5000; hardspi_wr_data(0xff) != 0xFF && bc; bc--)	/* Wait for ready */
					delay100us();
				if (bc) res = RES_OK;
			}
			DESELECT();
			hardspi_wr_data(0xff);
		}
	}
	return res;
}
#endif
#endif //   sdcard.h


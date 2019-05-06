#include "AT45DB081B.h"
#include "delay.h"
#include "usart.h"

#ifndef MAX_AT45_DEVICES
#define MAX_AT45_DEVICES        1
#endif

#ifndef MAX_AT45_CMDLEN
#define MAX_AT45_CMDLEN         8
#endif

#ifndef AT45_CONF_DFSPI
#define AT45_CONF_DFSPI         SPI0_BASE
#endif

#ifndef AT45_CONF_DFPCS
#define AT45_CONF_DFPCS         1
#endif

#ifndef AT45_ERASE_WAIT
#define AT45_ERASE_WAIT         3000
#endif

#ifndef AT45_CHIP_ERASE_WAIT
#define AT45_CHIP_ERASE_WAIT    50000
#endif

#ifndef AT45_WRITE_POLLS
#define AT45_WRITE_POLLS        1000
#endif

#define DFCMD_CONT_READ_LF      0x03

#define DFCMD_CONT_READ_HF      0x0B

#define DFCMD_BLOCK_ERASE       0x50

#define DFCMD_SECTOR_ERASE      0x7C

#define DFCMD_PAGE_ERASE        0x81

#define DFCMD_BUF1_PROG         0x82

#define DFCMD_BUF1_FLASH        0x83

#define DFCMD_BUF1_WRITE        0x84

#define DFCMD_BUF2_PROG         0x85

#define DFCMD_BUF2_FLASH        0x86

#define DFCMD_BUF2_WRITE        0x87

#define DFCMD_BUF1_FLASH_NE     0x88

#define DFCMD_BUF2_FLASH_NE     0x89

#define DFCMD_CHIP_ERASE        0xC7

#define DFCMD_BUF1_READ_LF      0xD1

#define DFCMD_READ_PAGE         0xD2

#define DFCMD_BUF2_READ_LF      0xD3

#define DFCMD_BUF1_READ         0xD4

#define DFCMD_BUF2_READ         0xD6

#define DFCMD_READ_STATUS       0xD7

#define DFCMD_CONT_READ         0xE8

#define AT45DB_AT91


#if defined(AT45DB_SPI0_DEVICE)

#include <dev/sppif0.h>
#if defined(AT45DB_RESET_ACTIVE_HIGH)
#define SpiReset(act)       Sppi0ChipReset(AT45DB_SPI0_DEVICE, act)
#else
#define SpiReset(act)       Sppi0ChipReset(AT45DB_SPI0_DEVICE, !act)
#endif
#define SpiSetMode()        Sppi0SetMode(AT45DB_SPI0_DEVICE, AT45DB_SPI_MODE)
#define SpiSetSpeed()       Sppi0SetSpeed(AT45DB_SPI0_DEVICE, AT45DB_SPI_RATE)
#if defined(AT45DB_SELECT_ACTIVE_HIGH)
#define SpiSelect()         Sppi0SelectDevice(AT45DB_SPI0_DEVICE)
#define SpiDeselect()       Sppi0DeselectDevice(AT45DB_SPI0_DEVICE)
#else
#define SpiSelect()         Sppi0NegSelectDevice(AT45DB_SPI0_DEVICE)
#define SpiDeselect()       Sppi0NegDeselectDevice(AT45DB_SPI0_DEVICE)
#endif
#define SpiByte             Sppi0Byte

#elif defined(AT45DB_SBBI0_DEVICE)

#include <dev/sbbif0.h>
#if defined(AT45DB_RESET_ACTIVE_HIGH)
#define SpiReset(act)       Sbbi0ChipReset(AT45DB_SBBI0_DEVICE, act)
#else
#define SpiReset(act)       Sbbi0ChipReset(AT45DB_SBBI0_DEVICE, !act)
#endif
#define SpiSetMode()        Sbbi0SetMode(AT45DB_SBBI0_DEVICE, AT45DB_SPI_MODE)
#define SpiSetSpeed()       Sbbi0SetSpeed(AT45DB_SBBI0_DEVICE, AT45DB_SPI_RATE)
#if defined(AT45DB_SELECT_ACTIVE_HIGH)
#define SpiSelect()         Sbbi0SelectDevice(AT45DB_SBBI0_DEVICE)
#define SpiDeselect()       Sbbi0DeselectDevice(AT45DB_SBBI0_DEVICE)
#else
#define SpiSelect()         Sbbi0NegSelectDevice(AT45DB_SBBI0_DEVICE)
#define SpiDeselect()       Sbbi0NegDeselectDevice(AT45DB_SBBI0_DEVICE)
#endif
#define SpiByte             Sbbi0Byte

#endif

typedef struct _AT45_DEVTAB {
    uint32_t devt_pages;
    unsigned int devt_pagsiz;
    unsigned int devt_offs;
    uint8_t devt_srmsk;
    uint8_t devt_srval;
} AT45_DEVTAB;

typedef struct _AT45DB_DCB {
    AT45_DEVTAB *dcb_devt;
    unsigned int dcb_spibas;
    unsigned int dcb_spipcs;
    uint8_t dcb_cmdbuf[MAX_AT45_CMDLEN];
} AT45DB_DCB;

AT45_DEVTAB at45_devt[] = {
    {512, 264, 9, 0x3C, 0x0C},  /* AT45DB011B - 128kB */
    {1025, 264, 9, 0x3C, 0x14}, /* AT45DB021B - 256kB */
    {2048, 264, 9, 0x3C, 0x1C}, /* AT45DB041B - 512kB */
    {4096, 264, 9, 0x3C, 0x24}, /* AT45DB081B - 1MB */
    {4096, 528, 10, 0x3C, 0x2C},        /* AT45DB0161B - 2MB */
    {8192, 528, 10, 0x3C, 0x34},        /* AT45DB0321B - 4MB */
    {8192, 1056, 11, 0x38, 0x38},       /* AT45DB0642 - 8MB */
    {0, 0, 0, 0, 0}             /* End of table */
};

static AT45DB_DCB dcbtab[MAX_AT45_DEVICES];

/* Number of active chips. */
static uint_least8_t dcbnum;

/* Chip used for parameter storage. */
static int dd_param = -1;

int At45dbSendCmd(int dd, uint8_t op, uint32_t parm, int len, const void *tdata, void *rdata, int datalen)
{
    uint8_t *cb = dcbtab[dd].dcb_cmdbuf;

    if (len > MAX_AT45_CMDLEN) {
        return -1;
    }
    memset(cb, 0, len);
    cb[0] = op;
    if (parm) {
        cb[1] = (uint8_t) (parm >> 16);
        cb[2] = (uint8_t) (parm >> 8);
        cb[3] = (uint8_t) parm;
    }
    return At91SpiTransfer2(dcbtab[dd].dcb_spibas, dcbtab[dd].dcb_spipcs, cb, cb, len, tdata, rdata, datalen);
}

uint8_t At45dbGetStatus(int dd)
{
    uint8_t buf[2] = { DFCMD_READ_STATUS, 0xFF };

    if (At91SpiTransfer2(dcbtab[dd].dcb_spibas, dcbtab[dd].dcb_spipcs, buf, buf, 2, NULL, NULL, 0)) {
        return (uint8_t) - 1;
    }
    return buf[1];
}

int At45dbWaitReady(int dd, uint32_t tmo, int poll)
{
    uint8_t sr;

    while (((sr = At45dbGetStatus(dd)) & 0x80) == 0) {
        if (!poll) {
            NutSleep(1);
        }
        if (tmo-- == 0) {
            return -1;
        }
    }
    return 0;
}

int At45dbInit(unsigned int spibas, unsigned int spipcs)
{
    int dd = -1;
    uint8_t sr;
    uint_fast8_t i;

    for (i = 0; i < dcbnum; i++) {
        if (dcbtab[i].dcb_spibas == spibas && dcbtab[i].dcb_spipcs == spipcs) {
            return i;
        }
    }

    if (dcbnum >= MAX_AT45_DEVICES) {
        return -1;
    }

#if defined(MCU_AT91SAM7X) || defined(MCU_AT91SAM7SE) || defined(MCU_AT91SAM9260) || defined(MCU_AT91SAM9XE)
    At91SpiInit(spibas);
    At91SpiReset(spibas);
    At91SpiInitChipSelects(spibas, _BV(spipcs));
    At91SpiSetRate(spibas, spipcs, 1000000);
    At91SpiSetModeFlags(spibas, spipcs, SPIMF_MASTER | SPIMF_SCKIAHI | SPIMF_CAPRISE);
#elif defined(MCU_AT91R40008)
#endif

    dcbtab[dcbnum].dcb_spibas = spibas;
    dcbtab[dcbnum].dcb_spipcs = spipcs;
    sr = At45dbGetStatus(dcbnum);

    for (i = 0; at45_devt[i].devt_pages; i++) {
        if ((sr & at45_devt[i].devt_srmsk) == at45_devt[i].devt_srval) {
            dcbtab[dcbnum].dcb_devt = &at45_devt[i];
            dd = dcbnum++;
            break;
        }
    }
    return dd;
}

int At45dbPageErase(int dd, uint32_t pgn)
{
    pgn <<= dcbtab[dd].dcb_devt->devt_offs;
    return At45dbSendCmd(dd, DFCMD_PAGE_ERASE, pgn, 4, NULL, NULL, 0);
}

int At45dbChipErase(void)
{
    return -1;
}

int At45dbPageRead(int dd, uint32_t pgn, void *data, unsigned int len)
{
    pgn <<= dcbtab[dd].dcb_devt->devt_offs;
    return At45dbSendCmd(dd, DFCMD_CONT_READ, pgn, 8, data, data, len);
}

int At45dbPageWrite(int dd, uint32_t pgn, CONST void *data, unsigned int len)
{
    int rc = -1;
    void *rp;

    if ((rp = malloc(len)) != NULL) {
        /* Copy data to dataflash RAM buffer. */
        if (At45dbSendCmd(dd, DFCMD_BUF1_WRITE, 0, 4, data, rp, len) == 0) {
            /* Flash RAM buffer. */
            pgn <<= dcbtab[dd].dcb_devt->devt_offs;
            if (At45dbSendCmd(dd, DFCMD_BUF1_FLASH, pgn, 4, NULL, NULL, 0) == 0) {
                rc = At45dbWaitReady(dd, AT45_WRITE_POLLS, 1);
            }
        }
        free(rp);
    }
    return rc;
}

uint32_t At45dbPages(int dd)
{
    return dcbtab[dd].dcb_devt->devt_pages;
}

unsigned int At45dbPageSize(int dd)
{
    return dcbtab[dd].dcb_devt->devt_pagsiz;
}

uint32_t At45dbParamPage(void)
{
#ifdef AT45_CONF_PAGE
    return AT45_CONF_PAGE;
#else
    return dcbtab[dd_param].dcb_devt->devt_pages - 1;
#endif
}

int At45dbParamSize(void)
{
    int rc;

    if (dd_param == -1 && (dd_param = At45dbInit(AT45_CONF_DFSPI, AT45_CONF_DFPCS)) == -1) {
        return -1;
    }
#ifdef AT45_CONF_SIZE
    rc = AT45_CONF_SIZE;
#else
    rc = dcbtab[dd_param].dcb_devt->devt_pagsiz;
#endif
    return rc;
}

int At45dbParamRead(unsigned int pos, void *data, unsigned int len)
{
    int rc = -1;
    uint8_t *buff;
    int csize = At45dbParamSize();
    uint32_t cpage = At45dbParamPage();

    /* Load the complete configuration area. */
    if (csize > len && (buff = malloc(csize)) != NULL) {
        rc = At45dbPageRead(dd_param, cpage, buff, csize);
        /* Copy requested contents to caller's buffer. */
        memcpy(data, buff + pos, len);
        free(buff);
    }
    return rc;
}

int At45dbParamWrite(unsigned int pos, CONST void *data, unsigned int len)
{
    int rc = -1;
    uint8_t *buff;
    int csize = At45dbParamSize();
    uint32_t cpage = At45dbParamPage();

    /* Load the complete configuration area. */
    if (csize > len && (buff = malloc(csize)) != NULL) {
        rc = At45dbPageRead(dd_param, cpage, buff, csize);
        /* Compare old with new contents. */
        if (memcmp(buff + pos, data, len)) {
            /* New contents differs. Copy it into the sector buffer. */
            memcpy(buff + pos, data, len);
            /* Erase sector and write new data. */
            rc = At45dbPageWrite(dd_param, cpage, buff, csize);
        }
        free(buff);
    }
    return rc;
}

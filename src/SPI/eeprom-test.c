#include "spi-master.h"

/* AMTEL AT250x0 SPI EEPROM define */
#define AT25040_MAX_PKG_LEN 8
#define AT25040_MAX_BYTE 256
#define AT25040_BYTES_PER_READ 2

/* Commands */
#define AT25040_CMD_WREN 0x06	/* set write enable latch */
#define AT25040_CMD_WRDI 0x04	/* reset write enable latch */
#define AT25040_CMD_RDSR 0x05	/* read status register */
#define AT25040_CMD_WRSR 0x01	/* write status register */
#define AT25040_CMD_READ 0x03	/* read data from memory array */
#define AT25040_CMD_WRITE 0x02	/* write data to memory array */

/* Read Status Register Definition */
#define AT25040_STAT_RDY 0x01	/* 0: indicate the device is READY
				 * 1: indicate the write cycle is in progress*/
#define AT25040_STAT_WEN 0x02 /* 0: indicate the device is not WRITE ENABLED
			       * 1: indicate the device is WRITE ENABLED*/

#define RETRY_CNT_THRESHOLD 100

static u8 *sq_spi_pattern_buf = (u8 *)PATTERN_BUF_ADDR;
static u8 *sq_spi_cmpr_buf = (u8 *)(PATTERN_BUF_ADDR + PATTERN_BUF_SIZE);

static int sq_spi_at25040_wait_for_ready(void);
static int sq_spi_at25040_set_write_enable(void);
static int sq_spi_at25040_set_write_enable(void);
static int sq_spi_at25040_wait_for_write_enable(void);
static int sq_spi_at25040_page_write_data(u8 addr, u8 *buf);
static int sq_spi_at25040_read_data(u8 addr, u8 *buf, u16 len);



extern int
sq_spi_eeprom(int autotest)
{
	u8 divisor, addr, i;
	
    /* Reset SPI controller */
	sq_spi_write(
  	sq_spi_read(SQ_SPI_FWCR) |
    SQ_SPI_SOFT_RST, 
    SQ_SPI_FWCR);
            
	/* Configure SPI controller */
	sq_spi_write(
#if defined (CONFIG_PC9223)
		SQ_SPI_MASTER_SIGNAL_CTL_HW |
		SQ_SPI_MASTER_SIGNAL_ACT_NO |
		SQ_SPI_MODE_MASTER |
#endif
		SQ_SPI_SOFT_N_RST |
		SQ_SPI_TXRX_N_RUN |
		SQ_SPI_CLK_IDLE_AST |
		SQ_SPI_TXRX_SIMULT_DIS |
		SQ_SPI_CPOL_0 |
		SQ_SPI_CPHA_0 |
		SQ_SPI_TX_MSB_FIRST |
		SQ_SPI_OP_NORMAL,
		SQ_SPI_FWCR);
			
	/* Enable SPI interrupt */
	sq_spi_write(
		//SQ_SPI_IER_RXFIFO_OVR_INT_EN |
		SQ_SPI_IER_RXFIFO_INT_EN |
		SQ_SPI_IER_RX_COMPLETE_INT_EN,
		SQ_SPI_IER);

	/* Configure FIFO and clear Tx & Rx FIFO */
	sq_spi_write(
		SQ_SPI_RXFIFO_INT_TRIGGER_LEVEL_4 |
		SQ_SPI_TXFIFO_INT_TRIGGER_LEVEL_4 |
		SQ_SPI_RXFIFO_CLR |
		SQ_SPI_TXFIFO_CLR,
		SQ_SPI_FCR);
     
	/* Set the SPI slaves select and characteristic control register */
	divisor = sq_spi_calculate_divisor(3000000); /* 3.0 MHz clock rate */
	
	sq_spi_write(
		SQ_SPI_CHAR_LEN_8 |
		SQ_SPI_SLAVE_SEL_0 |
		SQ_SPI_CLK_DIV(divisor),
		SQ_SPI_SSCR);

	/* Config SPI clock delay */
	sq_spi_write(
		SQ_SPI_PBTXRX_DELAY_NONE |
		SQ_SPI_PBCT_DELAY_NONE |
		SQ_SPI_PBCA_DELAY_1_2,
		SQ_SPI_DLYCR);

	/* Set per char length */
	sq_spi_set_current_mode(MODE_CHAR_LEN_8);
	/* Clear pattern buffer and compare buffer */
	memset((char *)sq_spi_pattern_buf, 0x0, AT25040_MAX_BYTE);
	memset((char *)sq_spi_cmpr_buf, 0x0, AT25040_MAX_BYTE);

	/* Make test pattern */
	sq_spi_make_test_pattern(sq_spi_pattern_buf, AT25040_MAX_BYTE);
	
	/* Write data into at25040 eeprom */
	addr = 0;
	for (i = 0; i < (AT25040_MAX_BYTE / AT25040_MAX_PKG_LEN); i++) {
		if (sq_spi_at25040_page_write_data(addr, &sq_spi_pattern_buf[i*AT25040_MAX_PKG_LEN]))
			return -1;
		addr += AT25040_MAX_PKG_LEN;
	}
	
	/* Read data from at25040 eeprom */
	if (sq_spi_at25040_read_data(addr, sq_spi_cmpr_buf, AT25040_MAX_BYTE))
		return -1;
	return sq_spi_compare_memory(sq_spi_pattern_buf, sq_spi_cmpr_buf, AT25040_MAX_BYTE, autotest);	
}

static int
sq_spi_at25040_wait_for_ready(void)
{
	u8 cmd, stat;
	u32 retry_cnt = 0;

	cmd = AT25040_CMD_RDSR;
	while (1) {
		if (sq_spi_transfer(&cmd, &stat, SET_TX_RX_LEN(1, 1)))
			return -1;
		if (!(stat & AT25040_STAT_RDY))
			break;
		if (retry_cnt > RETRY_CNT_THRESHOLD) {
			printf("Retry count (%d) exceed threshold\n", retry_cnt);
			return -1;
		}
		retry_cnt++;
	}
	return 0;
}

static int
sq_spi_at25040_set_write_enable(void)
{
	u8 cmd;
	cmd = AT25040_CMD_WREN;
	if (sq_spi_transfer(&cmd, NULL,SET_TX_RX_LEN(1, 0)))
		return -1;
	return 0;
}

static int 
sq_spi_at25040_wait_for_write_enable(void)
{
	u8 cmd, stat;
	u32 retry_cnt = 0;

	cmd = AT25040_CMD_RDSR;

	while(1) {
		if (sq_spi_transfer(&cmd, &stat, SET_TX_RX_LEN(1, 1)))
			return -1;
		if (stat & AT25040_STAT_WEN)
			break;
		if (retry_cnt > RETRY_CNT_THRESHOLD) {
			printf("Retry count (%d) exceed threshold\n", retry_cnt);
			return -1;
		}
		retry_cnt++;
	}
	return 0;
}

static int
sq_spi_at25040_page_write_data(u8 addr, u8 *buf)
{
	u8 tx_buf[10] = {0};

	if (sq_spi_at25040_set_write_enable())
		return -1;
	if (sq_spi_at25040_wait_for_write_enable())
		return -1;
	tx_buf[0] = AT25040_CMD_WRITE;
	tx_buf[1] = addr;
	memcpy((char *)&tx_buf[2], (char *)buf, AT25040_MAX_PKG_LEN);
	if(sq_spi_transfer(tx_buf, NULL, SET_TX_RX_LEN(AT25040_MAX_PKG_LEN+2,0)))
		return -1;

	/* Wait for the device to be ready */
	if (sq_spi_at25040_wait_for_ready())
		return -1;
	return 0;
}

static int
sq_spi_at25040_read_data(u8 addr, u8 *buf, u16 len)
{
	u8 cmd[2] = {0};
	
	cmd[0] = AT25040_CMD_READ;
	cmd[1] = addr;
	if (sq_spi_transfer(cmd, buf, SET_TX_RX_LEN(2,len)))
		return -1;
	return 0;
}




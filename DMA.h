
/*******************************************************************************
 *
 *	DMA library
 *
 *	Portions of code from Justin Deltener
 *	http://www.inversereality.org/tutorials/sound%20programming/DMAProgramming.html
 *	Additional information provided by Xavier Leclercq
 *	http://www.xaff.org/GI/dma.html
 */

#ifndef DMA_h
#define DMA_h

/*******************************************************************************
 *
 *	Direct Memory Access port map
 *
 ******************************************************************************/

/* Command bits */

#define DMA_MEMORY_TO_MEMORY_TRANSFER	0x01
#define DMA_ADDRESS_HOLD		0x02	/* For channel 0 or 4 */
#define DMA_CONTROLLER_DISABLE		0x04
#define DMA_COMPRESSED_MODE		0x08
#define DMA_ROTATING_PRIORITY		0x10
#define DMA_EXTENDED_WRITE		0x20
#define DMA_DRQ_LEVEL			0x40
#define DMA_DACK_LEVEL			0x80

/* Controller 1 */

#define DMA_CHANNEL0_ADDRESS		0x00
#define DMA_CHANNEL0_COUNT		0x01
#define DMA_CHANNEL1_ADDRESS		0x02
#define DMA_CHANNEL1_COUNT		0x03
#define DMA_CHANNEL2_ADDRESS		0x04
#define DMA_CHANNEL2_COUNT		0x05
#define DMA_CHANNEL3_ADDRESS		0x06
#define DMA_CHANNEL3_COUNT		0x07
#define DMA_CONTROL1			0x08
#define DMA_CONTROL1_REQUEST		0x09
#define DMA_CONTROL1_CHANNEL_MASK	0x0A
#define DMA_CONTROL1_MODE		0x0B
#define DMA_CONTROL1_RESET_FLIP_FLOP	0x0C
#define DMA_CONTROL1_MASTER_CLEAR	0x0D
#define DMA_CONTROL1_INTERMEDIATE	0x0D
#define DMA_CONTROL1_CLEAR_MASK		0x0E
#define DMA_CONTROL1_MASK		0x0F

/* Controller 2 */

#define DMA_CHANNEL4_ADDRESS		0xC0
#define DMA_CHANNEL4_COUNT		0xC1
#define DMA_CHANNEL5_ADDRESS		0xC2
#define DMA_CHANNEL5_COUNT		0xC3
#define DMA_CHANNEL6_ADDRESS		0xC4
#define DMA_CHANNEL6_COUNT		0xC5
#define DMA_CHANNEL7_ADDRESS		0xC6
#define DMA_CHANNEL7_COUNT		0xC7
#define DMA_CONTROL2			0xD0
#define DMA_CONTROL2_REQUEST		0xD2
#define DMA_CONTROL2_CHANNEL_MASK	0xD4
#define DMA_CONTROL2_MODE		0xD6
#define DMA_CONTROL2_RESET_FLIP_FLOP	0xD8
#define DMA_CONTROL2_MASTER_CLEAR	0xDA
#define DMA_CONTROL2_INTERMEDIATE	0xDA
#define DMA_CONTROL2_CLEAR_MASK		0xDC
#define DMA_CONTROL2_MASK		0xDE

/* Page registers */

#define DMA_CHANNEL0			0x87
#define DMA_CHANNEL1			0x83
#define DMA_CHANNEL2			0x81
#define DMA_CHANNEL3			0x82
#define DMA_CHANNEL4			0x8F	/* Used for memory refresh */
#define DMA_CHANNEL5			0x8B
#define DMA_CHANNEL6			0x89
#define DMA_CHANNEL7			0x8A

/* Bit masks */

#define DMA_CHANNEL_SELECTION_MASK	0x03	/* 00000011 */
#define DMA_CHANNEL0_BIT		0x00	/* ------00 */
#define DMA_CHANNEL1_BIT		0x01	/* ------01 */
#define DMA_CHANNEL2_BIT		0x02	/* ------10 */
#define DMA_CHANNEL3_BIT		0x03	/* ------11 */
#define DMA_CHANNEL4_BIT		0x00	/* ------00 */
#define DMA_CHANNEL5_BIT		0x01	/* ------01 */
#define DMA_CHANNEL6_BIT		0x02	/* ------10 */
#define DMA_CHANNEL7_BIT		0x03	/* ------11 */

#define DMA_TRANSFER_MASK		0x18	/* 00001100 */
#define DMA_VERIFY_TRANSFER_BIT		0x00	/* ----00-- */
#define DMA_WRITE_TRANSFER_BIT		0x04	/* ----01-- */
#define DMA_READ_TRANSFER_BIT		0x08	/* ----10-- */

#define DMA_AUTO_INIT_MASK		0x10	/* 00010000 */
#define DMA_SINGLE_CYCLE_MODE_BIT	0x00	/* ---0---- */
#define DMA_AUTO_INIT_MODE_BIT		0x10	/* ---1---- */

#define DMA_ADDRESS_MASK		0x10	/* 00100000 */
#define DMA_ADDRESS_INCREMENT_BIT	0x20	/* --0----- */
#define DMA_ADDRESS_DECREMENT_BIT	0x40	/* --1----- */

#define DMA_MODE_MASK			0xC0	/* 11000000 */
#define DMA_DEMAND_MODE_BIT		0x00	/* 00------ */
#define DMA_SINGLE_MODE_BIT		0x40	/* 01------ */
#define DMA_BLOCK_MODE_BIT		0x80	/* 10------ */
#define DMA_CASCADE_MODE_BIT		0xC0	/* 11------ */

#endif /* DMA_h */


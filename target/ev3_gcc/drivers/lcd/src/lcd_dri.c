#include "syssvc/syslog.h"
#include <string.h>
#include "gpio_dri.h"
#include <asm-generic/ioctl.h>
#include "ev3.h"
struct mutex {};
struct delayed_work {};
static void mdelay(unsigned long msecs);
int lcd_spi_write(const void *buf, size_t len);
void initialize_lcd_spi();
void initialize_lcd_font();
#define HZ (100) //TODO: need check
#define spi_write(spi,buf,len) lcd_spi_write((buf),(len))
#define virt_to_phys(x) (x)
#define copy_from_user memcpy

/**
 * Reuse 'st7586fb.c' from LEGO MINDSTORMS EV3 source code
 */
#include "../linux/drivers/video/st7586fb.c"

#include <sil.h>
static void mdelay(unsigned long msecs) {
	sil_dly_nse(msecs * 1000 * 1000);
}

#include <kernel.h>
#include <t_syslog.h>
#include "platform.h"
#include "platform_interface_layer.h"
#include "kernel_cfg.h"

static struct spi_device spidev;

extern brickinfo_t global_brick_info;

/**
 * Frame buffers
 */
static bitmap_t lcd_screen;
bitmap_t *on_display_fb;
bitmap_t *lcd_screen_fb;
bitmap_t *ev3rt_console_fb;

static void initialize(intptr_t unused) {
	/**
	 * Initialize frame buffers
	 */
    static bitmap_t console_bitmap;
	static uint8_t lcd_console_fb_vmem[((WIDTH + 2) / 3 * HEIGHT)];
	console_bitmap.height = HEIGHT;
	console_bitmap.width  = WIDTH;
	console_bitmap.pixels = lcd_console_fb_vmem;
    ev3rt_console_fb = &console_bitmap;
    lcd_screen_fb = &lcd_screen;
    on_display_fb = lcd_screen_fb;

    initialize_lcd_font();

	initialize_lcd_spi();
	st7586fb_probe(&spidev);
	st7586fb_ioctl(spidev.drvdata, FB_ST7586_INIT_DISPLAY, 0);
	st7586fb_ioctl(spidev.drvdata, FB_ST7586_START_DISPLAY, 0);

	lcd_screen.pixels = ((struct fb_info *)(spidev.drvdata))->screen_base;
	lcd_screen.height = HEIGHT;
	lcd_screen.width = WIDTH;
	global_brick_info.lcd_screen = &lcd_screen;

	SVC_PERROR(act_tsk(LCD_REFRESH_TSK));
}

static void softreset(intptr_t unused) {
	memset(lcd_screen.pixels, 0, BITMAP_PIXELS_SIZE(lcd_screen.width, lcd_screen.height));
}

void initialize_lcd_dri() {
	initialize(0);
	softreset(0);

	ev3_driver_t driver;
	driver.init_func = NULL;
	driver.softreset_func = softreset;
	SVC_PERROR(platform_register_driver(&driver));
}

static void* current_video_memory;

void lcd_refresh_tsk(intptr_t unused) {
	struct st7586fb_par *par = ((struct fb_info *)(spidev.drvdata))->par;

	while (1) {
#if 0 // For test
		struct fb_info *info = spidev.drvdata;
		u8 *vmem = info->screen_base;
		static int i = 0;
		vmem[i++] = 0xFF;
		if (i == (WIDTH+2)/3*HEIGHT) i = 0;
#endif
		st7586_set_addr_win(par, 0, 0, WIDTH, HEIGHT);
		st7586_write_cmd(par, ST7586_RAMWR);

		/* Blast frame buffer to ST7586 internal display RAM */
		ER ercd = st7586_write_data_buf(par, on_display_fb->pixels, (WIDTH + 2) / 3 * HEIGHT);
		assert(ercd == E_OK);

		dly_tsk(1000000U / (LCD_FRAME_RATE));
	}
}

/**
 * Implementation of extended service calls
 */

/**
 * Interface for CSL
 */

void lcd_set_framebuffer(const bitmap_t *fb) {
    current_video_memory = fb->pixels;
}


/*
 * motor_dri.h
 *
 *  Created on: Sep 20, 2013
 *      Author: liyixiao
 */

#pragma once

#define LCD_SPI_INT (SYS_INT_SPINT1)

void initialize_lcd_dri();
void lcd_spi_isr(intptr_t unused);
void lcd_refresh_tsk(intptr_t unused);

/**
 * Interface for Core Services Layer
 */

extern bitmap_t *on_display_fb;    // Current framebuffer to show on display
extern bitmap_t *lcd_screen_fb;    // Framebuffer for (default) LCD screen
extern bitmap_t *ev3rt_console_fb; // Framebuffer for EV3RT Console


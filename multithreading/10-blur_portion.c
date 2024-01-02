#include "multithreading.h"
#include <sys/types.h>

/**
 * blur_portion - blurs a portion of an image using a GAUSSIAN BLUR
 *
 * @portion: pointer to data struct blur_portion_t
 */

void blur_portion(blur_portion_t const *portion)
{
	size_t x = 0, y = 0;

	/* sanity checks */
	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	/* let's loop over all the pixels in given portion */

	for (y = portion->y; y < portion->y + portion->h; y++)
		for (x = portion->x; x < portion->x + portion->w; x++)
			perform_bluring(portion->img, portion->img_blur, portion->kernel, x, y);
}

void perform_bluring(const img_t *img, img_t *new_img, const kernel_t *kernel,
					 size_t x, size_t y)
{
	float divider = 0, total_r = 0, total_g = 0, total_b = 0;
	size_t pos_i = 0, radius = 0, i = 0, j;
	ssize_t pos_x = 0, pos_y = 0;


	radius = kernel->size / 2;
	/* loop over the kernel */
	for (pos_y = (ssize_t)y - radius; i < kernel->size; i++, pos_y++)
		for (j = 0, pos_x = (ssize_t)x - radius; j < kernel->size; j++, pos_x++)
		{
			if (pos_x >= 0 && (size_t)pos_x < img->w &&
				pos_y >= 0 && (size_t)pos_y < img->h)
			{
				/* Add current pixel to the average */
				divider += kernel->matrix[i][j];
				pos_i = (pos_y * img->w) + pos_x;
				total_r += img->pixels[pos_i].r * kernel->matrix[i][j];
				total_g += img->pixels[pos_i].g * kernel->matrix[i][j];
				total_b += img->pixels[pos_i].b * kernel->matrix[i][j];
			}
		}

	/* set the new value of rgb levers in the new_img*/
	pos_i = (y * new_img->w) + x;
	new_img->pixels[pos_i].r = (uint8_t)(total_r / divider);
	new_img->pixels[pos_i].g = (uint8_t)(total_g / divider);
	new_img->pixels[pos_i].b = (uint8_t)(total_b / divider);
}

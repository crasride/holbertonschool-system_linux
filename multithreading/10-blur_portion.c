#include "multithreading.h"
#include <sys/types.h>

void aply_convolution(const img_t *img, img_t *blur_img, const kernel_t *kernel,
					size_t x, size_t y);

void blur_portion(blur_portion_t const *portion)
{
	size_t x = 0, y = 0;

	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	for (y = portion->y; y < portion->y + portion->h; y++)
		for (x = portion->x; x < portion->x + portion->w; x++)
			aply_convolution(portion->img, portion->img_blur, portion->kernel, x, y);
}

void aply_convolution(const img_t *img, img_t *blur_img, const kernel_t *kernel,
						size_t x, size_t y)
{
	float divider = 0, total_r = 0, total_g = 0, total_b = 0;
	size_t img_blur_index = 0, half_kernel_size = 0, i = 0, j;
	ssize_t pos_x = 0, pos_y = 0;

	half_kernel_size = kernel->size / 2;
	for (pos_y = (ssize_t)y - half_kernel_size; i < kernel->size; i++, pos_y++)
		for (j = 0, pos_x = (ssize_t)x - half_kernel_size; j < kernel->size; j++, pos_x++)
		{
			if (pos_x >= 0 && (size_t)pos_x < img->w &&
				pos_y >= 0 && (size_t)pos_y < img->h)
			{
				/* value the divider */
				divider += kernel->matrix[i][j];
				img_blur_index = (pos_y * img->w) + pos_x;
				total_r += img->pixels[img_blur_index].r * kernel->matrix[i][j];
				total_g += img->pixels[img_blur_index].g * kernel->matrix[i][j];
				total_b += img->pixels[img_blur_index].b * kernel->matrix[i][j];
			}
		}

	/* value of rgb levers in the blur_img*/
	img_blur_index = (y * blur_img->w) + x;
	blur_img->pixels[img_blur_index].r = (uint8_t)(total_r / divider);
	blur_img->pixels[img_blur_index].g = (uint8_t)(total_g / divider);
	blur_img->pixels[img_blur_index].b = (uint8_t)(total_b / divider);
}

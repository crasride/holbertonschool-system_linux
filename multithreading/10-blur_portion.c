#include "multithreading.h"
#include <sys/types.h>

void m_bluring(const img_t *img, img_t *img_blur, const kernel_t *kernel,
					size_t x, size_t y)
{
	size_t i = 0, j = 0;
	size_t img_blur_index = 0, half_kernel_size = 0;
	float divider = 0, total_r = 0, total_g = 0, total_b = 0;


	half_kernel_size = kernel->size / 2;

	for (i = y > half_kernel_size ? y - half_kernel_size : 0; i < (y + half_kernel_size < img->h ? y + half_kernel_size : img->h); i++)
	{
		for (j = x > half_kernel_size ? x - half_kernel_size : 0; j < (x + half_kernel_size < img->w ? x + half_kernel_size : img->w); j++)
		{
			size_t img_row = i;
			size_t img_col = j;

			float kernel_value = kernel->matrix[i - (y - half_kernel_size)][j - (x - half_kernel_size)];

			divider += kernel_value;
			img_blur_index = (img_row * img->w) + img_col;
			total_r += img->pixels[img_blur_index].r * kernel_value;
			total_g += img->pixels[img_blur_index].g * kernel_value;
			total_b += img->pixels[img_blur_index].b * kernel_value;
		}
	}

	img_blur_index = (y * img_blur->w) + x;
	img_blur->pixels[img_blur_index].r = (uint8_t)(total_r / divider);
	img_blur->pixels[img_blur_index].g = (uint8_t)(total_g / divider);
	img_blur->pixels[img_blur_index].b = (uint8_t)(total_b / divider);
}

void blur_portion(blur_portion_t const *portion)
{
	size_t x = 0, y = 0;

	/* Check if the portion is valid */
	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	for (y = portion->y; y < portion->y + portion->h; y++)
	{
		for (x = portion->x; x < portion->x + portion->w; x++)
		{
			m_bluring(portion->img, portion->img_blur, portion->kernel, x, y);
		}
	}
}

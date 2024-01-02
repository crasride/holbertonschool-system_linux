#include "multithreading.h"

static void accumulate_color(const img_t *img, const kernel_t *kernel,
							size_t x, size_t y,
							float *sum_r, float *sum_g, float *sum_b)
{
	size_t radius;
	size_t kernel_row, kernel_col;
	int img_row, img_col;

	*sum_r = 0.0;
	*sum_g = 0.0;
	*sum_b = 0.0;

	radius = kernel->size / 2;

	for (kernel_row = 0; kernel_row < kernel->size; kernel_row++)
	{
		for (kernel_col = 0; kernel_col < kernel->size; kernel_col++)
		{
			img_row = (int)(y - radius + kernel_row);
			img_col = (int)(x - radius + kernel_col);

			if (img_row >= 0 && img_row < (int)img->h &&
				img_col >= 0 && img_col < (int)img->w)
			{
				float kernel_value = kernel->matrix[kernel_row][kernel_col];
				*sum_r += kernel_value * img->pixels[(size_t)img_row * img->w + (size_t)img_col].r;
				*sum_g += kernel_value * img->pixels[(size_t)img_row * img->w + (size_t)img_col].g;
				*sum_b += kernel_value * img->pixels[(size_t)img_row * img->w + (size_t)img_col].b;
			}
		}
	}
}

void blur_portion(const blur_portion_t *portion)
{
	size_t dest_row, dest_col;
	size_t img_blur_index = 0;

	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	for (dest_row = 0; dest_row < portion->h; dest_row++)
	{
		for (dest_col = 0; dest_col < portion->w; dest_col++)
		{
			float sum_r, sum_g, sum_b;
			accumulate_color(portion->img, portion->kernel,
							portion->x + dest_col, portion->y + dest_row,
							&sum_r, &sum_g, &sum_b);

			img_blur_index = (portion->y + dest_row) * portion->img_blur->w + (portion->x + dest_col);
			portion->img_blur->pixels[img_blur_index].r = (uint8_t)(sum_r);
			portion->img_blur->pixels[img_blur_index].g = (uint8_t)(sum_g);
			portion->img_blur->pixels[img_blur_index].b = (uint8_t)(sum_b);
		}
	}
}




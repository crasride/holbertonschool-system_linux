#include "multithreading.h"
#include <sys/types.h>

static void accumulate_color(blur_portion_t const *portion, size_t dest_row,
							size_t dest_col, float *sum_r, float *sum_g,
							float *sum_b)
{
	size_t kernel_half_size = portion->kernel->size / 2;
	size_t kernel_row, kernel_col;

	*sum_r = 0.0;
	*sum_g = 0.0;
	*sum_b = 0.0;

	for (kernel_row = 0; kernel_row < portion->kernel->size; kernel_row++)
	{
		for (kernel_col = 0; kernel_col < portion->kernel->size; kernel_col++)
		{
			int img_row = (int)(dest_row - kernel_half_size + kernel_row);
			int img_col = (int)(dest_col - kernel_half_size + kernel_col);

			if (img_col >= 0 && img_col < (int)portion->img->w &&
				img_row >= 0 && img_row < (int)portion->img->h)
			{
				float kernel_value = portion->kernel->matrix[kernel_row][kernel_col];
				*sum_r += kernel_value * portion->img->pixels[img_row * portion->img->w + img_col].r;
				*sum_g += kernel_value * portion->img->pixels[img_row * portion->img->w + img_col].g;
				*sum_b += kernel_value * portion->img->pixels[img_row * portion->img->w + img_col].b;
			}
		}
	}
}

void blur_portion(blur_portion_t const *portion)
{
	size_t dest_row, dest_col;
	float sum_r, sum_g, sum_b;
	size_t img_blur_index = 0;

	for (dest_row = 0; dest_row < portion->h; dest_row++)
	{
		for (dest_col = 0; dest_col < portion->w; dest_col++)
		{
			accumulate_color(portion, dest_row, dest_col, &sum_r, &sum_g, &sum_b);

			img_blur_index = (portion->y + dest_row) * portion->img_blur->w + (portion->x + dest_col);

			if (dest_col < portion->img_blur->w && dest_row < portion->img_blur->h)
			{
				portion->img_blur->pixels[img_blur_index].r = (uint8_t)sum_r;
				portion->img_blur->pixels[img_blur_index].g = (uint8_t)sum_g;
				portion->img_blur->pixels[img_blur_index].b = (uint8_t)sum_b;
			}
		}
	}
}








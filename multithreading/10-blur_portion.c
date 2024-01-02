
#include "multithreading.h"


void apply_convolution(blur_portion_t const *portion, size_t i, size_t j)
{
	size_t kernel_size = portion->kernel->size;
	size_t img_width = portion->img->w;
	size_t img_height = portion->img->h;
	size_t x, y;
	size_t result_index = i * img_width + j;
	size_t k = 0, l = 0;

	float sum_r = 0.0, sum_g = 0.0, sum_b = 0.0;

	for (k = 0; k < kernel_size; k++)
	{
		for (l = 0; l < kernel_size; l++)
		{
			x = i + k - (kernel_size / 2);
			y = j + l - (kernel_size / 2);

			if (x < img_width && y < img_height)
			{
				size_t pixel_index = x * img_width + y;
				float kernel_value = portion->kernel->matrix[k][l];

				sum_r += kernel_value * portion->img->pixels[pixel_index].r;
				sum_g += kernel_value * portion->img->pixels[pixel_index].g;
				sum_b += kernel_value * portion->img->pixels[pixel_index].b;
			}
		}
	}

	result_index = i * img_width + j;
	portion->img_blur->pixels[result_index].r = (uint8_t)sum_r;
	portion->img_blur->pixels[result_index].g = (uint8_t)sum_g;
	portion->img_blur->pixels[result_index].b = (uint8_t)sum_b;
}


void blur_portion(blur_portion_t const *portion)
{
	size_t i, j;

	for (i = portion->x; i < portion->x + portion->w; i++)
	{
		for (j = portion->y; j < portion->y + portion->h; j++)
		{
			apply_convolution(portion, i, j);
		}
	}
}

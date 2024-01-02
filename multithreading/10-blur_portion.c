#include "multithreading.h"


void perform_bluring(const img_t *img, img_t *new_img, const kernel_t *kernel,
					size_t x, size_t y)
{
	float divider = 0, total_r = 0, total_g = 0, total_b = 0;
	size_t new_img_index = 0, half_kernel_size = 0;

	half_kernel_size = kernel->size / 2;

	for (size_t i = y > half_kernel_size ? y - half_kernel_size : 0; i < (y + half_kernel_size < img->h ? y + half_kernel_size : img->h); i++)
	{
		for (size_t j = x > half_kernel_size ? x - half_kernel_size : 0; j < (x + half_kernel_size < img->w ? x + half_kernel_size : img->w); j++)
		{
			size_t img_row = i;
			size_t img_col = j;

			float kernel_value = kernel->matrix[i - (y - half_kernel_size)][j - (x - half_kernel_size)];

			divider += kernel_value;
			new_img_index = (img_row * img->w) + img_col;
			total_r += img->pixels[new_img_index].r * kernel_value;
			total_g += img->pixels[new_img_index].g * kernel_value;
			total_b += img->pixels[new_img_index].b * kernel_value;
		}
	}

	new_img_index = (y * new_img->w) + x;
	new_img->pixels[new_img_index].r = (uint8_t)(total_r / divider);
	new_img->pixels[new_img_index].g = (uint8_t)(total_g / divider);
	new_img->pixels[new_img_index].b = (uint8_t)(total_b / divider);
}

void blur_portion(blur_portion_t const *portion)
{
	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	for (size_t y = portion->y; y < portion->y + portion->h; y++)
	{
		for (size_t x = portion->x; x < portion->x + portion->w; x++)
		{
			perform_bluring(portion->img, portion->img_blur, portion->kernel, x, y);
		}
	}
}

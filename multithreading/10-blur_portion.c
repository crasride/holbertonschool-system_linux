#include "multithreading.h"

/**
* blur_portion - Blurs a portion of an image using a kernel matrix
*
* @portion: An instance of the struct blur_portion_s
*/
void blur_portion(blur_portion_t const *portion)
{
	size_t x = 0, y = 0;

	/* Check if the structures are valid */
	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
	{
		fprintf(stderr, "Error: At least one of the structures is null.\n");
		return;
	}

	/* Check if the slice dimensions are valid */
	if (portion->x >= portion->img->w || portion->y >= portion->img->h ||
		portion->w > portion->img->w || portion->h > portion->img->h ||
		portion->x + portion->w > portion->img->w || portion->y + portion->h >
		portion->img->h)
	{
		fprintf(stderr, "Error: Invalid portion dimensions.\n");
		return;
	}

	/* Iterate over rows in the portion */
	for (y = portion->y; y < portion->y + portion->h; y++)
		/* Iterate over columns in the portion */
		for (x = portion->x; x < portion->x + portion->w; x++)
			apply_convolution(portion->img, portion->img_blur, portion->kernel, x, y);
}

/**
* apply_convolution - Applies a convolution kernel to an image portion
* @img: Source image
* @blur_img: Destination image
* @kernel: Convolution kernel to use
* @x: X position of the portion in the image
* @y: Y position of the portion in the image
*/
void apply_convolution(const img_t *img, img_t *blur_img,
						const kernel_t *kernel, size_t x, size_t y)
{
	float divider = 0, total_r = 0, total_g = 0, total_b = 0;
	size_t img_blur_index = 0, half_kernel_size = 0, i = 0, j;
	/* positions being iterated over during the convolution process */
	ssize_t current_column = 0, current_row = 0;

	/* Half size of the convolution kernel */
	half_kernel_size = kernel->size / 2;
	/* Iterate over rows the ker */
	for (current_row = (ssize_t)y - half_kernel_size; i < kernel->size; i++,
		current_row++)
		/* Iterate over columns the kernel */
		for (j = 0, current_column = (ssize_t)x - half_kernel_size; j < kernel->size;
			j++, current_column++)
		{
			if (current_column >= 0 && (size_t)current_column < img->w &&
				current_row >= 0 && (size_t)current_row < img->h)
			{
				/* value the divider */
				divider += kernel->matrix[i][j];
				img_blur_index = (current_row * img->w) + current_column;
				total_r += img->pixels[img_blur_index].r * kernel->matrix[i][j];
				total_g += img->pixels[img_blur_index].g * kernel->matrix[i][j];
				total_b += img->pixels[img_blur_index].b * kernel->matrix[i][j];
			}
		}

	/* Assign the result to the blur image */
	img_blur_index = (y * blur_img->w) + x;
	/* Normalize by channel before assigning to result pixel*/
	blur_img->pixels[img_blur_index].r = (uint8_t)(total_r / divider);
	blur_img->pixels[img_blur_index].g = (uint8_t)(total_g / divider);
	blur_img->pixels[img_blur_index].b = (uint8_t)(total_b / divider);
}

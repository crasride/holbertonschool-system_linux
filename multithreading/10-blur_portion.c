#include "multithreading.h"

/**
* accumulate_color - Acumula los componentes de color ponderados
*
* @portion: Puntero a la estructura blur_portion_t
* @dest_row: Índice de fila en la imagen de destino
* @dest_col: Índice de columna en la imagen de destino
* @sum_red: Suma acumulativa de los componentes de color rojo
* @sum_green: Suma acumulativa de los componentes de color verde
* @sum_blue: Suma acumulativa de los componentes de color azul
*/
static void accumulate_color(blur_portion_t const *portion, size_t dest_row,
							size_t dest_col, float *sum_red, float *sum_green,
							float *sum_blue)
{
	size_t kernel_half_size = portion->kernel->size / 2;
	size_t kernel_row, kernel_col, img_row, img_col;

	*sum_red = 0.0;
	*sum_green = 0.0;
	*sum_blue = 0.0;

	for (kernel_row = 0; kernel_row < portion->kernel->size; kernel_row++)
	{
		for (kernel_col = 0; kernel_col < portion->kernel->size; kernel_col++)
		{
			img_row = portion->y + dest_row - kernel_half_size + kernel_row;
			img_col = portion->x + dest_col - kernel_half_size + kernel_col;

			/* Make sure the ratios are within limits. */
			if (img_col < portion->img->w && img_row < portion->img->h)
			{
				float kernel_value = portion->kernel->matrix[kernel_row][kernel_col];
				*sum_red += kernel_value * portion->img->pixels[img_row *
				portion->img->w + img_col].r;
				*sum_green += kernel_value * portion->img->pixels[img_row *
				portion->img->w + img_col].g;
				*sum_blue += kernel_value * portion->img->pixels[img_row *
				portion->img->w + img_col].b;
			}
		}
	}
}

/**
* blur_portion - Blur a portion of an image using a Gaussian Blur
*
* @portion: Pointer to the blur portion t structure that describes the portion
* to be blurred
*/
void blur_portion(blur_portion_t const *portion)
{
	size_t dest_row, dest_col;
	float sum_red, sum_green, sum_blue;

	for (dest_row = 0; dest_row < portion->h; dest_row++)
	{
		for (dest_col = 0; dest_col < portion->w; dest_col++)
		{
			accumulate_color(portion, dest_row, dest_col, &sum_red, &sum_green,
							&sum_blue);

			/* Make sure the indices are within the boundaries of the image */
			if (dest_col < portion->img_blur->w && dest_row < portion->img_blur->h)
			{
				portion->img_blur->pixels[(portion->y + dest_row) * portion->
				img_blur->w + (portion->x + dest_col)].r = (uint8_t)sum_red;
				portion->img_blur->pixels[(portion->y + dest_row) * portion->
				img_blur->w + (portion->x + dest_col)].g = (uint8_t)sum_green;
				portion->img_blur->pixels[(portion->y + dest_row) * portion->
				img_blur->w + (portion->x + dest_col)].b = (uint8_t)sum_blue;
			}
		}
	}
}

/*
* kernel_0.knl
* kernel de convolución es una matriz que se utiliza para realizar operaciones
* como desenfoque, nitidez, detección de bordes, entre otras.
*/

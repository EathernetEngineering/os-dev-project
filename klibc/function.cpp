#include "klibc/function.hpp"

// Forward declerations
int64_t partition(int32_t array[], int64_t low, int64_t high);
int64_t partition(uint32_t array[], int64_t low, int64_t high);
int64_t partition(int64_t array[], int64_t low, int64_t high);
int64_t partition(uint64_t array[], int64_t low, int64_t high);
int32_t *quicksort(int32_t *array, int64_t low, int64_t high);
uint32_t *quicksort(uint32_t *array, int64_t low, int64_t high);
int64_t *quicksort(int64_t *array, int64_t low, int64_t high);
uint64_t *quicksort(uint64_t *array, int64_t low, int64_t high);

int32_t *sort(int32_t *array, size_t count)
{
	return quicksort(array, 0, count - 1);
}

uint32_t *sort(uint32_t *array, size_t count)
{
	return quicksort(array, 0, count - 1);
}

int64_t *sort(int64_t *array, size_t count)
{
	return quicksort(array, 0, count - 1);
}

uint64_t *sort(uint64_t *array, size_t count)
{
	return quicksort(array, 0, count - 1);
}

int64_t partition(int32_t array[], int64_t low , int64_t high)
{
	int32_t pivot = array[high];
	int64_t i = low - 1;

	for (int64_t j = low; j < high; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[high]);
	return i + 1;
}

int64_t partition(uint32_t array[], int64_t low , int64_t high)
{
	uint32_t pivot = array[high];
	int64_t i = low - 1;

	for (int64_t j = low; j < high; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[high]);
	return i + 1;
}

int64_t partition(int64_t array[], int64_t low , int64_t high)
{
	int64_t pivot = array[high];
	int64_t i = low - 1;

	for (int64_t j = low; j < high; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[high]);
	return i + 1;
}

int64_t partition(uint64_t array[], int64_t low , int64_t high)
{
	uint64_t pivot = array[high];
	int64_t i = low - 1;

	for (int64_t j = low; j < high; j++)
	{
		if (array[j] <= pivot)
		{
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[high]);
	return i + 1;
}

int32_t *quicksort(int32_t array[], int64_t low, int64_t high)
{
	if (low >= high) return array;
	int64_t part = partition(array, low, high);
	quicksort(array, low, part - 1);
	quicksort(array, part + 1, high);
	return array;
}

uint32_t *quicksort(uint32_t array[], int64_t low, int64_t high)
{
	if (low >= high) return array;
	int64_t part = partition(array, low, high);
	quicksort(array, low, part - 1);
	quicksort(array, part + 1, high);
	return array;
}

int64_t *quicksort(int64_t array[], int64_t low, int64_t high)
{
	if (low >= high) return array;
	int64_t part = partition(array, low, high);
	quicksort(array, low, part - 1);
	quicksort(array, part + 1, high);
	return array;
}

uint64_t *quicksort(uint64_t array[], int64_t low, int64_t high)
{
	if (low >= high) return array;
	int64_t part = partition(array, low, high);
	quicksort(array, low, part - 1);
	quicksort(array, part + 1, high);
	return array;
}


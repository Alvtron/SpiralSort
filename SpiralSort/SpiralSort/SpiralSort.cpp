// SpiralSort.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>

#define M_PI 3.14159265358979323846

class Coordinate {
public:

	double x;

	double y;

	int order = -1;

	Coordinate() {
		this->x = -1;
		this->y = -1;
	}

	Coordinate(double x, double y) {
		this->x = x;
		this->y = y;
	}

	void print() {
		std::cout << "(" << x << "," << y << "),";
	}

	void printX() {
		std::cout << x << std::endl;
	}

	void printY() {
		std::cout << y << std::endl;
	}

	static void saveToFile(std::string filename, std::vector<Coordinate>* coordinates, std::vector<Coordinate>* unsorted_coordinates = nullptr)
	{
		std::ofstream _file;

		try {
			_file.open(filename);

			if (unsorted_coordinates != nullptr)
				_file << "Sorted_X, Sorted_Y, Unsorted_X, Unsorted_Y" << std::endl;
			else
				_file << "Sorted_X, Sorted_Y" << std::endl;

			for (int i = 0; i < coordinates->size(); i++)
			{
				_file << coordinates->at(i).x << "," << coordinates->at(i).y;

				if (unsorted_coordinates != nullptr)
					_file << "," << unsorted_coordinates->at(i).x << "," << unsorted_coordinates->at(i).y;

				_file << std::endl;
			}
		}
		catch (const std::ofstream::failure& e) {
			std::cout << "Could not create file. No data saved.";
		}

		_file.close();
	}

	bool operator==(const Coordinate& b) {
		return (this->x == b.x && this->y == b.y);
	}
};

struct by_order {
	bool operator()(Coordinate const &a, Coordinate const &b) {
		return a.order < b.order;
	}
};

/* Logging */
void log(const char* message)
{
	std::cout << message << std::endl;
}

/* Generate random coordinates */
std::vector<Coordinate> generateCircularMappedCordinates(double diameter, int rows, double point_gap) {

	std::vector<Coordinate> coordinates;

	double radius = abs(diameter / 2.0);

	for (double y = radius; y >= -radius; y -= radius / (double)rows)
		for (double x = -radius; x <= radius; x += point_gap)
			if (x * x + y * y < radius * radius)
				coordinates.push_back(Coordinate(x, y));

	return coordinates;
}

/* Generate outward spiral */
std::vector<Coordinate> generateSpiral(int MAX_POINTS, double gap_width) {
	std::vector<Coordinate> coordinates_spiral;

	double x = 0;
	double y = 0;
	double angle = 0.0f;

	double a = gap_width, b = gap_width;

	for (double i = 0.0; i < MAX_POINTS; i += 1.0)
	{
		angle = 0.1 * i;
		x = (a + b * angle) * cos(angle) - 1.0;
		y = (a + b * angle) * sin(angle);

		coordinates_spiral.push_back(Coordinate(x, y));
	}
	return coordinates_spiral;
}

/* Sort with coordinates and spiral */
void spiralSort(std::vector<Coordinate>* coordinates, int MAX_POINTS, double gap_width)
{
	// Generate spiral
	std::vector<Coordinate> spiral = generateSpiral(MAX_POINTS, gap_width);

	for (int c_index = 0; c_index < coordinates->size(); c_index++)
	{
		double dxdy = -1.0;

		for (int i = 0; i < spiral.size(); i++)
		{
			double _dx = coordinates->at(c_index).x - spiral.at(i).x;
			double _dy = coordinates->at(c_index).y - spiral.at(i).y;
			double _dxdy = abs(_dx) + abs(_dy);

			if ((dxdy == -1.0) || (_dxdy <= dxdy))
			{
				dxdy = _dxdy;
				coordinates->at(c_index).order = i;
			}
		}
	}

	// Sorting
	std::sort(coordinates->begin(), coordinates->end(), by_order());
}

// ENTRY POINT
int main()
{
	log("Generating circular-mapped coordinates...");
	std::vector<Coordinate> circular_mapped_coordinates = generateCircularMappedCordinates(440.0, 8, 25.0);
	log("Completed\n");

	log("\nSorting circular-mapped coordinates using SpiralSort...");
	std::vector<Coordinate> circular_mapped_coordinates_spiralsorted = circular_mapped_coordinates;
	spiralSort(&circular_mapped_coordinates_spiralsorted, 1000, 4.0);
	log("Completed!\n");

	log("\nSaving Coordinates to files X\n");
	Coordinate::saveToFile("Output/spiral.dat", &generateSpiral(1000, 4.0));
	Coordinate::saveToFile("Output/spiralsort_circular_mapped_coordinates.dat", &circular_mapped_coordinates_spiralsorted, &circular_mapped_coordinates);

	return 0;
}
#pragma once
#include <iostream>
#include <vector>
#include "png++/png.hpp"
#include <thread>

#define PI 3.14159265358979323846 
int const RTI_res_width = 3840;
int const RTI_res_height = 2160;
struct vec3
{
	float x = 0, y = 0, z = 0;

	float magnitude()
	{
		return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
};
//function declarations
float vec3multiply(const vec3 vec_0, const vec3 vec_1);
float vec3Angle(const vec3 vec_0, const vec3 vec_1);
vec3 vec3average(vec3 vec_0, vec3 vec_1);
vec3 vec3substract(vec3 vec_0, vec3 vec_1);

struct rectangularFace
{
public:
	vec3 pointLarge;
	vec3 pointSmall;
	vec3 normal;

	bool cull(vec3 &origin)
	{
		vec3 difference = vec3substract(pointLarge, origin);
		if (vec3Angle(normal, difference) < PI / 2)
		{
			std::cout << vec3Angle(normal, difference) << std::endl;
			return true;
		}
		else
			return false;
	}
};

struct CubeWorldObject
{
	unsigned int blockID = 0;
	std::vector<rectangularFace> faces;
	vec3 pos;
	vec3 dir;
	void blockSetup()
	{
		if (blockID == 0)
		{
			rectangularFace face;
			face.normal.x = 1;
			face.normal.y = 0;
			face.normal.z = 0;
			face.pointSmall.x = pos.x + 0.5f;
			face.pointSmall.y = pos.y - 0.5f;
			face.pointSmall.z = pos.z - 0.5f;
			face.pointLarge.x = pos.x + 0.5f;
			face.pointLarge.y = pos.y + 0.5f;
			face.pointLarge.z = pos.z + 0.5f;
			faces.push_back(face);
			face.normal.x = -1;
			face.normal.y = 0;
			face.normal.z = 0;
			face.pointSmall.x = pos.x - 0.5f;
			face.pointSmall.y = pos.y - 0.5f;
			face.pointSmall.z = pos.z - 0.5f;
			face.pointLarge.x = pos.x - 0.5f;
			face.pointLarge.y = pos.y + 0.5f;
			face.pointLarge.z = pos.z + 0.5f;
			faces.push_back(face);
			face.normal.x = 0;
			face.normal.y = 1;
			face.normal.z = 0;
			face.pointSmall.x = pos.x - 0.5f;
			face.pointSmall.y = pos.y + 0.5f;
			face.pointSmall.z = pos.z - 0.5f;
			face.pointLarge.x = pos.x + 0.5f;
			face.pointLarge.y = pos.y + 0.5f;
			face.pointLarge.z = pos.z + 0.5f;
			faces.push_back(face);
			face.normal.x = 0;
			face.normal.y = -1;
			face.normal.z = 0;
			face.pointSmall.x = pos.x - 0.5f;
			face.pointSmall.y = pos.y - 0.5f;
			face.pointSmall.z = pos.z - 0.5f;
			face.pointLarge.x = pos.x + 0.5f;
			face.pointLarge.y = pos.y - 0.5f;
			face.pointLarge.z = pos.z + 0.5f;
			faces.push_back(face);
			face.normal.x = 0;
			face.normal.y = 0;
			face.normal.z = 1;
			face.pointSmall.x = pos.x - 0.5f;
			face.pointSmall.y = pos.y - 0.5f;
			face.pointSmall.z = pos.z + 0.5f;
			face.pointLarge.x = pos.x + 0.5f;
			face.pointLarge.y = pos.y + 0.5f;
			face.pointLarge.z = pos.z + 0.5f;
			faces.push_back(face);
			face.normal.x = 0;
			face.normal.y = 0;
			face.normal.z = -1;
			face.pointSmall.x = pos.x - 0.5f;
			face.pointSmall.y = pos.y - 0.5f;
			face.pointSmall.z = pos.z - 0.5f;
			face.pointLarge.x = pos.x + 0.5f;
			face.pointLarge.y = pos.y + 0.5f;
			face.pointLarge.z = pos.z - 0.5f;
			faces.push_back(face);
		}
	}
};

struct RTI
{
public:
	vec3 pos;
	vec3 vec;
	vec3 intersect;
	vec3 curIntersect;
	int rayIntersects = 0;
	int threadsCount = 0;
private:
	vec3 raypos;
	float factor  = 0;
	bool hit = false;
public:

	void RayQuadIntersect(std::vector<CubeWorldObject> objects, png::image<png::rgb_pixel> &image, int thread)
	{
		vec3 rayvec;
		rayvec = vec;
		hit = false;
		signed int x = 0;
		signed int y = 0;
		int startX;
		int startY;
		int endX;
		int endY;
		int brightness;

		if (thread == 0)
		{
			startX = 0;
			startY = 0;
			endX = image.get_width() / 2;
			endY = image.get_height() / 2;
		}
		else if (thread == 1)
		{
			startX = image.get_width() / 2;
			startY = 0;
			endX = image.get_width();
			endY = image.get_height() / 2;
		}
		else if (thread == 2)
		{
			startX = 0;
			startY = image.get_height() / 2;
			endX = image.get_width() / 2;
			endY = image.get_height();
		}
		else if (thread == 3)
		{
			startX = image.get_width() / 2;
			startY = image.get_height() / 2;
			endX = image.get_width();
			endY = image.get_height();
		}

		int test = 0;
		for (png::uint_32 y = startY; y < endY; y++) {
			for (png::uint_32 x = startX; x < endX; x++) {

				rayvec.y = x - ((float)(RTI_res_width - 1) / 2);
				rayvec.z = ((float)(RTI_res_height - 1) / 2) - y;
				int brightness = 0;

				RayCast(objects, brightness, rayvec);

				//if(brightness > 10)
					//std::cout << brightness << std::endl;
				image[y][x] = png::rgb_pixel(brightness, brightness, brightness);
			}
		}
	}
private:
	void RayCast(std::vector<CubeWorldObject> objects, int &brightness, vec3 const &rayvec)
	{
		for (const CubeWorldObject object : objects)
		{
			for (const rectangularFace face : object.faces)
			{
					if (face.normal.x == 1 || face.normal.x == -1)
					{
						factor = face.pointLarge.x - raypos.x;
						intersect.x = face.pointLarge.x;
						intersect.y = rayvec.y / rayvec.x * factor;
						intersect.z = rayvec.z / rayvec.x * factor;
						//axis normalized 
						//now off to trace some rays
						//std::cout << "so far so good" << std::endl;
						if (intersect.y <= face.pointLarge.y && intersect.y >= face.pointSmall.y)
						{
							//std::cout << "Even better" << std::endl;
							if (intersect.z <= face.pointLarge.z && intersect.z >= face.pointSmall.z)
							{
								//std::cout << "test";
									brightness += 50;
							}
						}
						//Thats it, its that easy
					}
					else if (face.normal.y == 1 || face.normal.y == -1)
					{
						//std::cout << "test Y" << std::endl;
						factor = face.pointLarge.y - raypos.y;
						intersect.x = rayvec.x / rayvec.y * factor;
						intersect.y = face.pointLarge.y;
						intersect.z = rayvec.z / rayvec.y * factor;
						//axis normalized 
						//now off to trace some rays
						//std::cout << "so far so good" << std::endl;
						if (intersect.x <= face.pointLarge.x && intersect.x >= face.pointSmall.x)
						{
							//std::cout << "Even better" << std::endl;
							if (intersect.z <= face.pointLarge.z && intersect.z >= face.pointSmall.z)
							{
								//std::cout << "Hey, it hit, nice";
								brightness += 50;
							}
						}
						//Thats it, its that easy
					}
					else if (face.normal.z == 1 || face.normal.z == -1)
					{
						factor = face.pointLarge.z - raypos.z;
						intersect.x = rayvec.x / rayvec.z * factor;
						intersect.y = rayvec.y / rayvec.z * factor;
						intersect.z = face.pointLarge.z;
						//axis normalized 
						//now off to trace some rays
						//std::cout << "so far so good" << std::endl;
						if (intersect.y <= face.pointLarge.y && intersect.y >= face.pointSmall.y)
						{
							//std::cout << "Even better" << std::endl;
							if (intersect.x <= face.pointLarge.x && intersect.x >= face.pointSmall.x)
							{
								//std::cout << "Hey, it hit, nice";
								brightness += 50;
							}
						}
						//Thats it, its that easy
					}
			}
		}
		if (brightness > 255)
				brightness = 255;

	}
	
};

void worldConstruction(std::vector<CubeWorldObject>& blocks)
{
	CubeWorldObject block00;
	block00.blockID = 0;
	block00.pos.x = 4;
	block00.pos.y = 1;
	block00.pos.z = -2;
	block00.blockSetup();

	CubeWorldObject block01;
	block01.blockID = 0;
	block01.pos.x = 5;
	block01.pos.y = 1;
	block01.pos.z = -2;
	block01.blockSetup();

	CubeWorldObject block02;
	block02.blockID = 0;
	block02.pos.x = 6;
	block02.pos.y = 1;
	block02.pos.z = -2;
	block02.blockSetup();

	CubeWorldObject block03;
	block03.blockID = 0;
	block03.pos.x = 4;
	block03.pos.y = 0;
	block03.pos.z = -2;
	block03.blockSetup();

	CubeWorldObject block04;
	block04.blockID = 0;
	block04.pos.x = 6;
	block04.pos.y = 0;
	block04.pos.z = -2;
	block04.blockSetup();

	CubeWorldObject block05;
	block05.blockID = 0;
	block05.pos.x = 4;
	block05.pos.y = -1;
	block05.pos.z = -2;
	block05.blockSetup();

	CubeWorldObject block06;
	block06.blockID = 0;
	block06.pos.x = 5;
	block06.pos.y = -1;
	block06.pos.z = -2;
	block06.blockSetup();

	CubeWorldObject block07;
	block07.blockID = 0;
	block07.pos.x = 6;
	block07.pos.y = -1;
	block07.pos.z = -2;
	block07.blockSetup();

	CubeWorldObject block08;
	block08.blockID = 0;
	block08.pos.x = 5;
	block08.pos.y = 0;
	block08.pos.z = -1;
	block08.blockSetup();



	blocks.push_back(block00);
	blocks.push_back(block01);
	blocks.push_back(block02);
	blocks.push_back(block03);
	blocks.push_back(block04);
	blocks.push_back(block05);
	blocks.push_back(block06);
	blocks.push_back(block07);
	blocks.push_back(block08);
}

//support functions for vector multiplication etc.

float vec3multiply(vec3 vec_0, vec3 vec_1)
{
	return float(vec_0.x * vec_1.x + vec_0.y * vec_1.y + vec_0.z * vec_1.z);
}
float vec3Angle(vec3 vec_0, vec3 vec_1)
{
	return acos(vec3multiply(vec_0, vec_1)/(vec_0.magnitude()*vec_1.magnitude()));
}
vec3 vec3average(vec3 vec_0, vec3 vec_1)
{
	vec3 average;
	average.x = (vec_0.x + vec_1.x) / 2;
	average.y = (vec_0.y + vec_1.y) / 2;
	average.z = (vec_0.z + vec_1.z) / 2;
	return average;
}
vec3 vec3substract(vec3 vec_0, vec3 vec_1)
{
	vec3 substracted;
	substracted.x = vec_0.x - vec_1.x;
	substracted.y = vec_0.y - vec_1.y;
	substracted.z = vec_0.z - vec_1.z;
	return substracted;
}
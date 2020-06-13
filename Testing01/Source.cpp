#include <iostream>
#include <vector>
#include "Rtlib.h"
#include "png++/png.hpp"
#include <cmath>
#include <algorithm>
#include <chrono>

#include <thread>

#define PI 3.14159265358979323846

//constants here
float const RTI_fov = 106;
float const RTI_focal_length = (1 / tan(RTI_fov / 360 * PI)) * (RTI_res_width / 2);
vec3 lightDir;

std::vector<std::thread> threads;

using namespace std;

int main()
{
	lightDir.x = 1;
	lightDir.y = 1;
	lightDir.z = 1;

	std::vector<CubeWorldObject> blocks;

	worldConstruction(blocks);

	auto blocksInFrame = blocks;


	RTI ray;
	ray.vec.x = RTI_focal_length;
	ray.pos.x = 0;
	ray.pos.y = 0;
	ray.pos.z = 0;

	vec3 origin;
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;

	int forTest = 0;
		auto startCull = chrono::steady_clock::now();
		for (auto &block : blocksInFrame)
		{
			auto it = std::remove_if(block.faces.begin(), block.faces.end(), [&origin](rectangularFace face)
				{
					return face.cull(origin);
				});
			block.faces.erase(it, block.faces.end()); 
		}

		auto endCull = chrono::steady_clock::now();

		std::cout << "beginning raytracing process..." << std::endl;

		auto start = chrono::steady_clock::now();



		png::image<png::rgb_pixel> image(RTI_res_width, RTI_res_height);
		//this is utterly fucking retarded


		std::thread th00(&RTI::RayQuadIntersect, ray, std::ref(blocksInFrame), std::ref(image), 0);
		std::thread th01(&RTI::RayQuadIntersect, ray, std::ref(blocksInFrame), std::ref(image), 1);
		std::thread th02(&RTI::RayQuadIntersect, ray, std::ref(blocksInFrame), std::ref(image), 2);
		std::thread th03(&RTI::RayQuadIntersect, ray, std::ref(blocksInFrame), std::ref(image), 3);


		th00.join();
		th01.join();
		th02.join();
		th03.join();
		auto end = chrono::steady_clock::now();
		forTest += chrono::duration_cast<chrono::milliseconds>(end - start).count();
	std::cout << forTest << std::endl;
	std::cout << ray.rayIntersects << std::endl;
		//std::cout << "cullingtime: " << chrono::duration_cast<chrono::milliseconds>(endCull - startCull).count() << " milliseconds" << std::endl;
		//std::cout << "raytime: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " milliseconds" << std::endl;
		//std::cout << "total time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() + chrono::duration_cast<chrono::milliseconds>(endCull - startCull).count() << " milliseconds" << std::endl;


	image.write("rgb.png");
}
 
#ifndef ASSIGNMENT3_THREAD_H
#define ASSIGNMENT3_THREAD_H

#include "ValueProperties.h"
#include <vector>
#include <chrono>
#include "Vertex.h"
#include "Triangle.h"
#include "OperationRasterizer.h"
#include <thread>


namespace rasterPipeline {

	template<class Target_t, class Shader>
	class Parallelization {
	public:
		Parallelization(OperationRasterizer<Target_t, Shader>& operation_, std::vector<Triangle>& triangles_) : operation(operation_){
			
			unsigned int max_thread = std::thread::hardware_concurrency();

			/*when the system isn't able to determine the max number of threads*/
			if (max_thread == 0) {
				/*in this case each thread has more or less 2 triangles, 0.5 need in the case that there is only one triangle*/
				number_of_thread = static_cast<unsigned int> ((triangles_.size() / 2) + 0.5);
				std::cerr << warning_1 << std::endl;
			}
			/*when the number of threads isn't correct*/
			else if (N_THREAD <= 0 || N_THREAD > max_thread) {
				number_of_thread = (max_thread > triangles_.size()) ? triangles_.size() : max_thread;
				std::cerr << error_1 << std::endl;
			}
			
			else {
				/*when there are more threads than triangles*/
				if (N_THREAD > triangles_.size()) {
					number_of_thread = triangles_.size();
					std::cerr << warning_2 << std::endl;
				}
				else {
					number_of_thread = N_THREAD;
				}	
			}
			/*resize the vector with correct dimension*/
			vector_triangles.resize(number_of_thread);

			/*fairness distribution of worked for each thread*/
			unsigned int i = 0;
			for (auto triangle = triangles_.begin(); triangle != triangles_.end(); triangle++){
				vector_triangles[i % number_of_thread].push_back(*triangle);
				i++;
			}
		}


		/*do the rendering*/
		void rendering() {
			std::cout << "NUMBER OF THREAD : " << number_of_thread << std::endl;

			/*start timer*/
			auto start = std::chrono::high_resolution_clock::now();

			/*start all threads*/
			for (unsigned int i = 0; i < number_of_thread; i++) {
				vector_thread.push_back(std::thread (&Parallelization::threadWork, this, i));
			}
			/*wait all threads*/
			for (auto thread = vector_thread.begin(); thread != vector_thread.end(); thread++) {
				if(thread->joinable()){
					thread->join();
				}
			}
			/*print screen*/
			operation.print();

			/*finish timer*/
			auto stop = std::chrono::high_resolution_clock::now();
			/*find the total time*/
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			/*print the time*/
			std::cout << "TIME TO COMPLETE THE JOB: " << duration.count() << " microseconds" << std::endl;
		}

	private:
		/*it contains all operations*/
		OperationRasterizer<Target_t, Shader> operation;
		/*it contains all threads*/
		std::vector<std::thread> vector_thread; 
		/*it contains set of triangles for each thread*/ 
		std::vector<std::vector<Triangle>> vector_triangles; 
		/*number of threads*/
		unsigned int number_of_thread;
		/*mutex for printing*/
		std::mutex m_print;

		const std::string warning_1 = "WARNING: THE SYSTEM ISN'T ABLE TO DETERMINE THE MAX NUMBER OF THREADS, SO IT HAS CHOSEN THE NUMBER";
		const std::string warning_2 = "WARNING: THERE ARE MORE THREADS THAN TRIANGLES, SO THE SYSTEM WILL NOT USE ALL OF THEM";
		const std::string error_1 = "ERROR: INCORRECT NUMBER OF THREAD, SO THE SYSTEM HAS CHOSEN IT";

		/*work for each thread*/
		void threadWork(unsigned int pos) {

			/*start timer*/
			auto start = std::chrono::high_resolution_clock::now();

			operation.transformCoordinatesUsingViewMatrix(vector_triangles[pos]);

			operation.calculateNormalizedVertices(vector_triangles[pos]);

			operation.transformNormalizedCoordinatesInScreenCoordinates(vector_triangles[pos]);

			operation.orderTriangleVerticesClockwiseOrder(vector_triangles[pos]);

			operation.depthZubberOperation(vector_triangles[pos]);

			/*finish timer*/
			auto stop = std::chrono::high_resolution_clock::now();
			/*find the total time*/
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			m_print.lock();
			/*print the time*/
			std::cout << "TIME FOR THREAD " << pos + 1 << " : " << duration.count() << " microseconds" << std::endl;
			m_print.unlock();
		}
	};
};

#endif
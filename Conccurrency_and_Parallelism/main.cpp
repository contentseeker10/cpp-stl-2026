/*
 * Volodymyr "contentseeker10" Stepanov
 * August 2026
 * C++ STL Cookbook Second Edition
 * Concurrency and Parallelism
*/

#include <print>

#include <chrono>
#include <thread>

using std::print, std::println;

using std::chrono::steady_clock;
using std::chrono::duration;

using namespace std::this_thread;
using namespace std::chrono_literals;

//{
//	println("\n---  ---\n");
//	
//	// .
//
//	println();
//}

int main() {
	{
		println("\n--- Sleep a process for a specific amount of time ---\n");
		
		auto t1 = steady_clock::now();
		println("sleep for 1.3 seconds");
		sleep_for(1s + 300ms);

		println("sleep for 2 seconds");
		sleep_until(steady_clock::now() + 2s);

		duration<double> dur1 = steady_clock::now() - t1;
		println("total duration: {:.5}s", dur1.count());
	
		println();
	}

	{
		println("\n---  ---\n");
		
		// .
	
		println();
	}

}
#pragma once  
#include <chrono>  
#include <thread>  

namespace TAGE {  
class Timer {  
public:  
	Timer();  
	void Reset();  
	float Elapsed() const;  
	float ElapsedMillis() const;  
	bool HasElapsed(float seconds) const {  
		return Elapsed() >= seconds;  
	}  
	static void Delay(float seconds) {  
		std::this_thread::sleep_for(std::chrono::duration<float>(seconds));  
	}  

private:  
	std::chrono::time_point<std::chrono::high_resolution_clock> _Start;  
};  
}
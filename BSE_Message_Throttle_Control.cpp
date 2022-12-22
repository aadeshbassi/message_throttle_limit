//============================================================================
// Name        : BSE_Message_Throttle_Control.cpp
// Author      : BSE_Infra_Team
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <ctime>
using namespace std;

//Timespec difference
void  timespec_diff(long* result, struct timespec* x, struct timespec* y)
{
	long current_time_nsec = x->tv_nsec + (x->tv_sec * 1000000000);
	long prev_time_nsec = y->tv_nsec + (y->tv_sec * 1000000000);
	*result = (current_time_nsec - prev_time_nsec)/1000000;      //result in milli_seconds
}
//For F2F
class BSE_OLD_INFRA_MESSAGE_THROTTLE {
private:
	int throttle_time_interval;
	int throttle_no_messages;
	int counter;
	timespec* circular_array;

public:
	BSE_OLD_INFRA_MESSAGE_THROTTLE(int, int);
	~BSE_OLD_INFRA_MESSAGE_THROTTLE();
	void message_throttle_control(timespec current_time);
};

BSE_OLD_INFRA_MESSAGE_THROTTLE::BSE_OLD_INFRA_MESSAGE_THROTTLE( // @suppress("Class members should be properly initialized")
	int throttle_time_interval, int throttle_no_messages) {
	this->throttle_time_interval = throttle_time_interval;
	this->throttle_no_messages = throttle_no_messages;
	this->counter = 0;
	circular_array = new timespec[throttle_no_messages];
	for (int i = 0; i < throttle_no_messages; i++) circular_array[i] = {};
	//TODO: initialize member fields(if any)
}

void BSE_OLD_INFRA_MESSAGE_THROTTLE::message_throttle_control(
	timespec current_time) {
	counter = counter + 1;
	if (counter >= throttle_no_messages) {
		circular_array[counter % throttle_no_messages] = current_time;
		long milli_seconds;
		timespec_diff(&milli_seconds, &circular_array[counter % throttle_no_messages], &circular_array[(counter - throttle_no_messages + 1) % throttle_no_messages]);
		//cout << "time difference :" << milli_seconds << endl;
		if (milli_seconds < throttle_time_interval)
		{
			cout << "Sleep for : " << abs(throttle_time_interval - milli_seconds) << "milli seconds in OLD INFRA" << endl; //sleep(throttle_limit-change-->absolute change)
			//usleep( abs(throttle_time_interval - milli_seconds)*1000);
			cout << "Send Message !!! OLD INFRA" << endl;
		}
		else 
		{
			cout << "Send Message !!! OLD INFRA" << endl;
		}
	}
	else {
		circular_array[counter % throttle_no_messages] = current_time;
		cout << "Send Message !!! OLD INFRA" << endl;
	}
		counter = counter + 1;
	//TODO: complete code (AIM: apply old BSE message throttle control i.e. sleep till required time unit not passed)
	//TODO: print appropriate message to display BSE old infra message throttle control
}

BSE_OLD_INFRA_MESSAGE_THROTTLE::~BSE_OLD_INFRA_MESSAGE_THROTTLE() {
	delete [] circular_array;
	//TODO: clean up member fields(if any)
}

class BSE_NEW_INFRA_MESSAGE_THROTTLE {
private:
	int throttle_time_interval;
	int throttle_no_messages;
	int counter;
	timespec* circular_array;

public:
	BSE_NEW_INFRA_MESSAGE_THROTTLE(int, int);
	~BSE_NEW_INFRA_MESSAGE_THROTTLE();
	void message_throttle_control(timespec current_time);
};

BSE_NEW_INFRA_MESSAGE_THROTTLE::BSE_NEW_INFRA_MESSAGE_THROTTLE( // @suppress("Unused function declaration") // @suppress("Class members should be properly initialized")
	int throttle_time_interval, int throttle_no_messages) {
	this->throttle_time_interval = throttle_time_interval;
	this->throttle_no_messages = throttle_no_messages;
	this->counter = 0;
	circular_array = new timespec[throttle_no_messages];
	for (int i = 0; i < throttle_no_messages; i++) circular_array[i] = {};
	//TODO: initialize member fields(if any)
}

void BSE_NEW_INFRA_MESSAGE_THROTTLE::message_throttle_control(
	timespec current_time) {
	counter = counter + 1;
	if (counter >= throttle_no_messages) {
		circular_array[counter % throttle_no_messages] = current_time;
		long milli_seconds
		timespec_diff(&milli_seconds, &circular_array[counter % throttle_no_messages], &circular_array[(counter - throttle_no_messages + 1) % throttle_no_messages]);
		//cout << "time difference :" << milli_seconds << endl;
		if (milli_seconds < throttle_time_interval)
		{
			cout << "Message throttle Rejection" << endl;
			counter = counter - 1; 
		}
		else
		{
			cout << "Send Message !!!!! NEW INFRA" << endl;	//send the message
		}
	}
	else {
		circular_array[counter % throttle_no_messages] = current_time;
		cout << "Send Message !!! NEW INFRA" << endl;
	}

	//TODO: complete code (AIM: apply NEW BSE message throttle control)
	//TODO: print appropriate message to display BSE new infra message throttle control
}

BSE_NEW_INFRA_MESSAGE_THROTTLE::~BSE_NEW_INFRA_MESSAGE_THROTTLE() {
	delete[] circular_array;
	//TODO: clean up member fields(if any)
}

int main(int argc, char** argv) {
	if (argc != 3) {
		cout << " Usage: program_name throttle_time_interval_in_ms throttle_no_messages";
		return 1;
	}
	int throttle_time_interval = atoi(argv[1]);
	int throttle_no_messages = atoi(argv[2]);

	cout << " throttle_time_interval[" << throttle_time_interval << "] throttle_no_messages[" << throttle_no_messages << "]" << endl;

	BSE_OLD_INFRA_MESSAGE_THROTTLE old_infra_message_throttle(throttle_time_interval, throttle_no_messages);
	BSE_NEW_INFRA_MESSAGE_THROTTLE new_infra_message_throttle(throttle_time_interval, throttle_no_messages);

	srand(time(NULL));
	while (true) {
		//usleep(time(NULL) % (rand()));//random sleep to simulate random order placement interval
		timespec request_generation_time;
		clock_gettime(CLOCK_REALTIME, &request_generation_time);

		//Old infra
		old_infra_message_throttle.message_throttle_control(request_generation_time);

		//New infra
		new_infra_message_throttle.message_throttle_control(request_generation_time);
		usleep(10000);
	}

	return 0;
}

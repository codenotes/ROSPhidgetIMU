#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <phidget21.h>

#include "phidgets_api/imu_filter_nodelet.h"

using namespace std;
using namespace ros;

// --------------------Event Functions-------------------------------------- -
int LocalErrorCatcher(int errorCode);

int CCONV AttachHandler(CPhidgetHandle device, void *userptr) {

	int serialNumber;
	const char *name;

	LocalErrorCatcher(
		CPhidget_getDeviceName(device, &name));
	LocalErrorCatcher(
		CPhidget_getSerialNumber(device, &serialNumber));

	printf("Hello Device %s, Serial Number: %d\n", name, serialNumber);

	return 0;
}

int CCONV DetachHandler(CPhidgetHandle device, void *userptr) {

	int serialNumber;
	const char *name;

	LocalErrorCatcher(
		CPhidget_getDeviceName(device, &name));
	LocalErrorCatcher(
		CPhidget_getSerialNumber(device, &serialNumber));

	printf("Goodbye Device %s, Serial Number: %d\n", name, serialNumber);

	return 0;
}

// When using an error handler with the manager, it takes a
// CPhidgetManagerHandle, when using an individual object, the
// object serves as its own handle.
int CCONV LibraryErrorHandler(CPhidgetManagerHandle device, void *usrptr,
	int errorCode, const char *errorDescription) {
	printf("Error Event: %d - %s\n", errorCode, errorDescription);
	return 0;
}

// This error handler can handle any CPhidget function that returns an int
int LocalErrorCatcher(int errorCode) {

	const char *errorDescription;

	// If the error code is 0, everything is okay
	if (errorCode != 0) {

		// Otherwise, you can print specific messages or perform actions by error value.
		switch (errorCode) {
		default:
			printf("Error: An error occurred with code %d.\n", errorCode);

			LocalErrorCatcher(
				CPhidget_getErrorDescription(errorCode, &errorDescription));
			printf("The description for this error is: %s\n", errorDescription);
			break;
		}
	}
	return 0;
}

int  phid_test()
{
	int result;
	const char *err;

	CPhidgetManagerHandle device = 0;

	LocalErrorCatcher(
		CPhidgetManager_create(&device));

	LocalErrorCatcher(
		CPhidgetManager_set_OnAttach_Handler((CPhidgetManagerHandle)device,
		AttachHandler, NULL));

	LocalErrorCatcher(
		CPhidgetManager_set_OnDetach_Handler((CPhidgetManagerHandle)device,
		DetachHandler, NULL));

	LocalErrorCatcher(
		CPhidgetManager_set_OnError_Handler((CPhidgetManagerHandle)device,
		LibraryErrorHandler, NULL));
	printf("Opening...\n");
	// Most opening and closing would be via a cast to
	// (CPhidgetHandle), however, this manager has its
	// own handle struct to cast to.
	LocalErrorCatcher(
		CPhidgetManager_open((CPhidgetManagerHandle)device));

	printf("Phidget Simple Playground (plug and unplug devices)\n");
	printf("Press Enter to end anytime...\n");
	getchar();

	printf("Closing...\n");
	LocalErrorCatcher(
		CPhidgetManager_close((CPhidgetManagerHandle)device));
	LocalErrorCatcher(
		CPhidgetManager_delete((CPhidgetManagerHandle)device));

	return 0;



}


//#include "phidgets_imu/phidgets_imu_nodelet.h"
#include "phidgets_api/phidgets_imu_nodelet.h"

typedef phidgets::PhidgetsImuNodelet PhidgetsImuNodelet;

PLUGINLIB_DECLARE_CLASS(phidgets_imu, PhidgetsImuNodelet, PhidgetsImuNodelet, nodelet::Nodelet);

void PhidgetsImuNodelet::onInit()
{
	NODELET_INFO("Initializing Phidgets IMU Nodelet");

	// TODO: Do we want the single threaded or multithreaded NH?
	ros::NodeHandle nh = getMTNodeHandle();
	ros::NodeHandle nh_private = getMTPrivateNodeHandle();

	imu_ = new ImuRosI(nh, nh_private);
}


#if 0
//imu node

#include "phidgets_imu/imu_ros_i.h"

int main(int argc, char **argv)
{
	ros::init(argc, argv, "PhidgetsImu");
	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");
	phidgets::ImuRosI imu(nh, nh_private);
	ros::spin();
	return 0;
}


<!-- Phidgets IMU launch file -->

<launch>

#### Nodelet manager ######################################################

<node pkg = "nodelet" type = "nodelet" name = "imu_manager"
args = "manager" output = "screen" / >

#### IMU Driver ###########################################################

<node pkg = "nodelet" type = "nodelet" name = "PhidgetsImuNodelet"
args = "load phidgets_imu/PhidgetsImuNodelet imu_manager"
output = "screen">

# supported data rates : 4 8 16 24 32 40 ... 1000 (in ms)
<param name = "period" value = "4" / >

< / node>

#### IMU Orientation Filter ###############################################

<node pkg = "nodelet" type = "nodelet" name = "ImuFilterNodelet"
args = "load imu_filter_madgwick/ImuFilterNodelet imu_manager"
output = "screen">

<param name = "use_mag" value = "true" / >

< / node>

< / launch>



#endif








int main(int argc, char *argv[])
{
	
	//return phid_test();
	

	ros::init(argc, argv, "PhidgetsImu");
	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");
	phidgets::ImuRosI imu(nh, nh_private);

	ros::NodeHandle nh2;
	ros::NodeHandle nh_private2("~");
	ImuFilter imu_filter(nh2, nh_private2);



	ros::spin();
	return 0;


	char sz[] = "Hello, World!";	//Hover mouse over "sz" while debugging to see its contents
//	cout << sz << endl;	//<================= Put a breakpoint here
	ros::init(argc, argv, "talker");
	
	ros::NodeHandle n;


	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

	ros::Rate loop_rate(10);


	int count = 0;
	while (ros::ok())
	{
		/**
		* This is a message object. You stuff it with data, and then publish it.
		*/
		std_msgs::String msg;

		std::stringstream ss;
		ss << "hello world " << count;
		msg.data = ss.str();

		ROS_INFO("%s", msg.data.c_str());


		chatter_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
		++count;
	}


}
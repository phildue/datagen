#include "Controller.h"
#include <iostream>
#include <Eigen/Dense>
#include <chrono>
#include "Kinematics.h"
#include "DroneModel.h"

using namespace Eigen;

void Controller::setReference(float x, float y, float z, float heading)
{
	controllerMode = POSITION_MODE;
	x_r = x; y_r = y; z_r = z; psi_r = heading;
//	std::cout << "[positon autopilot]" << psi_r << std::endl;
}

void Controller::setVelocity(float v_x, float v_y, float v_z, float psi)
{
	controllerMode = VELOCITY_MODE;
	v_x_r = v_x; v_y_r = v_y; v_z_r = v_z; psi_r = psi;
//	std::cout << "[velocity autopilot]" << psi_r << std::endl;
}

void Controller::setAttitude(float phi_r, float theta_r, float psi_r, float z_r)
{
	controllerMode = ATTITUDE_MODE;
	this->phi_r = phi_r;
	this->theta_r = theta_r;
	this->psi_r = psi_r;
	this->z_r = z_r;
}

mav::Kinematics Controller::controllerRun(mav::Kinematics kin)
{
//	std::cout << "x_r = " << x_r << std::endl;
//	std::cout << "y_r = " << y_r << std::endl;
//	std::cout << "psi_r = " << psi_r << std::endl;
	//std::cout << "Position: " << x << y << z << std::endl;
	/*std::cout << "Orientation: " << orientation[0] << orientation[1]  << orientation[2] << std::endl;
	std::cout << "Velocity: " << velocity[0] << velocity[1] << velocity[2] << std::endl;*/

	float deltaT_s = (timestep_ms-lastTimeStamp_ms)/1000.0f;
	Vector3f deltaX = Vector3f(x_r, y_r, z_r) - Vector3f(kin.x, kin.y, kin.z);

	if (controllerMode == POSITION_MODE)
	{
		v_x_r = deltaX(0)*K_P_VX;
		v_y_r = deltaX(1)*K_P_VY;
		v_z_r = deltaX(2)*K_P_VZ;
	}

	if (controllerMode == ATTITUDE_MODE)
	{
		v_z_r = deltaX(2)*K_P_VZ;
	}
	
//	std::cout << "Vzr: "<<v_z_r << std::endl;
	float currentHeading = kin.psi;
	
	Vector3f deltaV = Vector3f(v_x_r, v_y_r, v_z_r) - Vector3f(kin.vX, kin.vY, kin.vZ);
	Matrix<float, 3, 3> rotationMatrix;
	rotationMatrix << cos(currentHeading), sin(currentHeading), 0, -sin(currentHeading), cos(currentHeading),0, 0, 0, 1;
	Vector3f deltaVLocal = rotationMatrix * deltaV;
	//std::cout << "deltaV: " << deltaV << std::endl;
	//std::cout << "deltaVLocal: " << deltaVLocal << std::endl;

	float errorPhi = deltaVLocal(1);
	float errorTheta = deltaVLocal(0);
	float errorThrust = deltaVLocal(2);

//	std::cout << "errorTheta " << errorTheta << std::endl;

	sum_error_phi += errorPhi * deltaT_s;
	float phi_cmd = errorPhi * K_P_PHI + (errorPhi - previous_error_phi) / deltaT_s * K_D_PHI + sum_error_phi * K_I_PHI;
	previous_error_phi = errorPhi;

	sum_error_theta += errorTheta * deltaT_s;
	float theta_cmd = errorTheta * K_P_THETA + (errorTheta - previous_error_theta) / deltaT_s * K_D_THETA + sum_error_theta * K_I_THETA;
	//std::cout << "current x: " <<  kin.x << std::endl;
	//std::cout << "current vy: " << kin.vY << std::endl;
	//std::cout << "theta_cmd: " << theta_cmd << std::endl;
    float psi_cmd = 0.0;
	previous_error_theta = errorTheta;

	sum_error_thrust += sum_error_thrust * deltaT_s;
	float thrust_cmd = -GRAVITY_FACTOR + errorThrust * K_P_THRUST + K_I_THRUST * sum_error_thrust;
	/*std::cout << "control thrust errir: " << errorThrust << std::endl;
	std::cout << "control thrust P tern: " << errorThrust * K_P_THRUST << std::endl;
	std::cout << "control thrust I tern: " << K_I_THRUST * sum_error_thrust << std::endl;*/
	//std::cout << "phi_cmd: " << phi_cmd << std::endl;

	if (controllerMode == ATTITUDE_MODE)
	{
		phi_cmd = phi_r;
		theta_cmd = theta_r;
		psi_cmd = psi_r;
	}else if(controllerMode == POSITION_MODE){
	    psi_cmd = psi_r;
	} else if(controllerMode == VELOCITY_MODE){
	    psi_cmd = psi_r;
	}

	if (thrust_cmd > 0)
		thrust_cmd = 0;
	else if (thrust_cmd < -20)
		thrust_cmd = -20;

	if (theta_cmd > 40.0 / 180 * 3.14)
		theta_cmd = 40.0 / 180 * 3.14;
	else if (theta_cmd < -40.0 / 180 * 3.14)
		theta_cmd = -40.0 / 180 * 3.14;


    lastTimeStamp_ms = timestep_ms;

	// todo: psi is 0
    mav::Kinematics cmd;
    cmd.phi = phi_cmd;
    cmd.theta = theta_cmd;
    cmd.psi = psi_cmd;
    cmd.accZ = thrust_cmd;
	//std::cout << "[autopilot] attitude command psi " << att_cmd.yaw << std::endl;
	return cmd;
	 

}

Controller::Controller()
{
    this->phi_r = 0;
    this->theta_r = 0;
    this->psi_r = 0;
    this->z_r = 0;
	controllerMode = POSITION_MODE; // !!!!!!!!!!!!!!!!!!!!!!!!!  for debug
	previous_error_theta = 0;
	previous_error_phi = 0;
	sum_error_thrust = 0;
	sum_error_theta = 0;
	sum_error_phi = 0;
	lastTimeStamp_ms =0;
	
	
}




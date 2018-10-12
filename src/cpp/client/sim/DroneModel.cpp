#include <chrono>
#include "DroneModel.h"
#include <iostream>


DroneModel::DroneModel()
{
	x = 0; y = 0; z = 0; v_x = 0; v_y = 0; v_z = 0; phi = 0; theta = 0; psi = 0; thrust = -GRAVITY_FACTOR; phi_r = 0; theta_r = 0; psi_r = 0;
	dPhi = 0; dTheta = 0; dPsi = 0; p = 0; q= 0; r = 0;lastStepThrust = -GRAVITY_FACTOR;
	last_timestep_ms = 0;
}

DroneModel::~DroneModel() {}


void DroneModel:: setCommand(float phi_r,float theta_r,float psi_r,float thrust)
{
    this->phi_r = phi_r;
    this->theta_r = theta_r;
    this->psi_r = psi_r;
    this->thrust = thrust;
}

Matrix<float,12,1> DroneModel:: getStateVector()
{
    Matrix<float,12,1> states;
    states<<x,y,z,v_x,v_y,v_z,phi,dPhi,theta,dTheta,psi,dPsi;
    return states;
};

void DroneModel:: setStateVector(Matrix<float,12,1> state)
{
    x = state(0); y = state(1); z = state(2); v_x = state(3); v_y = state(4); v_z = state(5);
    phi = state(6); dPhi = state(7); theta = state(8); dTheta = state(9); psi = state(10); dPsi = state(11);
}


void DroneModel:: propagateDroneModel()
{
	

    Vector3f dPosition(v_x,v_y,v_z);
    Vector3f dVelocity;
    Vector3f gravityVectorEarth(0,0,GRAVITY_FACTOR);
    Vector3f minusGravityVectorEarth(0,0,-GRAVITY_FACTOR);
    Vector3f thrustVectorBody(0,0,lastStepThrust);
    Vector3f velocityVectorEarth(v_x,v_y,v_z);
    Matrix3f R_E_B,R_B_E,dragMatrix;
    R_E_B(0,0) = cos(theta)*cos(psi) ;
    R_E_B(0,1) = cos(theta)*sin(psi) ;
    R_E_B(0,2) = -sin(theta);
    R_E_B(1,0) = sin(phi)*sin(theta)*cos(psi)-cos(phi)*sin(psi);
    R_E_B(1,1) = sin(phi)*sin(theta)*sin(psi)+cos(phi)*cos(psi);
    R_E_B(1,2) = sin(phi)*cos(theta);
    R_E_B(2,0) = cos(phi)*sin(theta)*cos(psi)+sin(phi)*sin(psi);
    R_E_B(2,1) = cos(phi)*sin(theta)*sin(psi)-sin(phi)*cos(psi);
    R_E_B(2,2) = cos(phi)*cos(theta);
    R_B_E = R_E_B.transpose();
    dragMatrix<<K_X,0,0,0,K_Y,0,0,0,K_Z;

    dVelocity = gravityVectorEarth + R_B_E*thrustVectorBody + R_B_E*dragMatrix*R_E_B*velocityVectorEarth;
    specificForce = R_E_B*(R_B_E*thrustVectorBody+R_B_E*dragMatrix*R_E_B*velocityVectorEarth);
    //specificForce = R_E_B*minusGravityVectorEarth;
    Vector3f angularVelocity, dAttitude(dPhi,dTheta,dPsi);
    Matrix3f R;
    R << 1,0,-sin(theta),0,cos(phi),cos(theta)*sin(phi),0,-sin(phi),cos(theta)*cos(phi);
    angularVelocity = R*dAttitude;
    p = angularVelocity(0);
    q = angularVelocity(1);
    r = angularVelocity(2);

    // dPhiVector = [\dot{phi};\dot{dPhi}]
    Vector2f dPhiVector,dThetaVector,dPsiVector;
    Matrix<float,2,2> A;
    A<<0,1,-OMEGA*OMEGA,-2*XI*OMEGA;
    Vector2f B(0,OMEGA*OMEGA);
    Vector2f currentPhiVector(phi,dPhi),currentThetaVector(theta,dTheta),currentPsiVector(psi,dPsi);
    dPhiVector = A*currentPhiVector+B*phi_r;
    dThetaVector = A*currentThetaVector+B*theta_r;
    dPsiVector = A*currentPsiVector+B*psi_r;

    Matrix<float,12,1> dx;
    dx<< dPosition,dVelocity,dPhiVector,dThetaVector,dPsiVector;


    float deltaT =  (timestep_ms - last_timestep_ms)/1000.0f;


    Matrix<float,12,1> state = getStateVector()+dx*deltaT;
    last_timestep_ms = timestep_ms;
    setStateVector(state);

    lastStepThrust = thrust;


}


#include "AirSimInterface.h"

AirSimInterface::
AirSimInterface()
{
    client = new msr::airlib::MultirotorRpcLibClient;
    this->session = new Session(client);
}

AirSimInterface::
~AirSimInterface()
{
	this->session->disconnect();
}

void
AirSimInterface::
setDronePose(float north, float east, float down, float roll, float pitch, float yaw)
{
	Eigen::Quaternionf q = euler2Quat(roll,pitch,yaw);

	client->simSetPose(msr::airlib::Pose(Eigen::Vector3f({ north,east,down }), q),true);
}


void
AirSimInterface::
connect()
{
	this->session->connect();
}

Image
AirSimInterface::
getImage()
{
    vector<ImageCaptureBase::ImageRequest> request = { ImageCaptureBase::ImageRequest(1, ImageCaptureBase::ImageType::Scene, false, false) };

    const vector<ImageCaptureBase::ImageResponse>& response = client->simGetImages(request);
//TODO check if there are multiple responses
    auto r = response[0];
    return Image(r.image_data_uint8,r.width,r.height);
}

Eigen::Quaternionf
AirSimInterface::
euler2Quat(float roll, float pitch, float yaw)
{
    Eigen::Quaternionf q;
    // Abbreviations for the various angular functions
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);

    q.w() = cy * cr * cp + sy * sr * sp;
    q.x() = cy * sr * cp - sy * cr * sp;
    q.y() = cy * cr * sp + sy * sr * cp;
    q.z() = sy * cr * cp - cy * sr * sp;
    return q;
}

Pose
AirSimInterface::
getCameraPose()
{
    return client->getCameraInfo(0).pose;
}

std::map<std::string, msr::airlib::Pose>
AirSimInterface::
getObjPoses(const std::string &obj_name, int max_gates)
{
    std::map<std::string, msr::airlib::Pose> gates;
    for(int i = 0; i < max_gates; i++){
        std::ostringstream name;
        name << obj_name << i;
        std::cout << "Looking for object: " << name.str() << std::endl;
        auto pose = client->simGetObjectPose(name.str());//returns nan if no object is found
        if(!pose.position.hasNaN() &&
           !pose.orientation.toRotationMatrix().hasNaN()){
            std::cout << "Found: " << name.str() << std::endl;
            gates.insert(std::pair<std::string, msr::airlib::Pose>(name.str(),pose));
        }else{
            std::cout << "Not Found: " << name.str() << std::endl;
        }

    }
    return gates;
}










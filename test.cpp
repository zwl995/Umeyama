#include <iostream>
#include <string>
#include <fstream>
#include "AlignRobust_data.h"
using namespace std;

struct POSE {
	string img_name;
	double x, y, z;
};

vector<POSE> ReadTXT(const string& input);

int main() {
	std::string service_pose_path = "../data/reloc_wenguang.txt";
	std::string cc_pose_path = "../data/software_park_five_4group.txt";
	std::string Tcc2arkit_path = "../data/Tcc2arkit.txt";
	auto service_poses = ReadTXT(service_pose_path);
	auto cc_poses = ReadTXT(cc_pose_path);

	// 定义cc和服务端重定位对应相同图片的pose容器，直接用于sim3的数据
	std::vector<Eigen::Vector3d> cc_pose_res, service_pose_res;

	for (auto cc_pose : cc_poses) {
		std::string cc_img_name = cc_pose.img_name;
		for (auto service_pose : service_poses) {
			std::string service_img_name = service_pose.img_name;
			if (cc_img_name == service_img_name) {
				Eigen::Vector3d tem_cc_pose(cc_pose.x, cc_pose.y, cc_pose.z);
				Eigen::Vector3d tem_service_pose(service_pose.x, service_pose.y, service_pose.z);
				cc_pose_res.push_back(tem_cc_pose);
				service_pose_res.push_back(tem_service_pose);
			}
		}
	}
	
	std::cout << cc_pose_res.size() << std::endl;
	std::cout << service_pose_res.size() << std::endl;
	if ((cc_pose_res.size() == service_pose_res.size()) && cc_pose_res.size() > 3) {
		auto Tcc2arkit = Umeyama(service_pose_res, cc_pose_res);
		std::cout << Tcc2arkit << std::endl;
		std::ofstream fout(Tcc2arkit_path);
		fout << Tcc2arkit << std::endl;
		fout.flush();
	}
	else {
		std::cout << "The number of retrieved images is different or less than 3！" << std::endl;
	}
}

vector<POSE> ReadTXT(const string& input) {
	vector<POSE> pose;
	ifstream infile(input);
	string line;
	while (getline(infile, line)) {
		stringstream ss(line);
		string s;
		vector<string> tem;
		while (ss >> s) {
			tem.push_back(s);
		}
		POSE tem_pose;
		tem_pose.img_name = tem[0];
		tem_pose.x = atof(tem[1].c_str());
		tem_pose.y = atof(tem[2].c_str());
		tem_pose.z = atof(tem[3].c_str());
		pose.push_back(tem_pose);
	}
	return pose;
}
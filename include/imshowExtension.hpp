#pragma once

#include "common.hpp"

namespace cp
{
	//normalize image and then cast to 8U and imshow. NORM_INF(32) scale 0-max
	CP_EXPORT void imshowNormalize(std::string wname, cv::InputArray src, const int norm_type = cv::NORM_MINMAX);

	//scaling ax+b, cast to 8U, and then imshow
	CP_EXPORT void imshowScale(std::string name, cv::InputArray src, const double alpha = 1.0, const double beta = 0.0);

	//resize image, cast 8U (optional), and then imshow 
	CP_EXPORT void imshowResize(std::string name, cv::InputArray src, const cv::Size dsize, const double fx = 0.0, const double fy = 0.0, const int interpolation = cv::INTER_NEAREST, bool isCast8U = true);

	//3 times count down
	CP_EXPORT void imshowCountDown(std::string wname, cv::InputArray src, const int waitTime = 1000, cv::Scalar color = cv::Scalar::all(0), const int pointSize = 128, std::string fontName = "Consolas");

	class CP_EXPORT StackImage
	{
		std::vector<cv::Mat> stack;
		std::string wname;
		int num_stack = 0;
		int stack_max = 0;
	public:
		StackImage(std::string window_name="image stack");
		void setWindowName(std::string window_name);
		void overwrite(cv::Mat& src);
		void push(cv::Mat& src);
		void show();
		void show(cv::Mat& src);
	};
}
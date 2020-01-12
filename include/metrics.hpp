#pragma once

#include "common.hpp"

namespace cp
{

	CP_EXPORT double MSE(cv::InputArray src1, cv::InputArray src2);
	CP_EXPORT double MSE(cv::InputArray src1, cv::InputArray src2, cv::InputArray mask);
	CP_EXPORT double calcBadPixel(const cv::Mat& src, const cv::Mat& ref, int threshold);
	CP_EXPORT double SSIM(cv::Mat& src, cv::Mat& ref, double sigma = 1.5);
	CP_EXPORT double calcTV(cv::Mat& src);
	CP_EXPORT double calcEntropy(cv::InputArray src, cv::InputArray mask = cv::noArray());

	enum PSNR_CHANNEL
	{
		PSNR_ALL,
		PSNR_Y,
		PSNR_B,
		PSNR_G,
		PSNR_R,
	};
	enum PSNR_PRECISION
	{
		PSNR_8U,
		PSNR_32F,
		PSNR_64F,
		PSNR_KAHAN_64F
	};
	class CP_EXPORT PSNRMetrics
	{
		cv::Mat source;
		cv::Mat reference;

		cv::Mat crops;
		cv::Mat cropr;
		cv::Mat temp;
		std::vector<cv::Mat> vtemp;

		void cvtImageForPSNR64F(const cv::Mat& src, cv::Mat& dest, const int cmethod);
		void cvtImageForPSNR32F(const cv::Mat& src, cv::Mat& dest, const int cmethod);
		void cvtImageForPSNR8U(const cv::Mat& src, cv::Mat& dest, const int cmethod);
	public:

		/*
		boundingBox: ignoring outside region. default is 0
		precision: computing precision, default PSNR_32F(1), other PSNR_8U(0),PSNR_64F(2), PSNR_KAHAN_64F(3)
		compare_method: default compute MSE all channele and then logged PSNR_ALL(0), PSNR_Y(1), PSNR_B(2), PSNR_G(3), PSNR_R(4),
		*/
		double getPSNR(cv::InputArray src, cv::InputArray ref, const int boundingBox = 0, const int precision = PSNR_32F, const int compare_method = PSNR_ALL);

		/*
		same function of getPSNR() for short cut
		boundingBox: ignoring outside region. default is 0
		precision: computing precision, default PSNR_32F(1), other PSNR_8U(0),PSNR_64F(2), PSNR_KAHAN_64F(3)
		compare_method: default compute MSE all channele and then logged PSNR_ALL(0), PSNR_Y(1), PSNR_B(2), PSNR_G(3), PSNR_R(4),
		*/
		double operator()(cv::InputArray src, cv::InputArray ref, const int boundingBox = 0, const int precision = PSNR_32F, const int compare_method = PSNR_ALL);

		//set reference image for acceleration
		void setReference(cv::InputArray src, const int boundingBox = 0, const int precision = PSNR_32F, const int compare_method = PSNR_ALL);


		//using preset reference image for acceleration
		double getPSNRPreset(cv::InputArray src, const int boundingBox = 0, const int precision = PSNR_32F, const int compare_method = PSNR_ALL);
	};

	/*
	wrapper function for class PSNRMetrics
	boundingBox: ignoring outside region. default is 0
	precision: computing precision, default PSNR_32F(1), other PSNR_8U(0),PSNR_64F(2), PSNR_KAHAN_64F(3)
	compare_method: default compute MSE all channele and then logged PSNR_ALL(0), PSNR_Y(1), PSNR_B(2), PSNR_G(3), PSNR_R(4),
	*/
	CP_EXPORT double getPSNR(cv::InputArray src, cv::InputArray ref, const int boundingBox = 0, const int precision = PSNR_32F, const int compare_method = PSNR_ALL);
}
#include "guidedFilter_Merge_Transpose_Inverse.h"

using namespace std;
using namespace cv;

void guidedFilter_Merge_Transpose_Inverse_nonVec::init()
{
	b.create(t_size, CV_32F);
	if (guide.channels() == 1)
	{
		a.create(t_size, CV_32F);

		temp.resize(4);
		for (int i = 0; i < temp.size(); i++) temp[i].create(t_size, CV_32F);
		temp_ab.resize(2);
		for (int i = 0; i < temp_ab.size(); i++) temp_ab[i].create(src.size(), CV_32F);
	}
	else if (guide.channels() == 3)
	{
		split(guide, vI);

		va.resize(3);
		for (int i = 0; i < va.size(); i++) va[i].create(t_size, CV_32F);
		temp.resize(13);
		for (int i = 0; i < temp.size(); i++) temp[i].create(t_size, CV_32F);
		temp_ab.resize(4);
		for (int i = 0; i < temp_ab.size(); i++) temp_ab[i].create(src.size(), CV_32F);
	}
}

void guidedFilter_Merge_Transpose_Inverse_nonVec::filter_Guide1(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec rsf_ip2ab(input, guide, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec csf_ip2ab(temp, a, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_nonVec csf_ab2q(a, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide1_Transpose_Inverse rsf_ab2q(temp_ab, guide, output, r, parallelType); rsf_ab2q.filter();
}

void guidedFilter_Merge_Transpose_Inverse_nonVec::filter_Guide3(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec rsf_ip2ab(input, vI, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec csf_ip2ab(temp, va, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_nonVec csf_ab2q(va, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide3_Transpose_Inverse rsf_ab2q(temp_ab, vI, output, r, parallelType); rsf_ab2q.filter();
}

void guidedFilter_Merge_Transpose_Inverse_SSE::filter_Guide1(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE rsf_ip2ab(input, guide, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE csf_ip2ab(temp, a, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_SSE csf_ab2q(a, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide1_Transpose_Inverse rsf_ab2q(temp_ab, guide, output, r, parallelType); rsf_ab2q.filter();
}

void guidedFilter_Merge_Transpose_Inverse_SSE::filter_Guide3(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE rsf_ip2ab(input, vI, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE csf_ip2ab(temp, va, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_SSE csf_ab2q(va, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide3_Transpose_Inverse rsf_ab2q(temp_ab, vI, output, r, parallelType); rsf_ab2q.filter();
}

void guidedFilter_Merge_Transpose_Inverse_AVX::filter_Guide1(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX rsf_ip2ab(input, guide, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX csf_ip2ab(temp, a, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_AVX csf_ab2q(a, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide1_Transpose_Inverse rsf_ab2q(temp_ab, guide, output, r, parallelType); rsf_ab2q.filter();
}

void guidedFilter_Merge_Transpose_Inverse_AVX::filter_Guide3(cv::Mat& input, cv::Mat& output)
{
	RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX rsf_ip2ab(input, vI, temp, r, parallelType); rsf_ip2ab.filter();
	ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX csf_ip2ab(temp, va, b, r, eps, parallelType); csf_ip2ab.filter();
	ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_AVX csf_ab2q(va, b, temp_ab, r, parallelType); csf_ab2q.filter();
	RowSumFilter_ab2q_Guide3_Transpose_Inverse rsf_ab2q(temp_ab, vI, output, r, parallelType); rsf_ab2q.filter();
}



/*
 * Guide1
 */
void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p += step;
		*v1_p = sum[1];
		v1_p += step;
		*v2_p = sum[2];
		v2_p += step;
		*v3_p = sum[3];
		v3_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p += step;
		*v1_p = sum[1];
		v1_p += step;
		*v2_p = sum[2];
		v2_p += step;
		*v3_p = sum[3];
		v3_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p += step;
		*v1_p = sum[1];
		v1_p += step;
		*v2_p = sum[2];
		v2_p += step;
		*v3_p = sum[3];
		v3_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			v0_p += step;
			*v1_p = sum[1];
			v1_p += step;
			*v2_p = sum[2];
			v2_p += step;
			*v3_p = sum[3];
			v3_p += step;
		}
	}
}



void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 4 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 4 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 4 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 4 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 4 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 4 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 4 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 4 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 4 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 4 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 4 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 4 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 3) == 3)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}



void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 8 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 8 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 8 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 8 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 8 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 8 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 8 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 8 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* I_p1 = I.ptr<float>(j);
		float* p_p1 = p.ptr<float>(j);
		float* I_p2 = I.ptr<float>(j) + 1;
		float* p_p2 = p.ptr<float>(j) + 1;

		float* v0_p = tempVec[0].ptr<float>(0) + 8 * j; // mean_I
		float* v1_p = tempVec[1].ptr<float>(0) + 8 * j; // mean_p
		float* v2_p = tempVec[2].ptr<float>(0) + 8 * j; // corr_I
		float* v3_p = tempVec[3].ptr<float>(0) + 8 * j; // corr_Ip

		float sum[4] = { 0.f };
		sum[0] += *I_p1 * (r + 1);
		sum[1] += *p_p1 * (r + 1);
		sum[2] += (*I_p1 * *I_p1) * (r + 1);
		sum[3] += (*I_p1 * *p_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2;
			sum[1] += *p_p2;
			sum[2] += *I_p2 * *I_p2;
			sum[3] += *I_p2 * *p_p2;
			I_p2++;
			p_p2++;
		}
		*v0_p = sum[0];
		v0_p++;
		*v1_p = sum[1];
		v1_p++;
		*v2_p = sum[2];
		v2_p++;
		*v3_p = sum[3];
		v3_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;
			I_p2++;
			p_p2++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum[0] += *I_p2 - *I_p1;
			sum[1] += *p_p2 - *p_p1;
			sum[2] += (*I_p2 * *I_p2) - (*I_p1 * *I_p1);
			sum[3] += (*I_p2 * *p_p2) - (*I_p1 * *p_p1);
			I_p1++;
			p_p1++;

			*v0_p = sum[0];
			*v1_p = sum[1];
			*v2_p = sum[2];
			*v3_p = sum[3];

			if ((i & 7) == 7)
			{
				v0_p += step;
				v1_p += step;
				v2_p += step;
				v3_p += step;
			}
			else
			{
				v0_p++;
				v1_p++;
				v2_p++;
				v3_p++;
			}
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum[4] = { 0.f };
		float tmp[4] = { 0.f };
		float var_I, cov_Ip;

		sum[0] = *v0_p1 * (r + 1);
		sum[1] = *v1_p1 * (r + 1);
		sum[2] = *v2_p1 * (r + 1);
		sum[3] = *v3_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2;
			v0_p2++;
			sum[1] += *v1_p2;
			v1_p2++;
			sum[2] += *v2_p2;
			v2_p2++;
			sum[3] += *v3_p2;
			v3_p2++;
		}
		tmp[0] = sum[0] * div;
		tmp[1] = sum[1] * div;
		tmp[2] = sum[2] * div;
		tmp[3] = sum[3] * div;
		var_I = tmp[2] - tmp[0] * tmp[0];
		cov_Ip = tmp[3] - tmp[0] * tmp[1];
		*a_p = cov_Ip / (var_I + eps);
		*b_p = tmp[1] - *a_p * tmp[0];
		a_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum[4] = { 0.f };
		float tmp[4] = { 0.f };
		float var_I, cov_Ip;

		sum[0] = *v0_p1 * (r + 1);
		sum[1] = *v1_p1 * (r + 1);
		sum[2] = *v2_p1 * (r + 1);
		sum[3] = *v3_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2;
			v0_p2++;
			sum[1] += *v1_p2;
			v1_p2++;
			sum[2] += *v2_p2;
			v2_p2++;
			sum[3] += *v3_p2;
			v3_p2++;
		}
		tmp[0] = sum[0] * div;
		tmp[1] = sum[1] * div;
		tmp[2] = sum[2] * div;
		tmp[3] = sum[3] * div;
		var_I = tmp[2] - tmp[0] * tmp[0];
		cov_Ip = tmp[3] - tmp[0] * tmp[1];
		*a_p = cov_Ip / (var_I + eps);
		*b_p = tmp[1] - *a_p * tmp[0];
		a_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum[4] = { 0.f };
		float tmp[4] = { 0.f };
		float var_I, cov_Ip;

		sum[0] = *v0_p1 * (r + 1);
		sum[1] = *v1_p1 * (r + 1);
		sum[2] = *v2_p1 * (r + 1);
		sum[3] = *v3_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2;
			v0_p2++;
			sum[1] += *v1_p2;
			v1_p2++;
			sum[2] += *v2_p2;
			v2_p2++;
			sum[3] += *v3_p2;
			v3_p2++;
		}
		tmp[0] = sum[0] * div;
		tmp[1] = sum[1] * div;
		tmp[2] = sum[2] * div;
		tmp[3] = sum[3] * div;
		var_I = tmp[2] - tmp[0] * tmp[0];
		cov_Ip = tmp[3] - tmp[0] * tmp[1];
		*a_p = cov_Ip / (var_I + eps);
		*b_p = tmp[1] - *a_p * tmp[0];
		a_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum[0] += *v0_p2 - *v0_p1;
			v0_p1++;
			sum[1] += *v1_p2 - *v1_p1;
			v1_p1++;
			sum[2] += *v2_p2 - *v2_p1;
			v2_p1++;
			sum[3] += *v3_p2 - *v3_p1;
			v3_p1++;

			tmp[0] = sum[0] * div;
			tmp[1] = sum[1] * div;
			tmp[2] = sum[2] * div;
			tmp[3] = sum[3] * div;
			var_I = tmp[2] - tmp[0] * tmp[0];
			cov_Ip = tmp[3] - tmp[0] * tmp[1];
			*a_p = cov_Ip / (var_I + eps);
			*b_p = tmp[1] - *a_p * tmp[0];
			a_p++;
			b_p++;
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 4;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 4;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 4;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 4;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum0 = _mm_setzero_ps();
		__m128 mSum1 = _mm_setzero_ps();
		__m128 mSum2 = _mm_setzero_ps();
		__m128 mSum3 = _mm_setzero_ps();
		__m128 m0, m1, m2, m3, m4, m5, m6, m7;
		__m128 mTmp[4];

		mSum0 = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum1 = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum2 = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum3 = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_load_ps(v0_p2));
			mSum1 = _mm_add_ps(mSum1, _mm_load_ps(v1_p2));
			mSum2 = _mm_add_ps(mSum2, _mm_load_ps(v2_p2));
			mSum3 = _mm_add_ps(mSum3, _mm_load_ps(v3_p2));
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
		}
		m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
		m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
		m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

		_mm_store_ps(a_p, m6);
		_mm_store_ps(b_p, m7);
		a_p += 4;
		b_p += 4;

		mTmp[0] = _mm_load_ps(v0_p1);
		mTmp[1] = _mm_load_ps(v1_p1);
		mTmp[2] = _mm_load_ps(v2_p1);
		mTmp[3] = _mm_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, mTmp[3]);

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		mTmp[0] = _mm_load_ps(v0_p2);
		mTmp[1] = _mm_load_ps(v1_p2);
		mTmp[2] = _mm_load_ps(v2_p2);
		mTmp[3] = _mm_load_ps(v3_p2);
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum0 = _mm_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			a_p += 4;
			b_p += 4;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 4;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 4;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 4;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 4;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum0 = _mm_setzero_ps();
		__m128 mSum1 = _mm_setzero_ps();
		__m128 mSum2 = _mm_setzero_ps();
		__m128 mSum3 = _mm_setzero_ps();
		__m128 m0, m1, m2, m3, m4, m5, m6, m7;
		__m128 mTmp[4];

		mSum0 = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum1 = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum2 = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum3 = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_load_ps(v0_p2));
			mSum1 = _mm_add_ps(mSum1, _mm_load_ps(v1_p2));
			mSum2 = _mm_add_ps(mSum2, _mm_load_ps(v2_p2));
			mSum3 = _mm_add_ps(mSum3, _mm_load_ps(v3_p2));
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
		}
		m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
		m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
		m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

		_mm_store_ps(a_p, m6);
		_mm_store_ps(b_p, m7);
		a_p += 4;
		b_p += 4;

		mTmp[0] = _mm_load_ps(v0_p1);
		mTmp[1] = _mm_load_ps(v1_p1);
		mTmp[2] = _mm_load_ps(v2_p1);
		mTmp[3] = _mm_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, mTmp[3]);

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		mTmp[0] = _mm_load_ps(v0_p2);
		mTmp[1] = _mm_load_ps(v1_p2);
		mTmp[2] = _mm_load_ps(v2_p2);
		mTmp[3] = _mm_load_ps(v3_p2);
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum0 = _mm_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			a_p += 4;
			b_p += 4;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 4;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 4;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 4;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 4;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum0 = _mm_setzero_ps();
		__m128 mSum1 = _mm_setzero_ps();
		__m128 mSum2 = _mm_setzero_ps();
		__m128 mSum3 = _mm_setzero_ps();
		__m128 m0, m1, m2, m3, m4, m5, m6, m7;
		__m128 mTmp[4];

		mSum0 = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum1 = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum2 = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum3 = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_load_ps(v0_p2));
			mSum1 = _mm_add_ps(mSum1, _mm_load_ps(v1_p2));
			mSum2 = _mm_add_ps(mSum2, _mm_load_ps(v2_p2));
			mSum3 = _mm_add_ps(mSum3, _mm_load_ps(v3_p2));
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
		}
		m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
		m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
		m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

		_mm_store_ps(a_p, m6);
		_mm_store_ps(b_p, m7);
		a_p += 4;
		b_p += 4;

		mTmp[0] = _mm_load_ps(v0_p1);
		mTmp[1] = _mm_load_ps(v1_p1);
		mTmp[2] = _mm_load_ps(v2_p1);
		mTmp[3] = _mm_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, mTmp[3]);

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum0 = _mm_add_ps(mSum0, _mm_loadu_ps(v0_p2));
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, _mm_loadu_ps(v1_p2));
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, _mm_loadu_ps(v2_p2));
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, _mm_loadu_ps(v3_p2));
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			v0_p2 += 4;
			v1_p2 += 4;
			v2_p2 += 4;
			v3_p2 += 4;
			a_p += 4;
			b_p += 4;
		}

		mTmp[0] = _mm_load_ps(v0_p2);
		mTmp[1] = _mm_load_ps(v1_p2);
		mTmp[2] = _mm_load_ps(v2_p2);
		mTmp[3] = _mm_load_ps(v3_p2);
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum0 = _mm_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm_sub_ps(mSum0, _mm_loadu_ps(v0_p1));
			mSum1 = _mm_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm_sub_ps(mSum1, _mm_loadu_ps(v1_p1));
			mSum2 = _mm_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm_sub_ps(mSum2, _mm_loadu_ps(v2_p1));
			mSum3 = _mm_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm_sub_ps(mSum3, _mm_loadu_ps(v3_p1));

			m0 = _mm_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm_sub_ps(m2, _mm_mul_ps(m0, m0));	//var_I
			m5 = _mm_sub_ps(m3, _mm_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm_div_ps(m5, _mm_add_ps(m4, mEps));
			m7 = _mm_sub_ps(m1, _mm_mul_ps(m6, m0));

			_mm_store_ps(a_p, m6);
			_mm_store_ps(b_p, m7);

			v0_p1 += 4;
			v1_p1 += 4;
			v2_p1 += 4;
			v3_p1 += 4;
			a_p += 4;
			b_p += 4;
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 8;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 8;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 8;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 8;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum0 = _mm256_setzero_ps();
		__m256 mSum1 = _mm256_setzero_ps();
		__m256 mSum2 = _mm256_setzero_ps();
		__m256 mSum3 = _mm256_setzero_ps();
		__m256 m0, m1, m2, m3, m4, m5, m6, m7;
		__m256 mTmp[4];

		mSum0 = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum1 = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum2 = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum3 = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_load_ps(v0_p2));
			mSum1 = _mm256_add_ps(mSum1, _mm256_load_ps(v1_p2));
			mSum2 = _mm256_add_ps(mSum2, _mm256_load_ps(v2_p2));
			mSum3 = _mm256_add_ps(mSum3, _mm256_load_ps(v3_p2));
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
		}
		m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
		m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
		m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

		_mm256_store_ps(a_p, m6);
		_mm256_store_ps(b_p, m7);
		a_p += 8;
		b_p += 8;

		mTmp[0] = _mm256_load_ps(v0_p1);
		mTmp[1] = _mm256_load_ps(v1_p1);
		mTmp[2] = _mm256_load_ps(v2_p1);
		mTmp[3] = _mm256_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, mTmp[3]);

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		mTmp[0] = _mm256_load_ps(v0_p2);
		mTmp[1] = _mm256_load_ps(v1_p2);
		mTmp[2] = _mm256_load_ps(v2_p2);
		mTmp[3] = _mm256_load_ps(v3_p2);
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			a_p += 8;
			b_p += 8;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 8;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 8;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 8;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 8;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum0 = _mm256_setzero_ps();
		__m256 mSum1 = _mm256_setzero_ps();
		__m256 mSum2 = _mm256_setzero_ps();
		__m256 mSum3 = _mm256_setzero_ps();
		__m256 m0, m1, m2, m3, m4, m5, m6, m7;
		__m256 mTmp[4];

		mSum0 = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum1 = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum2 = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum3 = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_load_ps(v0_p2));
			mSum1 = _mm256_add_ps(mSum1, _mm256_load_ps(v1_p2));
			mSum2 = _mm256_add_ps(mSum2, _mm256_load_ps(v2_p2));
			mSum3 = _mm256_add_ps(mSum3, _mm256_load_ps(v3_p2));
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
		}
		m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
		m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
		m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

		_mm256_store_ps(a_p, m6);
		_mm256_store_ps(b_p, m7);
		a_p += 8;
		b_p += 8;

		mTmp[0] = _mm256_load_ps(v0_p1);
		mTmp[1] = _mm256_load_ps(v1_p1);
		mTmp[2] = _mm256_load_ps(v2_p1);
		mTmp[3] = _mm256_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, mTmp[3]);

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		mTmp[0] = _mm256_load_ps(v0_p2);
		mTmp[1] = _mm256_load_ps(v1_p2);
		mTmp[2] = _mm256_load_ps(v2_p2);
		mTmp[3] = _mm256_load_ps(v3_p2);
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			a_p += 8;
			b_p += 8;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide1_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = tempVec[0].ptr<float>(j);
		float* v1_p1 = tempVec[1].ptr<float>(j);
		float* v2_p1 = tempVec[2].ptr<float>(j);
		float* v3_p1 = tempVec[3].ptr<float>(j);
		float* v0_p2 = tempVec[0].ptr<float>(j) + 8;
		float* v1_p2 = tempVec[1].ptr<float>(j) + 8;
		float* v2_p2 = tempVec[2].ptr<float>(j) + 8;
		float* v3_p2 = tempVec[3].ptr<float>(j) + 8;

		float* a_p = a.ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum0 = _mm256_setzero_ps();
		__m256 mSum1 = _mm256_setzero_ps();
		__m256 mSum2 = _mm256_setzero_ps();
		__m256 mSum3 = _mm256_setzero_ps();
		__m256 m0, m1, m2, m3, m4, m5, m6, m7;
		__m256 mTmp[4];

		mSum0 = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum1 = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum2 = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum3 = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_load_ps(v0_p2));
			mSum1 = _mm256_add_ps(mSum1, _mm256_load_ps(v1_p2));
			mSum2 = _mm256_add_ps(mSum2, _mm256_load_ps(v2_p2));
			mSum3 = _mm256_add_ps(mSum3, _mm256_load_ps(v3_p2));
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
		}
		m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
		m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
		m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
		m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
		m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
		m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

		m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
		m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

		_mm256_store_ps(a_p, m6);
		_mm256_store_ps(b_p, m7);
		a_p += 8;
		b_p += 8;

		mTmp[0] = _mm256_load_ps(v0_p1);
		mTmp[1] = _mm256_load_ps(v1_p1);
		mTmp[2] = _mm256_load_ps(v2_p1);
		mTmp[3] = _mm256_load_ps(v3_p1);
		for (int i = 1; i <= r; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, mTmp[0]);
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, mTmp[1]);
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, mTmp[2]);
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, mTmp[3]);

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, _mm256_loadu_ps(v0_p2));
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, _mm256_loadu_ps(v1_p2));
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, _mm256_loadu_ps(v2_p2));
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, _mm256_loadu_ps(v3_p2));
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			v0_p2 += 8;
			v1_p2 += 8;
			v2_p2 += 8;
			v3_p2 += 8;
			a_p += 8;
			b_p += 8;
		}

		mTmp[0] = _mm256_load_ps(v0_p2);
		mTmp[1] = _mm256_load_ps(v1_p2);
		mTmp[2] = _mm256_load_ps(v2_p2);
		mTmp[3] = _mm256_load_ps(v3_p2);
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum0 = _mm256_add_ps(mSum0, mTmp[0]);
			mSum0 = _mm256_sub_ps(mSum0, _mm256_loadu_ps(v0_p1));
			mSum1 = _mm256_add_ps(mSum1, mTmp[1]);
			mSum1 = _mm256_sub_ps(mSum1, _mm256_loadu_ps(v1_p1));
			mSum2 = _mm256_add_ps(mSum2, mTmp[2]);
			mSum2 = _mm256_sub_ps(mSum2, _mm256_loadu_ps(v2_p1));
			mSum3 = _mm256_add_ps(mSum3, mTmp[3]);
			mSum3 = _mm256_sub_ps(mSum3, _mm256_loadu_ps(v3_p1));

			m0 = _mm256_mul_ps(mSum0, mDiv);	//mean_I
			m1 = _mm256_mul_ps(mSum1, mDiv);	//mean_p
			m2 = _mm256_mul_ps(mSum2, mDiv);	//corr_I
			m3 = _mm256_mul_ps(mSum3, mDiv);	//corr_Ip
			m4 = _mm256_sub_ps(m2, _mm256_mul_ps(m0, m0));	//var_I
			m5 = _mm256_sub_ps(m3, _mm256_mul_ps(m0, m1));	//cov_Ip

			m6 = _mm256_div_ps(m5, _mm256_add_ps(m4, mEps));
			m7 = _mm256_sub_ps(m1, _mm256_mul_ps(m6, m0));

			_mm256_store_ps(a_p, m6);
			_mm256_store_ps(b_p, m7);

			v0_p1 += 8;
			v1_p1 += 8;
			v2_p1 += 8;
			v3_p1 += 8;
			a_p += 8;
			b_p += 8;
		}
	}
}



void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 1;
		float* v1_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_b

		float suma = 0.f, sumb = 0.f;

		suma = *v0_p1 * (r + 1);
		sumb = *v1_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2;
			v0_p2++;
			sumb += *v1_p2;
			v1_p2++;
		}
		*d0_p = suma;
		d0_p += step;
		*d1_p = sumb;
		d1_p += step;

		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 1;
		float* v1_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_b

		float suma = 0.f, sumb = 0.f;

		suma = *v0_p1 * (r + 1);
		sumb = *v1_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2;
			v0_p2++;
			sumb += *v1_p2;
			v1_p2++;
		}
		*d0_p = suma;
		d0_p += step;
		*d1_p = sumb;
		d1_p += step;

		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 1;
		float* v1_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_b

		float suma = 0.f, sumb = 0.f;

		suma = *v0_p1 * (r + 1);
		sumb = *v1_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2;
			v0_p2++;
			sumb += *v1_p2;
			v1_p2++;
		}
		*d0_p = suma;
		d0_p += step;
		*d1_p = sumb;
		d1_p += step;

		for (int j = 1; j <= r; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			suma += *v0_p2 - *v0_p1;
			v0_p1++;
			sumb += *v1_p2 - *v1_p1;
			v1_p1++;

			*d0_p = suma;
			d0_p += step;
			*d1_p = sumb;
			d1_p += step;
		}
	}
}



void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 4;
		float* v1_p2 = b.ptr<float>(j) + 4;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 4;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4;

		__m128 mSum_a = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_a = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 4;
		float* v1_p2 = b.ptr<float>(j) + 4;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 4;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4;

		__m128 mSum_a = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_a = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 4;
		float* v1_p2 = b.ptr<float>(j) + 4;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 4;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4;

		__m128 mSum_a = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_a = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_a = _mm_add_ps(mSum_a, _mm_load_ps(v0_p2));
			mSum_a = _mm_sub_ps(mSum_a, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v1_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v1_p1));
			v1_p1 += 4;

			_mm_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}



void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 8;
		float* v1_p2 = b.ptr<float>(j) + 8;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 8;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8;

		__m256 mSum_a = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_a = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 8;
		float* v1_p2 = b.ptr<float>(j) + 8;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 8;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8;

		__m256 mSum_a = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_a = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide1_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = a.ptr<float>(j);
		float* v1_p1 = b.ptr<float>(j);
		float* v0_p2 = a.ptr<float>(j) + 8;
		float* v1_p2 = b.ptr<float>(j) + 8;

		float* d0_p = tempVec[0].ptr<float>(0) + j * 8;
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8;

		__m256 mSum_a = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_a = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_a);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_b);
		d1_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_a = _mm256_add_ps(mSum_a, _mm256_load_ps(v0_p2));
			mSum_a = _mm256_sub_ps(mSum_a, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v1_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v1_p1));
			v1_p1 += 8;

			_mm256_store_ps(d0_p, mSum_a);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_b);
			d1_p += step;
		}
	}
}



void RowSumFilter_ab2q_Guide1_Transpose_Inverse::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* a_p1 = tempVec[0].ptr<float>(j);
		float* b_p1 = tempVec[1].ptr<float>(j);
		float* a_p2 = tempVec[0].ptr<float>(j) + 1;
		float* b_p2 = tempVec[1].ptr<float>(j) + 1;

		float* I_p = I.ptr<float>(j);
		float* q_p = q.ptr<float>(j);

		float suma = 0.f, sumb = 0.f;
		suma += *a_p1 * (r + 1);
		sumb += *b_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2;
			a_p2++;
			sumb += *b_p2;
			b_p2++;
		}
		*q_p = (suma * *I_p + sumb) * div;
		I_p++;
		q_p++;

		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p1++;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			sumb += *b_p2 - *b_p1;
			b_p1++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
	}
}

void RowSumFilter_ab2q_Guide1_Transpose_Inverse::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* a_p1 = tempVec[0].ptr<float>(j);
		float* b_p1 = tempVec[1].ptr<float>(j);
		float* a_p2 = tempVec[0].ptr<float>(j) + 1;
		float* b_p2 = tempVec[1].ptr<float>(j) + 1;

		float* I_p = I.ptr<float>(j);
		float* q_p = q.ptr<float>(j);

		float suma = 0.f, sumb = 0.f;
		suma += *a_p1 * (r + 1);
		sumb += *b_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2;
			a_p2++;
			sumb += *b_p2;
			b_p2++;
		}
		*q_p = (suma * *I_p + sumb) * div;
		I_p++;
		q_p++;

		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p1++;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			sumb += *b_p2 - *b_p1;
			b_p1++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
	}
}

void RowSumFilter_ab2q_Guide1_Transpose_Inverse::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* a_p1 = tempVec[0].ptr<float>(j);
		float* b_p1 = tempVec[1].ptr<float>(j);
		float* a_p2 = tempVec[0].ptr<float>(j) + 1;
		float* b_p2 = tempVec[1].ptr<float>(j) + 1;

		float* I_p = I.ptr<float>(j);
		float* q_p = q.ptr<float>(j);

		float suma = 0.f, sumb = 0.f;
		suma += *a_p1 * (r + 1);
		sumb += *b_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2;
			a_p2++;
			sumb += *b_p2;
			b_p2++;
		}
		*q_p = (suma * *I_p + sumb) * div;
		I_p++;
		q_p++;

		for (int i = 1; i <= r; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			a_p2++;
			sumb += *b_p2 - *b_p1;
			b_p1++;
			b_p2++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			suma += *a_p2 - *a_p1;
			a_p1++;
			sumb += *b_p2 - *b_p1;
			b_p1++;

			*q_p = (suma * *I_p + sumb) * div;
			I_p++;
			q_p++;
		}
	}
}



/*
 * Guide3
 */
void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + j;	// corr_I_rr
		float* d10_p = tempVec[10].ptr<float>(0) + j;	// cov_Ip_b
		float* d11_p = tempVec[11].ptr<float>(0) + j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + j;	// cov_Ip_r

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p += step;
		*d01_p = sum01;
		d01_p += step;
		*d02_p = sum02;
		d02_p += step;
		*d03_p = sum03;
		d03_p += step;
		*d04_p = sum04;
		d04_p += step;
		*d05_p = sum05;
		d05_p += step;
		*d06_p = sum06;
		d06_p += step;
		*d07_p = sum07;
		d07_p += step;
		*d08_p = sum08;
		d08_p += step;
		*d09_p = sum09;
		d09_p += step;
		*d10_p = sum10;
		d10_p += step;
		*d11_p = sum11;
		d11_p += step;
		*d12_p = sum12;
		d12_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s00_p2++;
			s01_p1++;
			s01_p2++;
			s02_p1++;
			s02_p2++;
			s03_p1++;
			s03_p2++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + j;	// corr_I_rr
		float* d10_p = tempVec[10].ptr<float>(0) + j;	// cov_Ip_b
		float* d11_p = tempVec[11].ptr<float>(0) + j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + j;	// cov_Ip_r

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p += step;
		*d01_p = sum01;
		d01_p += step;
		*d02_p = sum02;
		d02_p += step;
		*d03_p = sum03;
		d03_p += step;
		*d04_p = sum04;
		d04_p += step;
		*d05_p = sum05;
		d05_p += step;
		*d06_p = sum06;
		d06_p += step;
		*d07_p = sum07;
		d07_p += step;
		*d08_p = sum08;
		d08_p += step;
		*d09_p = sum09;
		d09_p += step;
		*d10_p = sum10;
		d10_p += step;
		*d11_p = sum11;
		d11_p += step;
		*d12_p = sum12;
		d12_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s00_p2++;
			s01_p1++;
			s01_p2++;
			s02_p1++;
			s02_p2++;
			s03_p1++;
			s03_p2++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + j;	// corr_I_rr
		float* d10_p = tempVec[10].ptr<float>(0) + j;	// cov_Ip_b
		float* d11_p = tempVec[11].ptr<float>(0) + j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + j;	// cov_Ip_r

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p += step;
		*d01_p = sum01;
		d01_p += step;
		*d02_p = sum02;
		d02_p += step;
		*d03_p = sum03;
		d03_p += step;
		*d04_p = sum04;
		d04_p += step;
		*d05_p = sum05;
		d05_p += step;
		*d06_p = sum06;
		d06_p += step;
		*d07_p = sum07;
		d07_p += step;
		*d08_p = sum08;
		d08_p += step;
		*d09_p = sum09;
		d09_p += step;
		*d10_p = sum10;
		d10_p += step;
		*d11_p = sum11;
		d11_p += step;
		*d12_p = sum12;
		d12_p += step;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s00_p2++;
			s01_p1++;
			s01_p2++;
			s02_p1++;
			s02_p2++;
			s03_p1++;
			s03_p2++;
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			d00_p += step;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			d01_p += step;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			d02_p += step;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			d03_p += step;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			d04_p += step;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			d05_p += step;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			d06_p += step;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			d07_p += step;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			d08_p += step;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			d09_p += step;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			d10_p += step;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			d11_p += step;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;
			d12_p += step;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
		}
	}
}



void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 4 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 4 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 4 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 4 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 4 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 4 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 4 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 4 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 4 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 4 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 4 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 4 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 4 * j;	// cov_Ip_b
		
		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 4 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 4 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 4 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 4 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 4 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 4 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 4 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 4 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 4 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 4 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 4 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 4 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 4 * j;	// cov_Ip_b

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 4 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 4 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 4 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 4 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 4 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 4 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 4 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 4 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 4 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 4 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 4 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 4 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 4 * j;	// cov_Ip_b

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 3) == 3)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}



void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 8 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 8 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 8 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 8 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 8 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 8 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 8 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 8 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 8 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 8 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 8 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 8 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 8 * j;	// cov_Ip_b

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 8 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 8 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 8 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 8 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 8 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 8 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 8 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 8 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 8 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 8 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 8 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 8 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 8 * j;	// cov_Ip_b

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}

void RowSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = vI[0].ptr<float>(j);
		float* s01_p1 = vI[1].ptr<float>(j);
		float* s02_p1 = vI[2].ptr<float>(j);
		float* s03_p1 = p.ptr<float>(j);
		float* s00_p2 = vI[0].ptr<float>(j) + 1;
		float* s01_p2 = vI[1].ptr<float>(j) + 1;
		float* s02_p2 = vI[2].ptr<float>(j) + 1;
		float* s03_p2 = p.ptr<float>(j) + 1;

		float* d00_p = tempVec[0].ptr<float>(0) + 8 * j;	// mean_I_b
		float* d01_p = tempVec[1].ptr<float>(0) + 8 * j;	// mean_I_g
		float* d02_p = tempVec[2].ptr<float>(0) + 8 * j;	// mean_I_r
		float* d03_p = tempVec[3].ptr<float>(0) + 8 * j;	// mean_p
		float* d04_p = tempVec[4].ptr<float>(0) + 8 * j;	// corr_I_bb
		float* d05_p = tempVec[5].ptr<float>(0) + 8 * j;	// corr_I_bg
		float* d06_p = tempVec[6].ptr<float>(0) + 8 * j;	// corr_I_br
		float* d07_p = tempVec[7].ptr<float>(0) + 8 * j;	// corr_I_gg
		float* d08_p = tempVec[8].ptr<float>(0) + 8 * j;	// corr_I_gr
		float* d09_p = tempVec[9].ptr<float>(0) + 8 * j;	// corr_I_rr
		float* d11_p = tempVec[11].ptr<float>(0) + 8 * j;	// cov_Ip_g
		float* d12_p = tempVec[12].ptr<float>(0) + 8 * j;	// cov_Ip_r
		float* d10_p = tempVec[10].ptr<float>(0) + 8 * j;	// cov_Ip_b

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += (*s00_p1 * *s00_p1) * (r + 1);
		sum05 += (*s00_p1 * *s01_p1) * (r + 1);
		sum06 += (*s00_p1 * *s02_p1) * (r + 1);
		sum07 += (*s01_p1 * *s01_p1) * (r + 1);
		sum08 += (*s01_p1 * *s02_p1) * (r + 1);
		sum09 += (*s02_p1 * *s02_p1) * (r + 1);
		sum10 += (*s03_p1 * *s00_p1) * (r + 1);
		sum11 += (*s03_p1 * *s01_p1) * (r + 1);
		sum12 += (*s03_p1 * *s02_p1) * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			sum01 += *s01_p2;
			sum02 += *s02_p2;
			sum03 += *s03_p2;
			sum04 += *s00_p2 * *s00_p2;
			sum05 += *s00_p2 * *s01_p2;
			sum06 += *s00_p2 * *s02_p2;
			sum07 += *s01_p2 * *s01_p2;
			sum08 += *s01_p2 * *s02_p2;
			sum09 += *s02_p2 * *s02_p2;
			sum10 += *s03_p2 * *s00_p2;
			sum11 += *s03_p2 * *s01_p2;
			sum12 += *s03_p2 * *s02_p2;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;
		}
		*d00_p = sum00;
		d00_p++;
		*d01_p = sum01;
		d01_p++;
		*d02_p = sum02;
		d02_p++;
		*d03_p = sum03;
		d03_p++;
		*d04_p = sum04;
		d04_p++;
		*d05_p = sum05;
		d05_p++;
		*d06_p = sum06;
		d06_p++;
		*d07_p = sum07;
		d07_p++;
		*d08_p = sum08;
		d08_p++;
		*d09_p = sum09;
		d09_p++;
		*d10_p = sum10;
		d10_p++;
		*d11_p = sum11;
		d11_p++;
		*d12_p = sum12;
		d12_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = r + 1; i < img_col - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;
			s00_p2++;
			s01_p2++;
			s02_p2++;
			s03_p2++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
		for (int i = img_col - r - 1; i < img_col; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			*d00_p = sum00;
			sum01 += *s01_p2 - *s01_p1;
			*d01_p = sum01;
			sum02 += *s02_p2 - *s02_p1;
			*d02_p = sum02;
			sum03 += *s03_p2 - *s03_p1;
			*d03_p = sum03;
			sum04 += (*s00_p2 * *s00_p2) - (*s00_p1 * *s00_p1);
			*d04_p = sum04;
			sum05 += (*s00_p2 * *s01_p2) - (*s00_p1 * *s01_p1);
			*d05_p = sum05;
			sum06 += (*s00_p2 * *s02_p2) - (*s00_p1 * *s02_p1);
			*d06_p = sum06;
			sum07 += (*s01_p2 * *s01_p2) - (*s01_p1 * *s01_p1);
			*d07_p = sum07;
			sum08 += (*s01_p2 * *s02_p2) - (*s01_p1 * *s02_p1);
			*d08_p = sum08;
			sum09 += (*s02_p2 * *s02_p2) - (*s02_p1 * *s02_p1);
			*d09_p = sum09;
			sum10 += (*s03_p2 * *s00_p2) - (*s03_p1 * *s00_p1);
			*d10_p = sum10;
			sum11 += (*s03_p2 * *s01_p2) - (*s03_p1 * *s01_p1);
			*d11_p = sum11;
			sum12 += (*s03_p2 * *s02_p2) - (*s03_p1 * *s02_p1);
			*d12_p = sum12;

			s00_p1++;
			s01_p1++;
			s02_p1++;
			s03_p1++;

			if ((i & 7) == 7)
			{
				d00_p += step;
				d01_p += step;
				d02_p += step;
				d03_p += step;
				d04_p += step;
				d05_p += step;
				d06_p += step;
				d07_p += step;
				d08_p += step;
				d09_p += step;
				d10_p += step;
				d11_p += step;
				d12_p += step;
			}
			else
			{
				d00_p++;
				d01_p++;
				d02_p++;
				d03_p++;
				d04_p++;
				d05_p++;
				d06_p++;
				d07_p++;
				d08_p++;
				d09_p++;
				d10_p++;
				d11_p++;
				d12_p++;
			}
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);
		float* s01_p1 = tempVec[1].ptr<float>(j);
		float* s02_p1 = tempVec[2].ptr<float>(j);
		float* s03_p1 = tempVec[3].ptr<float>(j);
		float* s04_p1 = tempVec[4].ptr<float>(j);
		float* s05_p1 = tempVec[5].ptr<float>(j);
		float* s06_p1 = tempVec[6].ptr<float>(j);
		float* s07_p1 = tempVec[7].ptr<float>(j);
		float* s08_p1 = tempVec[8].ptr<float>(j);
		float* s09_p1 = tempVec[9].ptr<float>(j);
		float* s10_p1 = tempVec[10].ptr<float>(j);
		float* s11_p1 = tempVec[11].ptr<float>(j);
		float* s12_p1 = tempVec[12].ptr<float>(j);

		float* s00_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 1;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 1;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 1;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 1;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 1;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 1;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 1;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 1;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 1;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 1;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		float tmp00, tmp01, tmp02, tmp03, tmp04, tmp05, tmp06, tmp07, tmp08, tmp09, tmp10, tmp11, tmp12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		float bb, bg, br, gg, gr, rr;
		float covb, covg, covr;
		float det, id;


		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += *s04_p1 * (r + 1);
		sum05 += *s05_p1 * (r + 1);
		sum06 += *s06_p1 * (r + 1);
		sum07 += *s07_p1 * (r + 1);
		sum08 += *s08_p1 * (r + 1);
		sum09 += *s09_p1 * (r + 1);
		sum10 += *s10_p1 * (r + 1);
		sum11 += *s11_p1 * (r + 1);
		sum12 += *s12_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			s00_p2++;
			sum01 += *s01_p2;
			s01_p2++;
			sum02 += *s02_p2;
			s02_p2++;
			sum03 += *s03_p2;
			s03_p2++;
			sum04 += *s04_p2;
			s04_p2++;
			sum05 += *s05_p2;
			s05_p2++;
			sum06 += *s06_p2;
			s06_p2++;
			sum07 += *s07_p2;
			s07_p2++;
			sum08 += *s08_p2;
			s08_p2++;
			sum09 += *s09_p2;
			s09_p2++;
			sum10 += *s10_p2;
			s10_p2++;
			sum11 += *s11_p2;
			s11_p2++;
			sum12 += *s12_p2;
			s12_p2++;
		}
		tmp00 = sum00 * div;
		tmp01 = sum01 * div;
		tmp02 = sum02 * div;
		tmp03 = sum03 * div;
		tmp04 = sum04 * div;
		tmp05 = sum05 * div;
		tmp06 = sum06 * div;
		tmp07 = sum07 * div;
		tmp08 = sum08 * div;
		tmp09 = sum09 * div;
		tmp10 = sum10 * div;
		tmp11 = sum11 * div;
		tmp12 = sum12 * div;

		bb = tmp04 - tmp00 * tmp00;
		bg = tmp05 - tmp00 * tmp01;
		br = tmp06 - tmp00 * tmp02;
		gg = tmp07 - tmp01 * tmp01;
		gr = tmp08 - tmp01 * tmp02;
		rr = tmp09 - tmp02 * tmp02;
		covb = tmp10 - tmp00 * tmp03;
		covg = tmp11 - tmp01 * tmp03;
		covr = tmp12 - tmp02 * tmp03;

		bb += eps;
		gg += eps;
		rr += eps;

		det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
		id = 1.f / det;

		float c0 = gg*rr - gr*gr;
		float c1 = br*gr - bg*rr;
		float c2 = bg*gr - br*gg;
		float c4 = bb*rr - br*br;
		float c5 = br*bg - bb*gr;
		float c8 = bb*gg - bg*bg;

		*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
		*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
		*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
		*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
		a_b_p++;
		a_g_p++;
		a_r_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);
		float* s01_p1 = tempVec[1].ptr<float>(j);
		float* s02_p1 = tempVec[2].ptr<float>(j);
		float* s03_p1 = tempVec[3].ptr<float>(j);
		float* s04_p1 = tempVec[4].ptr<float>(j);
		float* s05_p1 = tempVec[5].ptr<float>(j);
		float* s06_p1 = tempVec[6].ptr<float>(j);
		float* s07_p1 = tempVec[7].ptr<float>(j);
		float* s08_p1 = tempVec[8].ptr<float>(j);
		float* s09_p1 = tempVec[9].ptr<float>(j);
		float* s10_p1 = tempVec[10].ptr<float>(j);
		float* s11_p1 = tempVec[11].ptr<float>(j);
		float* s12_p1 = tempVec[12].ptr<float>(j);

		float* s00_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 1;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 1;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 1;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 1;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 1;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 1;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 1;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 1;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 1;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 1;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		float tmp00, tmp01, tmp02, tmp03, tmp04, tmp05, tmp06, tmp07, tmp08, tmp09, tmp10, tmp11, tmp12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		float bb, bg, br, gg, gr, rr;
		float covb, covg, covr;
		float det, id;


		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += *s04_p1 * (r + 1);
		sum05 += *s05_p1 * (r + 1);
		sum06 += *s06_p1 * (r + 1);
		sum07 += *s07_p1 * (r + 1);
		sum08 += *s08_p1 * (r + 1);
		sum09 += *s09_p1 * (r + 1);
		sum10 += *s10_p1 * (r + 1);
		sum11 += *s11_p1 * (r + 1);
		sum12 += *s12_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			s00_p2++;
			sum01 += *s01_p2;
			s01_p2++;
			sum02 += *s02_p2;
			s02_p2++;
			sum03 += *s03_p2;
			s03_p2++;
			sum04 += *s04_p2;
			s04_p2++;
			sum05 += *s05_p2;
			s05_p2++;
			sum06 += *s06_p2;
			s06_p2++;
			sum07 += *s07_p2;
			s07_p2++;
			sum08 += *s08_p2;
			s08_p2++;
			sum09 += *s09_p2;
			s09_p2++;
			sum10 += *s10_p2;
			s10_p2++;
			sum11 += *s11_p2;
			s11_p2++;
			sum12 += *s12_p2;
			s12_p2++;
		}
		tmp00 = sum00 * div;
		tmp01 = sum01 * div;
		tmp02 = sum02 * div;
		tmp03 = sum03 * div;
		tmp04 = sum04 * div;
		tmp05 = sum05 * div;
		tmp06 = sum06 * div;
		tmp07 = sum07 * div;
		tmp08 = sum08 * div;
		tmp09 = sum09 * div;
		tmp10 = sum10 * div;
		tmp11 = sum11 * div;
		tmp12 = sum12 * div;

		bb = tmp04 - tmp00 * tmp00;
		bg = tmp05 - tmp00 * tmp01;
		br = tmp06 - tmp00 * tmp02;
		gg = tmp07 - tmp01 * tmp01;
		gr = tmp08 - tmp01 * tmp02;
		rr = tmp09 - tmp02 * tmp02;
		covb = tmp10 - tmp00 * tmp03;
		covg = tmp11 - tmp01 * tmp03;
		covr = tmp12 - tmp02 * tmp03;

		bb += eps;
		gg += eps;
		rr += eps;

		det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
		id = 1.f / det;

		float c0 = gg*rr - gr*gr;
		float c1 = br*gr - bg*rr;
		float c2 = bg*gr - br*gg;
		float c4 = bb*rr - br*br;
		float c5 = br*bg - bb*gr;
		float c8 = bb*gg - bg*bg;

		*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
		*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
		*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
		*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
		a_b_p++;
		a_g_p++;
		a_r_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);
		float* s01_p1 = tempVec[1].ptr<float>(j);
		float* s02_p1 = tempVec[2].ptr<float>(j);
		float* s03_p1 = tempVec[3].ptr<float>(j);
		float* s04_p1 = tempVec[4].ptr<float>(j);
		float* s05_p1 = tempVec[5].ptr<float>(j);
		float* s06_p1 = tempVec[6].ptr<float>(j);
		float* s07_p1 = tempVec[7].ptr<float>(j);
		float* s08_p1 = tempVec[8].ptr<float>(j);
		float* s09_p1 = tempVec[9].ptr<float>(j);
		float* s10_p1 = tempVec[10].ptr<float>(j);
		float* s11_p1 = tempVec[11].ptr<float>(j);
		float* s12_p1 = tempVec[12].ptr<float>(j);

		float* s00_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 1;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 1;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 1;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 1;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 1;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 1;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 1;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 1;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 1;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 1;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		float sum00, sum01, sum02, sum03, sum04, sum05, sum06, sum07, sum08, sum09, sum10, sum11, sum12;
		float tmp00, tmp01, tmp02, tmp03, tmp04, tmp05, tmp06, tmp07, tmp08, tmp09, tmp10, tmp11, tmp12;
		sum00 = sum01 = sum02 = sum03 = sum04 = sum05 = sum06 = sum07 = sum08 = sum09 = sum10 = sum11 = sum12 = 0.f;

		float bb, bg, br, gg, gr, rr;
		float covb, covg, covr;
		float det, id;


		sum00 += *s00_p1 * (r + 1);
		sum01 += *s01_p1 * (r + 1);
		sum02 += *s02_p1 * (r + 1);
		sum03 += *s03_p1 * (r + 1);
		sum04 += *s04_p1 * (r + 1);
		sum05 += *s05_p1 * (r + 1);
		sum06 += *s06_p1 * (r + 1);
		sum07 += *s07_p1 * (r + 1);
		sum08 += *s08_p1 * (r + 1);
		sum09 += *s09_p1 * (r + 1);
		sum10 += *s10_p1 * (r + 1);
		sum11 += *s11_p1 * (r + 1);
		sum12 += *s12_p1 * (r + 1);
		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2;
			s00_p2++;
			sum01 += *s01_p2;
			s01_p2++;
			sum02 += *s02_p2;
			s02_p2++;
			sum03 += *s03_p2;
			s03_p2++;
			sum04 += *s04_p2;
			s04_p2++;
			sum05 += *s05_p2;
			s05_p2++;
			sum06 += *s06_p2;
			s06_p2++;
			sum07 += *s07_p2;
			s07_p2++;
			sum08 += *s08_p2;
			s08_p2++;
			sum09 += *s09_p2;
			s09_p2++;
			sum10 += *s10_p2;
			s10_p2++;
			sum11 += *s11_p2;
			s11_p2++;
			sum12 += *s12_p2;
			s12_p2++;
		}
		tmp00 = sum00 * div;
		tmp01 = sum01 * div;
		tmp02 = sum02 * div;
		tmp03 = sum03 * div;
		tmp04 = sum04 * div;
		tmp05 = sum05 * div;
		tmp06 = sum06 * div;
		tmp07 = sum07 * div;
		tmp08 = sum08 * div;
		tmp09 = sum09 * div;
		tmp10 = sum10 * div;
		tmp11 = sum11 * div;
		tmp12 = sum12 * div;

		bb = tmp04 - tmp00 * tmp00;
		bg = tmp05 - tmp00 * tmp01;
		br = tmp06 - tmp00 * tmp02;
		gg = tmp07 - tmp01 * tmp01;
		gr = tmp08 - tmp01 * tmp02;
		rr = tmp09 - tmp02 * tmp02;
		covb = tmp10 - tmp00 * tmp03;
		covg = tmp11 - tmp01 * tmp03;
		covr = tmp12 - tmp02 * tmp03;

		bb += eps;
		gg += eps;
		rr += eps;

		det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
		id = 1.f / det;

		float c0 = gg*rr - gr*gr;
		float c1 = br*gr - bg*rr;
		float c2 = bg*gr - br*gg;
		float c4 = bb*rr - br*br;
		float c5 = br*bg - bb*gr;
		float c8 = bb*gg - bg*bg;

		*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
		*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
		*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
		*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
		a_b_p++;
		a_g_p++;
		a_r_p++;
		b_p++;

		for (int i = 1; i <= r; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = r + 1; i < img_row - r - 1; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			s00_p2++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			s01_p2++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			s02_p2++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			s03_p2++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			s04_p2++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			s05_p2++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			s06_p2++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			s07_p2++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			s08_p2++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			s09_p2++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			s10_p2++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			s11_p2++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;
			s12_p2++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}

		for (int i = img_row - r - 1; i < img_row; i++)
		{
			sum00 += *s00_p2 - *s00_p1;
			s00_p1++;
			sum01 += *s01_p2 - *s01_p1;
			s01_p1++;
			sum02 += *s02_p2 - *s02_p1;
			s02_p1++;
			sum03 += *s03_p2 - *s03_p1;
			s03_p1++;
			sum04 += *s04_p2 - *s04_p1;
			s04_p1++;
			sum05 += *s05_p2 - *s05_p1;
			s05_p1++;
			sum06 += *s06_p2 - *s06_p1;
			s06_p1++;
			sum07 += *s07_p2 - *s07_p1;
			s07_p1++;
			sum08 += *s08_p2 - *s08_p1;
			s08_p1++;
			sum09 += *s09_p2 - *s09_p1;
			s09_p1++;
			sum10 += *s10_p2 - *s10_p1;
			s10_p1++;
			sum11 += *s11_p2 - *s11_p1;
			s11_p1++;
			sum12 += *s12_p2 - *s12_p1;
			s12_p1++;

			tmp00 = sum00 * div;
			tmp01 = sum01 * div;
			tmp02 = sum02 * div;
			tmp03 = sum03 * div;
			tmp04 = sum04 * div;
			tmp05 = sum05 * div;
			tmp06 = sum06 * div;
			tmp07 = sum07 * div;
			tmp08 = sum08 * div;
			tmp09 = sum09 * div;
			tmp10 = sum10 * div;
			tmp11 = sum11 * div;
			tmp12 = sum12 * div;

			bb = tmp04 - tmp00 * tmp00;
			bg = tmp05 - tmp00 * tmp01;
			br = tmp06 - tmp00 * tmp02;
			gg = tmp07 - tmp01 * tmp01;
			gr = tmp08 - tmp01 * tmp02;
			rr = tmp09 - tmp02 * tmp02;
			covb = tmp10 - tmp00 * tmp03;
			covg = tmp11 - tmp01 * tmp03;
			covr = tmp12 - tmp02 * tmp03;

			bb += eps;
			gg += eps;
			rr += eps;

			det = (bb*gg*rr) + (gr*bg*br) + (br*gr*bg) - (bb*gr*gr) - (br*gg*br) - (bg*bg*rr);
			id = 1.f / det;

			float c0 = gg*rr - gr*gr;
			float c1 = br*gr - bg*rr;
			float c2 = bg*gr - br*gg;
			float c4 = bb*rr - br*br;
			float c5 = br*bg - bb*gr;
			float c8 = bb*gg - bg*bg;

			*a_b_p = id * (covb*c0 + covg*c1 + covr*c2);
			*a_g_p = id * (covb*c1 + covg*c4 + covr*c5);
			*a_r_p = id * (covb*c2 + covg*c5 + covr*c8);
			*b_p = tmp03 - (*a_b_p * tmp00 + *a_g_p * tmp01 + *a_r_p * tmp02);
			a_b_p++;
			a_g_p++;
			a_r_p++;
			b_p++;
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 4;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 4;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 4;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 4;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 4;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 4;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 4;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 4;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 4;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 4;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 4;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 4;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 4;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m128 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m128 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m128 mCov00, mCov01, mCov02;
		__m128 mDet;

		mSum00 = _mm_setzero_ps();
		mSum01 = _mm_setzero_ps();
		mSum02 = _mm_setzero_ps();
		mSum03 = _mm_setzero_ps();
		mSum04 = _mm_setzero_ps();
		mSum05 = _mm_setzero_ps();
		mSum06 = _mm_setzero_ps();
		mSum07 = _mm_setzero_ps();
		mSum08 = _mm_setzero_ps();
		mSum09 = _mm_setzero_ps();
		mSum10 = _mm_setzero_ps();
		mSum11 = _mm_setzero_ps();
		mSum12 = _mm_setzero_ps();

		mSum00 = _mm_mul_ps(mBorder, _mm_load_ps(s00_p1));
		mSum01 = _mm_mul_ps(mBorder, _mm_load_ps(s01_p1));
		mSum02 = _mm_mul_ps(mBorder, _mm_load_ps(s02_p1));
		mSum03 = _mm_mul_ps(mBorder, _mm_load_ps(s03_p1));
		mSum04 = _mm_mul_ps(mBorder, _mm_load_ps(s04_p1));
		mSum05 = _mm_mul_ps(mBorder, _mm_load_ps(s05_p1));
		mSum06 = _mm_mul_ps(mBorder, _mm_load_ps(s06_p1));
		mSum07 = _mm_mul_ps(mBorder, _mm_load_ps(s07_p1));
		mSum08 = _mm_mul_ps(mBorder, _mm_load_ps(s08_p1));
		mSum09 = _mm_mul_ps(mBorder, _mm_load_ps(s09_p1));
		mSum10 = _mm_mul_ps(mBorder, _mm_load_ps(s10_p1));
		mSum11 = _mm_mul_ps(mBorder, _mm_load_ps(s11_p1));
		mSum12 = _mm_mul_ps(mBorder, _mm_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;
		}
		mTmp00 = _mm_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm_add_ps(mVar00, mEps);
		mVar03 = _mm_add_ps(mVar03, mEps);
		mVar05 = _mm_add_ps(mVar05, mEps);

		mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm_add_ps(mTmp04, mTmp05);
		mDet = _mm_add_ps(mDet, mTmp05);
		mDet = _mm_sub_ps(mDet, mTmp06);
		mDet = _mm_sub_ps(mDet, mTmp07);
		mDet = _mm_sub_ps(mDet, mTmp08);
		mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																		   //
																		   /*
																		   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																		   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																		   */
		mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm_mul_ps(mTmp10, mDet);
		//_mm_store_ps(a_b_p, mTmp10);
		_mm_stream_ps(a_b_p, mTmp10);
		a_b_p += 4;

		/*
		mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm_add_ps(mTmp11, _mm_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm_mul_ps(mTmp11, mDet);
		//_mm_store_ps(a_g_p, mTmp11);
		_mm_stream_ps(a_g_p, mTmp11);
		a_g_p += 4;

		/*
		mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm_mul_ps(mTmp12, mDet);
		//_mm_store_ps(a_r_p, mTmp12);
		_mm_stream_ps(a_r_p, mTmp12);
		a_r_p += 4;

		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
		//_mm_store_ps(b_p, mTmp03);
		_mm_stream_ps(b_p, mTmp03);
		b_p += 4;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 4;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 4;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 4;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 4;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 4;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 4;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 4;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 4;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 4;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 4;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 4;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 4;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 4;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m128 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m128 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m128 mCov00, mCov01, mCov02;
		__m128 mDet;

		mSum00 = _mm_setzero_ps();
		mSum01 = _mm_setzero_ps();
		mSum02 = _mm_setzero_ps();
		mSum03 = _mm_setzero_ps();
		mSum04 = _mm_setzero_ps();
		mSum05 = _mm_setzero_ps();
		mSum06 = _mm_setzero_ps();
		mSum07 = _mm_setzero_ps();
		mSum08 = _mm_setzero_ps();
		mSum09 = _mm_setzero_ps();
		mSum10 = _mm_setzero_ps();
		mSum11 = _mm_setzero_ps();
		mSum12 = _mm_setzero_ps();

		mSum00 = _mm_mul_ps(mBorder, _mm_load_ps(s00_p1));
		mSum01 = _mm_mul_ps(mBorder, _mm_load_ps(s01_p1));
		mSum02 = _mm_mul_ps(mBorder, _mm_load_ps(s02_p1));
		mSum03 = _mm_mul_ps(mBorder, _mm_load_ps(s03_p1));
		mSum04 = _mm_mul_ps(mBorder, _mm_load_ps(s04_p1));
		mSum05 = _mm_mul_ps(mBorder, _mm_load_ps(s05_p1));
		mSum06 = _mm_mul_ps(mBorder, _mm_load_ps(s06_p1));
		mSum07 = _mm_mul_ps(mBorder, _mm_load_ps(s07_p1));
		mSum08 = _mm_mul_ps(mBorder, _mm_load_ps(s08_p1));
		mSum09 = _mm_mul_ps(mBorder, _mm_load_ps(s09_p1));
		mSum10 = _mm_mul_ps(mBorder, _mm_load_ps(s10_p1));
		mSum11 = _mm_mul_ps(mBorder, _mm_load_ps(s11_p1));
		mSum12 = _mm_mul_ps(mBorder, _mm_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;
		}
		mTmp00 = _mm_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm_add_ps(mVar00, mEps);
		mVar03 = _mm_add_ps(mVar03, mEps);
		mVar05 = _mm_add_ps(mVar05, mEps);

		mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm_add_ps(mTmp04, mTmp05);
		mDet = _mm_add_ps(mDet, mTmp05);
		mDet = _mm_sub_ps(mDet, mTmp06);
		mDet = _mm_sub_ps(mDet, mTmp07);
		mDet = _mm_sub_ps(mDet, mTmp08);
		mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																		   //
																		   /*
																		   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																		   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																		   */
		mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm_mul_ps(mTmp10, mDet);
		//_mm_store_ps(a_b_p, mTmp10);
		_mm_stream_ps(a_b_p, mTmp10);
		a_b_p += 4;

		/*
		mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm_add_ps(mTmp11, _mm_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm_mul_ps(mTmp11, mDet);
		//_mm_store_ps(a_g_p, mTmp11);
		_mm_stream_ps(a_g_p, mTmp11);
		a_g_p += 4;

		/*
		mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm_mul_ps(mTmp12, mDet);
		//_mm_store_ps(a_r_p, mTmp12);
		_mm_stream_ps(a_r_p, mTmp12);
		a_r_p += 4;

		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
		//_mm_store_ps(b_p, mTmp03);
		_mm_stream_ps(b_p, mTmp03);
		b_p += 4;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 4;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 4;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 4;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 4;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 4;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 4;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 4;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 4;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 4;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 4;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 4;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 4;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 4;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m128 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m128 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m128 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m128 mCov00, mCov01, mCov02;
		__m128 mDet;

		mSum00 = _mm_setzero_ps();
		mSum01 = _mm_setzero_ps();
		mSum02 = _mm_setzero_ps();
		mSum03 = _mm_setzero_ps();
		mSum04 = _mm_setzero_ps();
		mSum05 = _mm_setzero_ps();
		mSum06 = _mm_setzero_ps();
		mSum07 = _mm_setzero_ps();
		mSum08 = _mm_setzero_ps();
		mSum09 = _mm_setzero_ps();
		mSum10 = _mm_setzero_ps();
		mSum11 = _mm_setzero_ps();
		mSum12 = _mm_setzero_ps();

		mSum00 = _mm_mul_ps(mBorder, _mm_load_ps(s00_p1));
		mSum01 = _mm_mul_ps(mBorder, _mm_load_ps(s01_p1));
		mSum02 = _mm_mul_ps(mBorder, _mm_load_ps(s02_p1));
		mSum03 = _mm_mul_ps(mBorder, _mm_load_ps(s03_p1));
		mSum04 = _mm_mul_ps(mBorder, _mm_load_ps(s04_p1));
		mSum05 = _mm_mul_ps(mBorder, _mm_load_ps(s05_p1));
		mSum06 = _mm_mul_ps(mBorder, _mm_load_ps(s06_p1));
		mSum07 = _mm_mul_ps(mBorder, _mm_load_ps(s07_p1));
		mSum08 = _mm_mul_ps(mBorder, _mm_load_ps(s08_p1));
		mSum09 = _mm_mul_ps(mBorder, _mm_load_ps(s09_p1));
		mSum10 = _mm_mul_ps(mBorder, _mm_load_ps(s10_p1));
		mSum11 = _mm_mul_ps(mBorder, _mm_load_ps(s11_p1));
		mSum12 = _mm_mul_ps(mBorder, _mm_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;
		}
		mTmp00 = _mm_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm_add_ps(mVar00, mEps);
		mVar03 = _mm_add_ps(mVar03, mEps);
		mVar05 = _mm_add_ps(mVar05, mEps);

		mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm_add_ps(mTmp04, mTmp05);
		mDet = _mm_add_ps(mDet, mTmp05);
		mDet = _mm_sub_ps(mDet, mTmp06);
		mDet = _mm_sub_ps(mDet, mTmp07);
		mDet = _mm_sub_ps(mDet, mTmp08);
		mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																		   //
																		   /*
																		   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																		   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																		   */
		mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm_mul_ps(mTmp10, mDet);
		//_mm_store_ps(a_b_p, mTmp10);
		_mm_stream_ps(a_b_p, mTmp10);
		a_b_p += 4;

		/*
		mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm_add_ps(mTmp11, _mm_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm_mul_ps(mTmp11, mDet);
		//_mm_store_ps(a_g_p, mTmp11);
		_mm_stream_ps(a_g_p, mTmp11);
		a_g_p += 4;

		/*
		mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm_mul_ps(mTmp12, mDet);
		//_mm_store_ps(a_r_p, mTmp12);
		_mm_stream_ps(a_r_p, mTmp12);
		a_r_p += 4;

		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
		//_mm_store_ps(b_p, mTmp03);
		_mm_stream_ps(b_p, mTmp03);
		b_p += 4;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;
			s00_p2 += 4;
			s01_p2 += 4;
			s02_p2 += 4;
			s03_p2 += 4;
			s04_p2 += 4;
			s05_p2 += 4;
			s06_p2 += 4;
			s07_p2 += 4;
			s08_p2 += 4;
			s09_p2 += 4;
			s10_p2 += 4;
			s11_p2 += 4;
			s12_p2 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}

		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum00 = _mm_add_ps(mSum00, _mm_loadu_ps(s00_p2));
			mSum00 = _mm_sub_ps(mSum00, _mm_load_ps(s00_p1));
			mSum01 = _mm_add_ps(mSum01, _mm_loadu_ps(s01_p2));
			mSum01 = _mm_sub_ps(mSum01, _mm_load_ps(s01_p1));
			mSum02 = _mm_add_ps(mSum02, _mm_loadu_ps(s02_p2));
			mSum02 = _mm_sub_ps(mSum02, _mm_load_ps(s02_p1));
			mSum03 = _mm_add_ps(mSum03, _mm_loadu_ps(s03_p2));
			mSum03 = _mm_sub_ps(mSum03, _mm_load_ps(s03_p1));
			mSum04 = _mm_add_ps(mSum04, _mm_loadu_ps(s04_p2));
			mSum04 = _mm_sub_ps(mSum04, _mm_load_ps(s04_p1));
			mSum05 = _mm_add_ps(mSum05, _mm_loadu_ps(s05_p2));
			mSum05 = _mm_sub_ps(mSum05, _mm_load_ps(s05_p1));
			mSum06 = _mm_add_ps(mSum06, _mm_loadu_ps(s06_p2));
			mSum06 = _mm_sub_ps(mSum06, _mm_load_ps(s06_p1));
			mSum07 = _mm_add_ps(mSum07, _mm_loadu_ps(s07_p2));
			mSum07 = _mm_sub_ps(mSum07, _mm_load_ps(s07_p1));
			mSum08 = _mm_add_ps(mSum08, _mm_loadu_ps(s08_p2));
			mSum08 = _mm_sub_ps(mSum08, _mm_load_ps(s08_p1));
			mSum09 = _mm_add_ps(mSum09, _mm_loadu_ps(s09_p2));
			mSum09 = _mm_sub_ps(mSum09, _mm_load_ps(s09_p1));
			mSum10 = _mm_add_ps(mSum10, _mm_loadu_ps(s10_p2));
			mSum10 = _mm_sub_ps(mSum10, _mm_load_ps(s10_p1));
			mSum11 = _mm_add_ps(mSum11, _mm_loadu_ps(s11_p2));
			mSum11 = _mm_sub_ps(mSum11, _mm_load_ps(s11_p1));
			mSum12 = _mm_add_ps(mSum12, _mm_loadu_ps(s12_p2));
			mSum12 = _mm_sub_ps(mSum12, _mm_load_ps(s12_p1));

			s00_p1 += 4;
			s01_p1 += 4;
			s02_p1 += 4;
			s03_p1 += 4;
			s04_p1 += 4;
			s05_p1 += 4;
			s06_p1 += 4;
			s07_p1 += 4;
			s08_p1 += 4;
			s09_p1 += 4;
			s10_p1 += 4;
			s11_p1 += 4;
			s12_p1 += 4;

			mTmp00 = _mm_mul_ps(mSum00, mDiv);
			mTmp01 = _mm_mul_ps(mSum01, mDiv);
			mTmp02 = _mm_mul_ps(mSum02, mDiv);
			mTmp03 = _mm_mul_ps(mSum03, mDiv);
			mTmp04 = _mm_mul_ps(mSum04, mDiv);
			mTmp05 = _mm_mul_ps(mSum05, mDiv);
			mTmp06 = _mm_mul_ps(mSum06, mDiv);
			mTmp07 = _mm_mul_ps(mSum07, mDiv);
			mTmp08 = _mm_mul_ps(mSum08, mDiv);
			mTmp09 = _mm_mul_ps(mSum09, mDiv);
			mTmp10 = _mm_mul_ps(mSum10, mDiv);
			mTmp11 = _mm_mul_ps(mSum11, mDiv);
			mTmp12 = _mm_mul_ps(mSum12, mDiv);

			mVar00 = _mm_sub_ps(mTmp04, _mm_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm_sub_ps(mTmp05, _mm_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm_sub_ps(mTmp06, _mm_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm_sub_ps(mTmp07, _mm_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm_sub_ps(mTmp08, _mm_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm_sub_ps(mTmp09, _mm_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm_sub_ps(mTmp10, _mm_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm_sub_ps(mTmp11, _mm_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm_sub_ps(mTmp12, _mm_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm_add_ps(mVar00, mEps);
			mVar03 = _mm_add_ps(mVar03, mEps);
			mVar05 = _mm_add_ps(mVar05, mEps);

			mTmp04 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar03, mVar05));
			mTmp05 = _mm_mul_ps(mVar01, _mm_mul_ps(mVar02, mVar04));
			mTmp06 = _mm_mul_ps(mVar00, _mm_mul_ps(mVar04, mVar04));
			mTmp07 = _mm_mul_ps(mVar03, _mm_mul_ps(mVar02, mVar02));
			mTmp08 = _mm_mul_ps(mVar05, _mm_mul_ps(mVar01, mVar01));

			mDet = _mm_add_ps(mTmp04, mTmp05);
			mDet = _mm_add_ps(mDet, mTmp05);
			mDet = _mm_sub_ps(mDet, mTmp06);
			mDet = _mm_sub_ps(mDet, mTmp07);
			mDet = _mm_sub_ps(mDet, mTmp08);
			mDet = _mm_div_ps(_mm_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm_sub_ps(_mm_mul_ps(mVar03, mVar05), _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar04), _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_sub_ps(_mm_mul_ps(mVar01, mVar04), _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar05), _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_sub_ps(_mm_mul_ps(mVar02, mVar01), _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_sub_ps(_mm_mul_ps(mVar00, mVar03), _mm_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm_fmsub_ps(mVar03, mVar05, _mm_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm_fmsub_ps(mVar02, mVar04, _mm_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm_fmsub_ps(mVar01, mVar04, _mm_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm_fmsub_ps(mVar00, mVar05, _mm_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm_fmsub_ps(mVar02, mVar01, _mm_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm_fmsub_ps(mVar00, mVar03, _mm_mul_ps(mVar01, mVar01)); //c8

																			   //a
																			   /*
																			   mTmp10 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp04), _mm_mul_ps(mCov01, mTmp05));
																			   mTmp10 = _mm_add_ps(mTmp10, _mm_mul_ps(mCov02, mTmp06));
																			   */
			mTmp10 = _mm_fmadd_ps(mCov00, mTmp04, _mm_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm_mul_ps(mTmp10, mDet);
			_mm_storeu_ps(a_b_p, mTmp10);
			a_b_p += 4;

			/*
			mTmp11 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp05), _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm_fmadd_ps(mCov00, mTmp05, _mm_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm_mul_ps(mTmp11, mDet);
			_mm_storeu_ps(a_g_p, mTmp11);
			a_g_p += 4;

			/*
			mTmp12 = _mm_add_ps(_mm_mul_ps(mCov00, mTmp06), _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_add_ps(mTmp12, _mm_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm_fmadd_ps(mCov00, mTmp06, _mm_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm_mul_ps(mTmp12, mDet);
			_mm_storeu_ps(a_r_p, mTmp12);
			a_r_p += 4;

			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm_sub_ps(mTmp03, _mm_mul_ps(mTmp12, mTmp02));
			_mm_storeu_ps(b_p, mTmp03);
			b_p += 4;
		}
	}
}



void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 8;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 8;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 8;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 8;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 8;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 8;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 8;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 8;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 8;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 8;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 8;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 8;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 8;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m256 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m256 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m256 mCov00, mCov01, mCov02;
		__m256 mDet;

		mSum00 = _mm256_setzero_ps();
		mSum01 = _mm256_setzero_ps();
		mSum02 = _mm256_setzero_ps();
		mSum03 = _mm256_setzero_ps();
		mSum04 = _mm256_setzero_ps();
		mSum05 = _mm256_setzero_ps();
		mSum06 = _mm256_setzero_ps();
		mSum07 = _mm256_setzero_ps();
		mSum08 = _mm256_setzero_ps();
		mSum09 = _mm256_setzero_ps();
		mSum10 = _mm256_setzero_ps();
		mSum11 = _mm256_setzero_ps();
		mSum12 = _mm256_setzero_ps();

		mSum00 = _mm256_mul_ps(mBorder, _mm256_load_ps(s00_p1));
		mSum01 = _mm256_mul_ps(mBorder, _mm256_load_ps(s01_p1));
		mSum02 = _mm256_mul_ps(mBorder, _mm256_load_ps(s02_p1));
		mSum03 = _mm256_mul_ps(mBorder, _mm256_load_ps(s03_p1));
		mSum04 = _mm256_mul_ps(mBorder, _mm256_load_ps(s04_p1));
		mSum05 = _mm256_mul_ps(mBorder, _mm256_load_ps(s05_p1));
		mSum06 = _mm256_mul_ps(mBorder, _mm256_load_ps(s06_p1));
		mSum07 = _mm256_mul_ps(mBorder, _mm256_load_ps(s07_p1));
		mSum08 = _mm256_mul_ps(mBorder, _mm256_load_ps(s08_p1));
		mSum09 = _mm256_mul_ps(mBorder, _mm256_load_ps(s09_p1));
		mSum10 = _mm256_mul_ps(mBorder, _mm256_load_ps(s10_p1));
		mSum11 = _mm256_mul_ps(mBorder, _mm256_load_ps(s11_p1));
		mSum12 = _mm256_mul_ps(mBorder, _mm256_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;
		}
		mTmp00 = _mm256_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm256_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm256_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm256_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm256_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm256_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm256_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm256_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm256_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm256_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm256_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm256_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm256_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm256_add_ps(mVar00, mEps);
		mVar03 = _mm256_add_ps(mVar03, mEps);
		mVar05 = _mm256_add_ps(mVar05, mEps);

		mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm256_add_ps(mTmp04, mTmp05);
		mDet = _mm256_add_ps(mDet, mTmp05);
		mDet = _mm256_sub_ps(mDet, mTmp06);
		mDet = _mm256_sub_ps(mDet, mTmp07);
		mDet = _mm256_sub_ps(mDet, mTmp08);
		mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																				 //
																				 /*
																				 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																				 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																				 */
		mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm256_mul_ps(mTmp10, mDet);
		//_mm256_store_ps(a_b_p, mTmp10);
		_mm256_stream_ps(a_b_p, mTmp10);
		a_b_p += 8;

		/*
		mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm256_mul_ps(mTmp11, mDet);
		//_mm256_store_ps(a_g_p, mTmp11);
		_mm256_stream_ps(a_g_p, mTmp11);
		a_g_p += 8;

		/*
		mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm256_mul_ps(mTmp12, mDet);
		//_mm256_store_ps(a_r_p, mTmp12);
		_mm256_stream_ps(a_r_p, mTmp12);
		a_r_p += 8;

		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
		//_mm256_store_ps(b_p, mTmp03);
		_mm256_stream_ps(b_p, mTmp03);
		b_p += 8;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 8;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 8;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 8;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 8;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 8;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 8;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 8;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 8;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 8;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 8;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 8;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 8;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 8;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m256 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m256 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m256 mCov00, mCov01, mCov02;
		__m256 mDet;

		mSum00 = _mm256_setzero_ps();
		mSum01 = _mm256_setzero_ps();
		mSum02 = _mm256_setzero_ps();
		mSum03 = _mm256_setzero_ps();
		mSum04 = _mm256_setzero_ps();
		mSum05 = _mm256_setzero_ps();
		mSum06 = _mm256_setzero_ps();
		mSum07 = _mm256_setzero_ps();
		mSum08 = _mm256_setzero_ps();
		mSum09 = _mm256_setzero_ps();
		mSum10 = _mm256_setzero_ps();
		mSum11 = _mm256_setzero_ps();
		mSum12 = _mm256_setzero_ps();

		mSum00 = _mm256_mul_ps(mBorder, _mm256_load_ps(s00_p1));
		mSum01 = _mm256_mul_ps(mBorder, _mm256_load_ps(s01_p1));
		mSum02 = _mm256_mul_ps(mBorder, _mm256_load_ps(s02_p1));
		mSum03 = _mm256_mul_ps(mBorder, _mm256_load_ps(s03_p1));
		mSum04 = _mm256_mul_ps(mBorder, _mm256_load_ps(s04_p1));
		mSum05 = _mm256_mul_ps(mBorder, _mm256_load_ps(s05_p1));
		mSum06 = _mm256_mul_ps(mBorder, _mm256_load_ps(s06_p1));
		mSum07 = _mm256_mul_ps(mBorder, _mm256_load_ps(s07_p1));
		mSum08 = _mm256_mul_ps(mBorder, _mm256_load_ps(s08_p1));
		mSum09 = _mm256_mul_ps(mBorder, _mm256_load_ps(s09_p1));
		mSum10 = _mm256_mul_ps(mBorder, _mm256_load_ps(s10_p1));
		mSum11 = _mm256_mul_ps(mBorder, _mm256_load_ps(s11_p1));
		mSum12 = _mm256_mul_ps(mBorder, _mm256_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;
		}
		mTmp00 = _mm256_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm256_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm256_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm256_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm256_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm256_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm256_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm256_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm256_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm256_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm256_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm256_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm256_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm256_add_ps(mVar00, mEps);
		mVar03 = _mm256_add_ps(mVar03, mEps);
		mVar05 = _mm256_add_ps(mVar05, mEps);

		mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm256_add_ps(mTmp04, mTmp05);
		mDet = _mm256_add_ps(mDet, mTmp05);
		mDet = _mm256_sub_ps(mDet, mTmp06);
		mDet = _mm256_sub_ps(mDet, mTmp07);
		mDet = _mm256_sub_ps(mDet, mTmp08);
		mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																				 //
																				 /*
																				 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																				 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																				 */
		mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm256_mul_ps(mTmp10, mDet);
		//_mm256_store_ps(a_b_p, mTmp10);
		_mm256_stream_ps(a_b_p, mTmp10);
		a_b_p += 8;

		/*
		mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm256_mul_ps(mTmp11, mDet);
		//_mm256_store_ps(a_g_p, mTmp11);
		_mm256_stream_ps(a_g_p, mTmp11);
		a_g_p += 8;

		/*
		mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm256_mul_ps(mTmp12, mDet);
		//_mm256_store_ps(a_r_p, mTmp12);
		_mm256_stream_ps(a_r_p, mTmp12);
		a_r_p += 8;

		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
		//_mm256_store_ps(b_p, mTmp03);
		_mm256_stream_ps(b_p, mTmp03);
		b_p += 8;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}
	}
}

void ColumnSumFilter_Ip2ab_Guide3_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s00_p1 = tempVec[0].ptr<float>(j);	// mean_I_b
		float* s01_p1 = tempVec[1].ptr<float>(j);	// mean_I_g
		float* s02_p1 = tempVec[2].ptr<float>(j);	// mean_I_r
		float* s03_p1 = tempVec[3].ptr<float>(j);	// mean_p
		float* s04_p1 = tempVec[4].ptr<float>(j);	// corr_I_bb
		float* s05_p1 = tempVec[5].ptr<float>(j);	// corr_I_bg
		float* s06_p1 = tempVec[6].ptr<float>(j);	// corr_I_br
		float* s07_p1 = tempVec[7].ptr<float>(j);	// corr_I_gg
		float* s08_p1 = tempVec[8].ptr<float>(j);	// corr_I_gr
		float* s09_p1 = tempVec[9].ptr<float>(j);	// corr_I_rr
		float* s10_p1 = tempVec[10].ptr<float>(j);	// cov_Ip_b
		float* s11_p1 = tempVec[11].ptr<float>(j);	// cov_Ip_g
		float* s12_p1 = tempVec[12].ptr<float>(j);	// cov_Ip_r

		float* s00_p2 = tempVec[0].ptr<float>(j) + 8;
		float* s01_p2 = tempVec[1].ptr<float>(j) + 8;
		float* s02_p2 = tempVec[2].ptr<float>(j) + 8;
		float* s03_p2 = tempVec[3].ptr<float>(j) + 8;
		float* s04_p2 = tempVec[4].ptr<float>(j) + 8;
		float* s05_p2 = tempVec[5].ptr<float>(j) + 8;
		float* s06_p2 = tempVec[6].ptr<float>(j) + 8;
		float* s07_p2 = tempVec[7].ptr<float>(j) + 8;
		float* s08_p2 = tempVec[8].ptr<float>(j) + 8;
		float* s09_p2 = tempVec[9].ptr<float>(j) + 8;
		float* s10_p2 = tempVec[10].ptr<float>(j) + 8;
		float* s11_p2 = tempVec[11].ptr<float>(j) + 8;
		float* s12_p2 = tempVec[12].ptr<float>(j) + 8;

		float* a_b_p = va[0].ptr<float>(j);
		float* a_g_p = va[1].ptr<float>(j);
		float* a_r_p = va[2].ptr<float>(j);
		float* b_p = b.ptr<float>(j);

		__m256 mSum00, mSum01, mSum02, mSum03, mSum04, mSum05, mSum06, mSum07, mSum08, mSum09, mSum10, mSum11, mSum12;
		__m256 mTmp00, mTmp01, mTmp02, mTmp03, mTmp04, mTmp05, mTmp06, mTmp07, mTmp08, mTmp09, mTmp10, mTmp11, mTmp12;
		__m256 mVar00, mVar01, mVar02, mVar03, mVar04, mVar05;
		__m256 mCov00, mCov01, mCov02;
		__m256 mDet;

		mSum00 = _mm256_setzero_ps();
		mSum01 = _mm256_setzero_ps();
		mSum02 = _mm256_setzero_ps();
		mSum03 = _mm256_setzero_ps();
		mSum04 = _mm256_setzero_ps();
		mSum05 = _mm256_setzero_ps();
		mSum06 = _mm256_setzero_ps();
		mSum07 = _mm256_setzero_ps();
		mSum08 = _mm256_setzero_ps();
		mSum09 = _mm256_setzero_ps();
		mSum10 = _mm256_setzero_ps();
		mSum11 = _mm256_setzero_ps();
		mSum12 = _mm256_setzero_ps();

		mSum00 = _mm256_mul_ps(mBorder, _mm256_load_ps(s00_p1));
		mSum01 = _mm256_mul_ps(mBorder, _mm256_load_ps(s01_p1));
		mSum02 = _mm256_mul_ps(mBorder, _mm256_load_ps(s02_p1));
		mSum03 = _mm256_mul_ps(mBorder, _mm256_load_ps(s03_p1));
		mSum04 = _mm256_mul_ps(mBorder, _mm256_load_ps(s04_p1));
		mSum05 = _mm256_mul_ps(mBorder, _mm256_load_ps(s05_p1));
		mSum06 = _mm256_mul_ps(mBorder, _mm256_load_ps(s06_p1));
		mSum07 = _mm256_mul_ps(mBorder, _mm256_load_ps(s07_p1));
		mSum08 = _mm256_mul_ps(mBorder, _mm256_load_ps(s08_p1));
		mSum09 = _mm256_mul_ps(mBorder, _mm256_load_ps(s09_p1));
		mSum10 = _mm256_mul_ps(mBorder, _mm256_load_ps(s10_p1));
		mSum11 = _mm256_mul_ps(mBorder, _mm256_load_ps(s11_p1));
		mSum12 = _mm256_mul_ps(mBorder, _mm256_load_ps(s12_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;
		}
		mTmp00 = _mm256_mul_ps(mSum00, mDiv);	// mean_I_b
		mTmp01 = _mm256_mul_ps(mSum01, mDiv);	// mean_I_g
		mTmp02 = _mm256_mul_ps(mSum02, mDiv);	// mean_I_r
		mTmp03 = _mm256_mul_ps(mSum03, mDiv);	// mean_p
		mTmp04 = _mm256_mul_ps(mSum04, mDiv);	// corr_I_bb
		mTmp05 = _mm256_mul_ps(mSum05, mDiv);	// corr_I_bg
		mTmp06 = _mm256_mul_ps(mSum06, mDiv);	// corr_I_br
		mTmp07 = _mm256_mul_ps(mSum07, mDiv);	// corr_I_gg
		mTmp08 = _mm256_mul_ps(mSum08, mDiv);	// corr_I_gr
		mTmp09 = _mm256_mul_ps(mSum09, mDiv);	// corr_I_rr
		mTmp10 = _mm256_mul_ps(mSum10, mDiv);	// cov_Ip_b
		mTmp11 = _mm256_mul_ps(mSum11, mDiv);	// cov_Ip_g
		mTmp12 = _mm256_mul_ps(mSum12, mDiv);	// cov_Ip_r

		mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
		mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
		mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
		mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
		mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
		mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
		mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
		mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
		mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

		mVar00 = _mm256_add_ps(mVar00, mEps);
		mVar03 = _mm256_add_ps(mVar03, mEps);
		mVar05 = _mm256_add_ps(mVar05, mEps);

		mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));	// bb*gg*rr
		mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));	// bg*br*gr
		mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));	// bb*gr*gr
		mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));	// gg*br*br
		mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));	// rr*bg*bg

		mDet = _mm256_add_ps(mTmp04, mTmp05);
		mDet = _mm256_add_ps(mDet, mTmp05);
		mDet = _mm256_sub_ps(mDet, mTmp06);
		mDet = _mm256_sub_ps(mDet, mTmp07);
		mDet = _mm256_sub_ps(mDet, mTmp08);
		mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

		/*
		mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
		*/
		mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
		mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
		mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
		mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
		mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
		mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																				 //
																				 /*
																				 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																				 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																				 */
		mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
		mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
		mTmp10 = _mm256_mul_ps(mTmp10, mDet);
		//_mm256_store_ps(a_b_p, mTmp10);
		_mm256_stream_ps(a_b_p, mTmp10);
		a_b_p += 8;

		/*
		mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp08));
		mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
		*/
		mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
		mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
		mTmp11 = _mm256_mul_ps(mTmp11, mDet);
		//_mm256_store_ps(a_g_p, mTmp11);
		_mm256_stream_ps(a_g_p, mTmp11);
		a_g_p += 8;

		/*
		mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp07));
		mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
		*/
		mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
		mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
		mTmp12 = _mm256_mul_ps(mTmp12, mDet);
		//_mm256_store_ps(a_r_p, mTmp12);
		_mm256_stream_ps(a_r_p, mTmp12);
		a_r_p += 8;

		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
		mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
		//_mm256_store_ps(b_p, mTmp03);
		_mm256_stream_ps(b_p, mTmp03);
		b_p += 8;

		for (int i = 1; i <= r; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;
			s00_p2 += 8;
			s01_p2 += 8;
			s02_p2 += 8;
			s03_p2 += 8;
			s04_p2 += 8;
			s05_p2 += 8;
			s06_p2 += 8;
			s07_p2 += 8;
			s08_p2 += 8;
			s09_p2 += 8;
			s10_p2 += 8;
			s11_p2 += 8;
			s12_p2 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}

		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum00 = _mm256_add_ps(mSum00, _mm256_loadu_ps(s00_p2));
			mSum00 = _mm256_sub_ps(mSum00, _mm256_load_ps(s00_p1));
			mSum01 = _mm256_add_ps(mSum01, _mm256_loadu_ps(s01_p2));
			mSum01 = _mm256_sub_ps(mSum01, _mm256_load_ps(s01_p1));
			mSum02 = _mm256_add_ps(mSum02, _mm256_loadu_ps(s02_p2));
			mSum02 = _mm256_sub_ps(mSum02, _mm256_load_ps(s02_p1));
			mSum03 = _mm256_add_ps(mSum03, _mm256_loadu_ps(s03_p2));
			mSum03 = _mm256_sub_ps(mSum03, _mm256_load_ps(s03_p1));
			mSum04 = _mm256_add_ps(mSum04, _mm256_loadu_ps(s04_p2));
			mSum04 = _mm256_sub_ps(mSum04, _mm256_load_ps(s04_p1));
			mSum05 = _mm256_add_ps(mSum05, _mm256_loadu_ps(s05_p2));
			mSum05 = _mm256_sub_ps(mSum05, _mm256_load_ps(s05_p1));
			mSum06 = _mm256_add_ps(mSum06, _mm256_loadu_ps(s06_p2));
			mSum06 = _mm256_sub_ps(mSum06, _mm256_load_ps(s06_p1));
			mSum07 = _mm256_add_ps(mSum07, _mm256_loadu_ps(s07_p2));
			mSum07 = _mm256_sub_ps(mSum07, _mm256_load_ps(s07_p1));
			mSum08 = _mm256_add_ps(mSum08, _mm256_loadu_ps(s08_p2));
			mSum08 = _mm256_sub_ps(mSum08, _mm256_load_ps(s08_p1));
			mSum09 = _mm256_add_ps(mSum09, _mm256_loadu_ps(s09_p2));
			mSum09 = _mm256_sub_ps(mSum09, _mm256_load_ps(s09_p1));
			mSum10 = _mm256_add_ps(mSum10, _mm256_loadu_ps(s10_p2));
			mSum10 = _mm256_sub_ps(mSum10, _mm256_load_ps(s10_p1));
			mSum11 = _mm256_add_ps(mSum11, _mm256_loadu_ps(s11_p2));
			mSum11 = _mm256_sub_ps(mSum11, _mm256_load_ps(s11_p1));
			mSum12 = _mm256_add_ps(mSum12, _mm256_loadu_ps(s12_p2));
			mSum12 = _mm256_sub_ps(mSum12, _mm256_load_ps(s12_p1));

			s00_p1 += 8;
			s01_p1 += 8;
			s02_p1 += 8;
			s03_p1 += 8;
			s04_p1 += 8;
			s05_p1 += 8;
			s06_p1 += 8;
			s07_p1 += 8;
			s08_p1 += 8;
			s09_p1 += 8;
			s10_p1 += 8;
			s11_p1 += 8;
			s12_p1 += 8;

			mTmp00 = _mm256_mul_ps(mSum00, mDiv);
			mTmp01 = _mm256_mul_ps(mSum01, mDiv);
			mTmp02 = _mm256_mul_ps(mSum02, mDiv);
			mTmp03 = _mm256_mul_ps(mSum03, mDiv);
			mTmp04 = _mm256_mul_ps(mSum04, mDiv);
			mTmp05 = _mm256_mul_ps(mSum05, mDiv);
			mTmp06 = _mm256_mul_ps(mSum06, mDiv);
			mTmp07 = _mm256_mul_ps(mSum07, mDiv);
			mTmp08 = _mm256_mul_ps(mSum08, mDiv);
			mTmp09 = _mm256_mul_ps(mSum09, mDiv);
			mTmp10 = _mm256_mul_ps(mSum10, mDiv);
			mTmp11 = _mm256_mul_ps(mSum11, mDiv);
			mTmp12 = _mm256_mul_ps(mSum12, mDiv);

			mVar00 = _mm256_sub_ps(mTmp04, _mm256_mul_ps(mTmp00, mTmp00));
			mVar01 = _mm256_sub_ps(mTmp05, _mm256_mul_ps(mTmp00, mTmp01));
			mVar02 = _mm256_sub_ps(mTmp06, _mm256_mul_ps(mTmp00, mTmp02));
			mVar03 = _mm256_sub_ps(mTmp07, _mm256_mul_ps(mTmp01, mTmp01));
			mVar04 = _mm256_sub_ps(mTmp08, _mm256_mul_ps(mTmp01, mTmp02));
			mVar05 = _mm256_sub_ps(mTmp09, _mm256_mul_ps(mTmp02, mTmp02));
			mCov00 = _mm256_sub_ps(mTmp10, _mm256_mul_ps(mTmp00, mTmp03));
			mCov01 = _mm256_sub_ps(mTmp11, _mm256_mul_ps(mTmp01, mTmp03));
			mCov02 = _mm256_sub_ps(mTmp12, _mm256_mul_ps(mTmp02, mTmp03));

			mVar00 = _mm256_add_ps(mVar00, mEps);
			mVar03 = _mm256_add_ps(mVar03, mEps);
			mVar05 = _mm256_add_ps(mVar05, mEps);

			mTmp04 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar03, mVar05));
			mTmp05 = _mm256_mul_ps(mVar01, _mm256_mul_ps(mVar02, mVar04));
			mTmp06 = _mm256_mul_ps(mVar00, _mm256_mul_ps(mVar04, mVar04));
			mTmp07 = _mm256_mul_ps(mVar03, _mm256_mul_ps(mVar02, mVar02));
			mTmp08 = _mm256_mul_ps(mVar05, _mm256_mul_ps(mVar01, mVar01));

			mDet = _mm256_add_ps(mTmp04, mTmp05);
			mDet = _mm256_add_ps(mDet, mTmp05);
			mDet = _mm256_sub_ps(mDet, mTmp06);
			mDet = _mm256_sub_ps(mDet, mTmp07);
			mDet = _mm256_sub_ps(mDet, mTmp08);
			mDet = _mm256_div_ps(_mm256_set1_ps(1.f), mDet);

			/*
			mTmp04 = _mm256_sub_ps(_mm256_mul_ps(mVar03, mVar05), _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar04), _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_sub_ps(_mm256_mul_ps(mVar01, mVar04), _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar05), _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_sub_ps(_mm256_mul_ps(mVar02, mVar01), _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_sub_ps(_mm256_mul_ps(mVar00, mVar03), _mm256_mul_ps(mVar01, mVar01)); //c8
			*/
			mTmp04 = _mm256_fmsub_ps(mVar03, mVar05, _mm256_mul_ps(mVar04, mVar04)); //c0
			mTmp05 = _mm256_fmsub_ps(mVar02, mVar04, _mm256_mul_ps(mVar01, mVar05)); //c1
			mTmp06 = _mm256_fmsub_ps(mVar01, mVar04, _mm256_mul_ps(mVar02, mVar03)); //c2
			mTmp07 = _mm256_fmsub_ps(mVar00, mVar05, _mm256_mul_ps(mVar02, mVar02)); //c4
			mTmp08 = _mm256_fmsub_ps(mVar02, mVar01, _mm256_mul_ps(mVar00, mVar04)); //c5
			mTmp09 = _mm256_fmsub_ps(mVar00, mVar03, _mm256_mul_ps(mVar01, mVar01)); //c8

																					 //a
																					 /*
																					 mTmp10 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp04), _mm256_mul_ps(mCov01, mTmp05));
																					 mTmp10 = _mm256_add_ps(mTmp10, _mm256_mul_ps(mCov02, mTmp06));
																					 */
			mTmp10 = _mm256_fmadd_ps(mCov00, mTmp04, _mm256_mul_ps(mCov01, mTmp05));
			mTmp10 = _mm256_fmadd_ps(mCov02, mTmp06, mTmp10);
			mTmp10 = _mm256_mul_ps(mTmp10, mDet);
			_mm256_storeu_ps(a_b_p, mTmp10);
			a_b_p += 8;

			/*
			mTmp11 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp05), _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_add_ps(mTmp11, _mm256_mul_ps(mCov02, mTmp08));
			*/
			mTmp11 = _mm256_fmadd_ps(mCov00, mTmp05, _mm256_mul_ps(mCov01, mTmp07));
			mTmp11 = _mm256_fmadd_ps(mCov02, mTmp08, mTmp11);
			mTmp11 = _mm256_mul_ps(mTmp11, mDet);
			_mm256_storeu_ps(a_g_p, mTmp11);
			a_g_p += 8;

			/*
			mTmp12 = _mm256_add_ps(_mm256_mul_ps(mCov00, mTmp06), _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_add_ps(mTmp12, _mm256_mul_ps(mCov02, mTmp09));
			*/
			mTmp12 = _mm256_fmadd_ps(mCov00, mTmp06, _mm256_mul_ps(mCov01, mTmp08));
			mTmp12 = _mm256_fmadd_ps(mCov02, mTmp09, mTmp12);
			mTmp12 = _mm256_mul_ps(mTmp12, mDet);
			_mm256_storeu_ps(a_r_p, mTmp12);
			a_r_p += 8;

			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp10, mTmp00));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp11, mTmp01));
			mTmp03 = _mm256_sub_ps(mTmp03, _mm256_mul_ps(mTmp12, mTmp02));
			_mm256_storeu_ps(b_p, mTmp03);
			b_p += 8;
		}
	}
}



void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_nonVec::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 1;
		float* v1_p2 = va[1].ptr<float>(j) + 1;
		float* v2_p2 = va[2].ptr<float>(j) + 1;
		float* v3_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a_B
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_a_G
		float* d2_p = tempVec[2].ptr<float>(0) + j; // mean_a_R
		float* d3_p = tempVec[3].ptr<float>(0) + j; // mean_b

		float sum_ab = 0.f, sum_ag = 0.f, sum_ar = 0.f, sum_b = 0.f;

		sum_ab = *v0_p1 * (r + 1);
		sum_ag = *v1_p1 * (r + 1);
		sum_ar = *v2_p1 * (r + 1);
		sum_b = *v3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2;
			v0_p2++;
			sum_ag += *v1_p2;
			v1_p2++;
			sum_ar += *v2_p2;
			v2_p2++;
			sum_b += *v3_p2;
			v3_p2++;
		}
		*d0_p = sum_ab;
		d0_p += step;
		*d1_p = sum_ag;
		d1_p += step;
		*d2_p = sum_ar;
		d2_p += step;
		*d3_p = sum_b;
		d3_p += step;

		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_nonVec::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 1;
		float* v1_p2 = va[1].ptr<float>(j) + 1;
		float* v2_p2 = va[2].ptr<float>(j) + 1;
		float* v3_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a_B
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_a_G
		float* d2_p = tempVec[2].ptr<float>(0) + j; // mean_a_R
		float* d3_p = tempVec[3].ptr<float>(0) + j; // mean_b

		float sum_ab = 0.f, sum_ag = 0.f, sum_ar = 0.f, sum_b = 0.f;

		sum_ab = *v0_p1 * (r + 1);
		sum_ag = *v1_p1 * (r + 1);
		sum_ar = *v2_p1 * (r + 1);
		sum_b = *v3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2;
			v0_p2++;
			sum_ag += *v1_p2;
			v1_p2++;
			sum_ar += *v2_p2;
			v2_p2++;
			sum_b += *v3_p2;
			v3_p2++;
		}
		*d0_p = sum_ab;
		d0_p += step;
		*d1_p = sum_ag;
		d1_p += step;
		*d2_p = sum_ar;
		d2_p += step;
		*d3_p = sum_b;
		d3_p += step;

		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_nonVec::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 1;
		float* v1_p2 = va[1].ptr<float>(j) + 1;
		float* v2_p2 = va[2].ptr<float>(j) + 1;
		float* v3_p2 = b.ptr<float>(j) + 1;

		float* d0_p = tempVec[0].ptr<float>(0) + j; // mean_a_B
		float* d1_p = tempVec[1].ptr<float>(0) + j; // mean_a_G
		float* d2_p = tempVec[2].ptr<float>(0) + j; // mean_a_R
		float* d3_p = tempVec[3].ptr<float>(0) + j; // mean_b

		float sum_ab = 0.f, sum_ag = 0.f, sum_ar = 0.f, sum_b = 0.f;

		sum_ab = *v0_p1 * (r + 1);
		sum_ag = *v1_p1 * (r + 1);
		sum_ar = *v2_p1 * (r + 1);
		sum_b = *v3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2;
			v0_p2++;
			sum_ag += *v1_p2;
			v1_p2++;
			sum_ar += *v2_p2;
			v2_p2++;
			sum_b += *v3_p2;
			v3_p2++;
		}
		*d0_p = sum_ab;
		d0_p += step;
		*d1_p = sum_ag;
		d1_p += step;
		*d2_p = sum_ar;
		d2_p += step;
		*d3_p = sum_b;
		d3_p += step;

		for (int j = 1; j <= r; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = r + 1; j < img_row - r - 1; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			v0_p2++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			v1_p2++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			v2_p2++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;
			v3_p2++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
		for (int j = img_row - r - 1; j < img_row; j++)
		{
			sum_ab += *v0_p2 - *v0_p1;
			v0_p1++;
			sum_ag += *v1_p2 - *v1_p1;
			v1_p1++;
			sum_ar += *v2_p2 - *v2_p1;
			v2_p1++;
			sum_b += *v3_p2 - *v3_p1;
			v3_p1++;

			*d0_p = sum_ab;
			d0_p += step;
			*d1_p = sum_ag;
			d1_p += step;
			*d2_p = sum_ar;
			d2_p += step;
			*d3_p = sum_b;
			d3_p += step;
		}
	}
}



void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_SSE::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 4;
		float* v1_p2 = va[1].ptr<float>(j) + 4;
		float* v2_p2 = va[2].ptr<float>(j) + 4;
		float* v3_p2 = b.ptr<float>(j) + 4;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 4; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 4; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 4; //mean_b

		__m128 mSum_ab = _mm_setzero_ps();
		__m128 mSum_ag = _mm_setzero_ps();
		__m128 mSum_ar = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_ab = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_ag = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum_ar = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_SSE::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 4;
		float* v1_p2 = va[1].ptr<float>(j) + 4;
		float* v2_p2 = va[2].ptr<float>(j) + 4;
		float* v3_p2 = b.ptr<float>(j) + 4;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 4; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 4; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 4; //mean_b

		__m128 mSum_ab = _mm_setzero_ps();
		__m128 mSum_ag = _mm_setzero_ps();
		__m128 mSum_ar = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_ab = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_ag = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum_ar = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_SSE::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 4;
		float* v1_p2 = va[1].ptr<float>(j) + 4;
		float* v2_p2 = va[2].ptr<float>(j) + 4;
		float* v3_p2 = b.ptr<float>(j) + 4;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 4; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 4; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 4; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 4; //mean_b

		__m128 mSum_ab = _mm_setzero_ps();
		__m128 mSum_ag = _mm_setzero_ps();
		__m128 mSum_ar = _mm_setzero_ps();
		__m128 mSum_b = _mm_setzero_ps();

		mSum_ab = _mm_mul_ps(mBorder, _mm_load_ps(v0_p1));
		mSum_ag = _mm_mul_ps(mBorder, _mm_load_ps(v1_p1));
		mSum_ar = _mm_mul_ps(mBorder, _mm_load_ps(v2_p1));
		mSum_b = _mm_mul_ps(mBorder, _mm_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
		}
		_mm_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 4 - r - 1; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			v0_p2 += 4;
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			v1_p2 += 4;
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			v2_p2 += 4;
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			v3_p2 += 4;
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 4 - r - 1; i < img_row / 4; i++)
		{
			mSum_ab = _mm_add_ps(mSum_ab, _mm_load_ps(v0_p2));
			mSum_ab = _mm_sub_ps(mSum_ab, _mm_load_ps(v0_p1));
			v0_p1 += 4;
			mSum_ag = _mm_add_ps(mSum_ag, _mm_load_ps(v1_p2));
			mSum_ag = _mm_sub_ps(mSum_ag, _mm_load_ps(v1_p1));
			v1_p1 += 4;
			mSum_ar = _mm_add_ps(mSum_ar, _mm_load_ps(v2_p2));
			mSum_ar = _mm_sub_ps(mSum_ar, _mm_load_ps(v2_p1));
			v2_p1 += 4;
			mSum_b = _mm_add_ps(mSum_b, _mm_load_ps(v3_p2));
			mSum_b = _mm_sub_ps(mSum_b, _mm_load_ps(v3_p1));
			v3_p1 += 4;

			_mm_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}



void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_AVX::filter_naive_impl()
{
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 8;
		float* v1_p2 = va[1].ptr<float>(j) + 8;
		float* v2_p2 = va[2].ptr<float>(j) + 8;
		float* v3_p2 = b.ptr<float>(j) + 8;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 8; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 8; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 8; //mean_b

		__m256 mSum_ab = _mm256_setzero_ps();
		__m256 mSum_ag = _mm256_setzero_ps();
		__m256 mSum_ar = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_ab = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_ag = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum_ar = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm256_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm256_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_AVX::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_col; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 8;
		float* v1_p2 = va[1].ptr<float>(j) + 8;
		float* v2_p2 = va[2].ptr<float>(j) + 8;
		float* v3_p2 = b.ptr<float>(j) + 8;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 8; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 8; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 8; //mean_b

		__m256 mSum_ab = _mm256_setzero_ps();
		__m256 mSum_ag = _mm256_setzero_ps();
		__m256 mSum_ar = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_ab = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_ag = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum_ar = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm256_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm256_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}

void ColumnSumFilter_ab2q_Guide3_Transpose_Inverse_AVX::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* v0_p1 = va[0].ptr<float>(j);
		float* v1_p1 = va[1].ptr<float>(j);
		float* v2_p1 = va[2].ptr<float>(j);
		float* v3_p1 = b.ptr<float>(j);
		float* v0_p2 = va[0].ptr<float>(j) + 8;
		float* v1_p2 = va[1].ptr<float>(j) + 8;
		float* v2_p2 = va[2].ptr<float>(j) + 8;
		float* v3_p2 = b.ptr<float>(j) + 8;


		float* d0_p = tempVec[0].ptr<float>(0) + j * 8; //mean_ab
		float* d1_p = tempVec[1].ptr<float>(0) + j * 8; //mean_ag
		float* d2_p = tempVec[2].ptr<float>(0) + j * 8; //mean_ar
		float* d3_p = tempVec[3].ptr<float>(0) + j * 8; //mean_b

		__m256 mSum_ab = _mm256_setzero_ps();
		__m256 mSum_ag = _mm256_setzero_ps();
		__m256 mSum_ar = _mm256_setzero_ps();
		__m256 mSum_b = _mm256_setzero_ps();

		mSum_ab = _mm256_mul_ps(mBorder, _mm256_load_ps(v0_p1));
		mSum_ag = _mm256_mul_ps(mBorder, _mm256_load_ps(v1_p1));
		mSum_ar = _mm256_mul_ps(mBorder, _mm256_load_ps(v2_p1));
		mSum_b = _mm256_mul_ps(mBorder, _mm256_load_ps(v3_p1));
		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
		}
		_mm256_store_ps(d0_p, mSum_ab);
		d0_p += step;
		_mm256_store_ps(d1_p, mSum_ag);
		d1_p += step;
		_mm256_store_ps(d2_p, mSum_ar);
		d2_p += step;
		_mm256_store_ps(d3_p, mSum_b);
		d3_p += step;

		for (int i = 1; i <= r; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = r + 1; i < img_row / 8 - r - 1; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			v0_p2 += 8;
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			v1_p2 += 8;
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			v2_p2 += 8;
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			v3_p2 += 8;
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
		for (int i = img_row / 8 - r - 1; i < img_row / 8; i++)
		{
			mSum_ab = _mm256_add_ps(mSum_ab, _mm256_load_ps(v0_p2));
			mSum_ab = _mm256_sub_ps(mSum_ab, _mm256_load_ps(v0_p1));
			v0_p1 += 8;
			mSum_ag = _mm256_add_ps(mSum_ag, _mm256_load_ps(v1_p2));
			mSum_ag = _mm256_sub_ps(mSum_ag, _mm256_load_ps(v1_p1));
			v1_p1 += 8;
			mSum_ar = _mm256_add_ps(mSum_ar, _mm256_load_ps(v2_p2));
			mSum_ar = _mm256_sub_ps(mSum_ar, _mm256_load_ps(v2_p1));
			v2_p1 += 8;
			mSum_b = _mm256_add_ps(mSum_b, _mm256_load_ps(v3_p2));
			mSum_b = _mm256_sub_ps(mSum_b, _mm256_load_ps(v3_p1));
			v3_p1 += 8;

			_mm256_store_ps(d0_p, mSum_ab);
			d0_p += step;
			_mm256_store_ps(d1_p, mSum_ag);
			d1_p += step;
			_mm256_store_ps(d2_p, mSum_ar);
			d2_p += step;
			_mm256_store_ps(d3_p, mSum_b);
			d3_p += step;
		}
	}
}



void RowSumFilter_ab2q_Guide3_Transpose_Inverse::filter_naive_impl()
{
	for (int j = 0; j < img_row; j++)
	{
		float* s0_p1 = tempVec[0].ptr<float>(j);
		float* s1_p1 = tempVec[1].ptr<float>(j);
		float* s2_p1 = tempVec[2].ptr<float>(j);
		float* s3_p1 = tempVec[3].ptr<float>(j);
		float* s0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* I_b_p = vI[0].ptr<float>(j);
		float* I_g_p = vI[1].ptr<float>(j);
		float* I_r_p = vI[2].ptr<float>(j);

		float* q_p = q.ptr<float>(j);

		float sum0, sum1, sum2, sum3;
		float tmp0, tmp1, tmp2, tmp3;
		sum0 = sum1 = sum2 = sum3 = 0.f;

		sum0 += *s0_p1 * (r + 1);
		sum1 += *s1_p1 * (r + 1);
		sum2 += *s2_p1 * (r + 1);
		sum3 += *s3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2;
			s0_p2++;
			sum1 += *s1_p2;
			s1_p2++;
			sum2 += *s2_p2;
			s2_p2++;
			sum3 += *s3_p2;
			s3_p2++;
		}
		tmp0 = sum0 * div;
		tmp1 = sum1 * div;
		tmp2 = sum2 * div;
		tmp3 = sum3 * div;
		*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;
		I_b_p++;
		I_g_p++;
		I_r_p++;
		q_p++;

		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = r + 1; j < img_col - r - 1; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = img_col - r - 1; j < img_col; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
	}
}

void RowSumFilter_ab2q_Guide3_Transpose_Inverse::filter_omp_impl()
{
#pragma omp parallel for
	for (int j = 0; j < img_row; j++)
	{
		float* s0_p1 = tempVec[0].ptr<float>(j);
		float* s1_p1 = tempVec[1].ptr<float>(j);
		float* s2_p1 = tempVec[2].ptr<float>(j);
		float* s3_p1 = tempVec[3].ptr<float>(j);
		float* s0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* I_b_p = vI[0].ptr<float>(j);
		float* I_g_p = vI[1].ptr<float>(j);
		float* I_r_p = vI[2].ptr<float>(j);

		float* q_p = q.ptr<float>(j);

		float sum0, sum1, sum2, sum3;
		float tmp0, tmp1, tmp2, tmp3;
		sum0 = sum1 = sum2 = sum3 = 0.f;

		sum0 += *s0_p1 * (r + 1);
		sum1 += *s1_p1 * (r + 1);
		sum2 += *s2_p1 * (r + 1);
		sum3 += *s3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2;
			s0_p2++;
			sum1 += *s1_p2;
			s1_p2++;
			sum2 += *s2_p2;
			s2_p2++;
			sum3 += *s3_p2;
			s3_p2++;
		}
		tmp0 = sum0 * div;
		tmp1 = sum1 * div;
		tmp2 = sum2 * div;
		tmp3 = sum3 * div;
		*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;
		I_b_p++;
		I_g_p++;
		I_r_p++;
		q_p++;

		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = r + 1; j < img_col - r - 1; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = img_col - r - 1; j < img_col; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
	}
}

void RowSumFilter_ab2q_Guide3_Transpose_Inverse::operator()(const cv::Range& range) const
{
	for (int j = range.start; j < range.end; j++)
	{
		float* s0_p1 = tempVec[0].ptr<float>(j);
		float* s1_p1 = tempVec[1].ptr<float>(j);
		float* s2_p1 = tempVec[2].ptr<float>(j);
		float* s3_p1 = tempVec[3].ptr<float>(j);
		float* s0_p2 = tempVec[0].ptr<float>(j) + 1;
		float* s1_p2 = tempVec[1].ptr<float>(j) + 1;
		float* s2_p2 = tempVec[2].ptr<float>(j) + 1;
		float* s3_p2 = tempVec[3].ptr<float>(j) + 1;

		float* I_b_p = vI[0].ptr<float>(j);
		float* I_g_p = vI[1].ptr<float>(j);
		float* I_r_p = vI[2].ptr<float>(j);

		float* q_p = q.ptr<float>(j);

		float sum0, sum1, sum2, sum3;
		float tmp0, tmp1, tmp2, tmp3;
		sum0 = sum1 = sum2 = sum3 = 0.f;

		sum0 += *s0_p1 * (r + 1);
		sum1 += *s1_p1 * (r + 1);
		sum2 += *s2_p1 * (r + 1);
		sum3 += *s3_p1 * (r + 1);
		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2;
			s0_p2++;
			sum1 += *s1_p2;
			s1_p2++;
			sum2 += *s2_p2;
			s2_p2++;
			sum3 += *s3_p2;
			s3_p2++;
		}
		tmp0 = sum0 * div;
		tmp1 = sum1 * div;
		tmp2 = sum2 * div;
		tmp3 = sum3 * div;
		*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;
		I_b_p++;
		I_g_p++;
		I_r_p++;
		q_p++;

		for (int j = 1; j <= r; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = r + 1; j < img_col - r - 1; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			s0_p2++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			s1_p2++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			s2_p2++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;
			s3_p2++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
		for (int j = img_col - r - 1; j < img_col; j++)
		{
			sum0 += *s0_p2 - *s0_p1;
			s0_p1++;
			sum1 += *s1_p2 - *s1_p1;
			s1_p1++;
			sum2 += *s2_p2 - *s2_p1;
			s2_p1++;
			sum3 += *s3_p2 - *s3_p1;
			s3_p1++;

			tmp0 = sum0 * div;
			tmp1 = sum1 * div;
			tmp2 = sum2 * div;
			tmp3 = sum3 * div;
			*q_p = (tmp0 * *I_b_p + tmp1 * *I_g_p + tmp2 * *I_r_p) + tmp3;

			I_b_p++;
			I_g_p++;
			I_r_p++;
			q_p++;
		}
	}
}

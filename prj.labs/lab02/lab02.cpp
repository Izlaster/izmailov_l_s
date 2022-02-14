#include <opencv2/opencv.hpp>

void getHistNum(cv::Mat img, int* mas) {
	for (int i = 0; i < 256; i++) {
		mas[i] = 0;
	}
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			mas[img.at<uchar>(i, j)]++;
		}
	}
}

int main() {
	cv::Mat imgPng = cv::imread("../data/cross_0256x0256.png");

	std::vector<int> compression_params;
	compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
	compression_params.push_back(25);
	cv::imwrite("cross_0256x0256_025.jpg", imgPng, compression_params);

	cv::Mat channelsPNG[3];
	cv::split(imgPng, channelsPNG);
	
	cv::Mat zeroChannel(256, 256, CV_8UC1);
	zeroChannel = 0;

	cv::Mat mergeImg_png;
	cv::merge(channelsPNG, 3, mergeImg_png);

	cv::Mat m1, m2, mazaika_png;

	cv::Mat monoB_png[3] = { channelsPNG[0], zeroChannel, zeroChannel };
	cv::Mat monoG_png[3] = { zeroChannel, channelsPNG[1], zeroChannel };
	cv::Mat monoR_png[3] = { zeroChannel, zeroChannel, channelsPNG[2] };

	cv::Mat mergeImgB_png, mergeImgG_png, mergeImgR_png;
	cv::merge(monoB_png, 3, mergeImgB_png);
	cv::merge(monoG_png, 3, mergeImgG_png);
	cv::merge(monoR_png, 3, mergeImgR_png);

	cv::hconcat(mergeImg_png, mergeImgR_png, m1);
	cv::hconcat(mergeImgG_png, mergeImgB_png, m2);
	cv::vconcat(m1, m2, mazaika_png);
	cv::imwrite("cross_0256x0256_png_channels.png", mazaika_png);

	compression_params.pop_back();
	compression_params.push_back(100);
	cv::imwrite("cross_0256x0256_100.jpg", imgPng, compression_params);

	cv::Mat imgJpg = cv::imread("cross_0256x0256_100.jpg");
	cv::Mat channelsJPG[3];
	cv::split(imgJpg, channelsJPG);

	cv::Mat mergeImg_jpg;
	cv::merge(channelsJPG, 3, mergeImg_jpg);

	cv::Mat mazaika_jpg;

	cv::Mat monoB_jpg[3] = { channelsJPG[0], zeroChannel, zeroChannel };
	cv::Mat monoG_jpg[3] = { zeroChannel, channelsJPG[1], zeroChannel };
	cv::Mat monoR_jpg[3] = { zeroChannel, zeroChannel, channelsJPG[2] };

	cv::Mat mergeImgB_jpg, mergeImgG_jpg, mergeImgR_jpg;
	cv::merge(monoB_png, 3, mergeImgB_jpg);
	cv::merge(monoG_png, 3, mergeImgG_jpg);
	cv::merge(monoR_png, 3, mergeImgR_jpg);

	cv::hconcat(mergeImg_jpg, mergeImgR_jpg, m1);
	cv::hconcat(mergeImgG_jpg, mergeImgB_jpg, m2);
	cv::vconcat(m1, m2, mazaika_jpg);
	cv::imwrite("cross_0256x0256_jpg_channels.png", mazaika_jpg);

	int bin_w = 1;

	cv::Mat histPNG(256, 256, CV_8UC3, cv::Scalar(255, 255, 255));

	int countBp[256];
	int countGp[256];
	int countRp[256];

	getHistNum(channelsPNG[0], countBp);
	getHistNum(channelsPNG[1], countGp);
	getHistNum(channelsPNG[2], countRp);

	int max = countBp[0];
	for (int i = 1; i < 256; i++) {
		if (max < countBp[i]) {
			max = countBp[i];
		}
	}
	for (int i = 1; i < 256; i++) {
		if (max < countGp[i]) {
			max = countGp[i];
		}
	}
	for (int i = 1; i < 256; i++) {
		if (max < countRp[i]) {
			max = countRp[i];
		}
	}

	for (int i = 0; i < 256; i++) {
		countBp[i] = ((double)countBp[i] / max) * histPNG.rows;
	}
	for (int i = 0; i < 256; i++) {
		countGp[i] = ((double)countGp[i] / max) * histPNG.rows;
	}
	for (int i = 0; i < 256; i++) {
		countRp[i] = ((double)countRp[i] / max) * histPNG.rows;
	}

	for (int i = 0; i < 255; i++)
	{
		cv::line(histPNG, cv::Point(bin_w * (i), histPNG.rows - countBp[i]),
			cv::Point(bin_w * (i + 1), histPNG.rows - countBp[i + 1]),
			cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	for (int i = 0; i < 255; i++)
	{
		cv::line(histPNG, cv::Point(bin_w * (i), histPNG.rows - countGp[i]),
			cv::Point(bin_w * (i + 1), histPNG.rows - countGp[i + 1]),
			cv::Scalar(0, 255, 0), 1, 8, 0);
	}
	for (int i = 0; i < 255; i++)
	{
		cv::line(histPNG, cv::Point(bin_w * (i), histPNG.rows - countRp[i]),
			cv::Point(bin_w * (i + 1), histPNG.rows - countRp[i + 1]),
			cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	cv::Mat histJPG(256, 256, CV_8UC3, cv::Scalar(255, 255, 255));

	getHistNum(channelsJPG[0], countBp);
	getHistNum(channelsJPG[1], countGp);
	getHistNum(channelsJPG[2], countRp);

	max = countBp[0];
	for (int i = 1; i < 256; i++) {
		if (max < countBp[i]) {
			max = countBp[i];
		}
	}
	for (int i = 1; i < 256; i++) {
		if (max < countGp[i]) {
			max = countGp[i];
		}
	}
	for (int i = 1; i < 256; i++) {
		if (max < countRp[i]) {
			max = countRp[i];
		}
	}

	for (int i = 0; i < 256; i++) {
		countBp[i] = ((double)countBp[i] / max) * histJPG.rows;
	}
	for (int i = 0; i < 256; i++) {
		countGp[i] = ((double)countGp[i] / max) * histJPG.rows;
	}
	for (int i = 0; i < 256; i++) {
		countRp[i] = ((double)countRp[i] / max) * histJPG.rows;
	}

	for (int i = 0; i < 255; i++)
	{
		cv::line(histJPG, cv::Point(bin_w * (i), histJPG.rows - countBp[i]),
			cv::Point(bin_w * (i + 1), histJPG.rows - countBp[i + 1]),
			cv::Scalar(255, 0, 0), 1, 8, 0);
	}
	for (int i = 0; i < 255; i++)
	{
		cv::line(histJPG, cv::Point(bin_w * (i), histJPG.rows - countGp[i]),
			cv::Point(bin_w * (i + 1), histJPG.rows - countGp[i + 1]),
			cv::Scalar(0, 255, 0), 1, 8, 0);
	}
	for (int i = 0; i < 255; i++)
	{
		cv::line(histJPG, cv::Point(bin_w * (i), histJPG.rows - countRp[i]),
			cv::Point(bin_w * (i + 1), histJPG.rows - countRp[i + 1]),
			cv::Scalar(0, 0, 255), 1, 8, 0);
	}

	cv::vconcat(imgPng, histPNG, m1);
	cv::vconcat(imgJpg, histJPG, m2);
	cv::hconcat(m1, m2, mazaika_png);
	cv::imwrite("cross_0256x0256_hists.png", mazaika_png);
	cv::imshow("cross_0256x0256_hists.png", mazaika_png);

	cv::waitKey(0);
}
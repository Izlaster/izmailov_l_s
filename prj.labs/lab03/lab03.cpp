#include <opencv2/opencv.hpp>

void newFunc(cv::Mat img) {
	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			img.at<uint8_t>(i, j) = std::pow(img.at<uint8_t>(i, j), 3) / (255 * 255);
}

void newFuncLUT(int* mas) {
	for (int i = 0; i < 256; i++)
		mas[i] = std::pow(i, 3) / (255 * 255);
}

int main() {
	cv::Mat imgPng = cv::imread("../../../data/cross_0256x0256.png");
	cv::Mat imgPngGray = imgPng.clone();
	cv::cvtColor(imgPng, imgPngGray, cv::COLOR_BGR2GRAY);
	cv::imwrite("lab03_rgb.png", imgPng);
	cv::imwrite("lab03_gre.png", imgPngGray);

	int LUT[256];
	newFuncLUT(LUT);

	cv::Mat graph(512, 512, CV_8UC3, cv::Scalar(255, 255, 255));

	for (int i = 0; i < 255; i++) {
		cv::line(graph, cv::Point(2 * i, graph.rows - 2 * LUT[i]),
			cv::Point(2 * (i + 1), graph.rows - 2 * LUT[i + 1]),
			cv::Scalar(0, 0, 0), 1, 8, 0);
	}
	cv::imwrite("lab03_viz_func.png", graph);

	newFunc(imgPngGray);
	cv::imwrite("lab03_gre_res.png", imgPngGray);

	cv::Mat channelsPNG[3];
	cv::split(imgPng, channelsPNG);
	newFunc(channelsPNG[0]);
	newFunc(channelsPNG[1]);
	newFunc(channelsPNG[2]);
	cv::Mat mergeImg_png;
	cv::merge(channelsPNG, 3, mergeImg_png);

	cv::imwrite("lab03_rgb_res.png", mergeImg_png);
	cv::waitKey(0);
}
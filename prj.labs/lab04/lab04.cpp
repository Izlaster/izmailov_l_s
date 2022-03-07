#include <opencv2/opencv.hpp>

int main() {
	cv::Mat src;
	cv::VideoCapture cap("../../../data/video_money.mp4");
	cap >> src;
	cv::imwrite("src1.png", src);
	
	cv::Mat graySrc = cv::imread("src1.png", cv::IMREAD_GRAYSCALE);
	cv::imwrite("src2.png", graySrc);
	
	cv::Mat binarySrc;
	cv::threshold(graySrc, binarySrc, 190, 255, cv::THRESH_BINARY);
	cv::imshow("src3.png", binarySrc);
	cv::imwrite("src3.png", binarySrc);

	cv::Mat morghologySrc;
	cv::morphologyEx(binarySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(130, 130)));
	cv::morphologyEx(morghologySrc, morghologySrc, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));
	cv::imshow("src4.png", morghologySrc);
	cv::imwrite("src4.png", morghologySrc);

	cv::Mat srcOut(morghologySrc.size(), CV_32S);;
	int nLabels = cv::connectedComponents(morghologySrc, srcOut, 8);
	std::cout << std::endl;
	std::cout << nLabels << std::endl;

	cv::waitKey(0);
}
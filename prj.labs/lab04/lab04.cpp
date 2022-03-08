#include <opencv2/opencv.hpp>

void labProcess(std::string videoName, int binary, int morghology) {
	cv::Mat src[3];
	cv::VideoCapture cap("../../../data/" + videoName + ".mp4");
	int frameAll = cap.get(cv::CAP_PROP_FRAME_COUNT);

	for (int i = 0; i < 3; i++)
	{
		int frameNum = frameAll / 5 * (i + 2);
		cap.set(cv::CAP_PROP_POS_FRAMES, frameNum);
		cap >> src[i];
		cv::imwrite("original_" + videoName + std::to_string(i + 1) + ".png", src[i]);

		cv::Mat graySrc = src[i];
		cv::cvtColor(graySrc, graySrc, cv::COLOR_BGR2GRAY);

		cv::Mat binarySrc;
		cv::threshold(graySrc, binarySrc, binary, 255, cv::THRESH_BINARY);
		cv::imshow("binary_" + videoName + std::to_string(i + 1) + ".png", binarySrc);
		cv::imwrite("binary_" + videoName + std::to_string(i + 1) + ".png", binarySrc);

		cv::Mat morghologySrc;
		cv::morphologyEx(binarySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morghology, morghology)));
		cv::morphologyEx(morghologySrc, morghologySrc, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10)));
		cv::imshow("morghology_" + videoName + std::to_string(i + 1) + ".png", morghologySrc);
		cv::imwrite("morghology_" + videoName + std::to_string(i + 1) + ".png", morghologySrc);

		cv::Mat srcOut(morghologySrc.size(), CV_32S);;
		int nLabels = cv::connectedComponents(morghologySrc, srcOut, 8);
		std::cout << std::endl;
		std::cout << nLabels << std::endl;

		cv::waitKey(0);
	}
}

int main() {
	labProcess("video_money", 187, 140);
}
#include <opencv2/opencv.hpp>

void labProcess(std::string videoName, int binary, int morghology, int binaryFlag = cv::THRESH_BINARY, int morphFlag = cv::MORPH_CLOSE) {
	cv::Mat src[3];
	cv::VideoCapture cap("../../../data/lab_04_videos/" + videoName + ".mp4");
	int frameAll = cap.get(cv::CAP_PROP_FRAME_COUNT);

	for (int i = 0; i < 3; i++)
	{
		int frameNum = frameAll / 5 * (i + 2);
		cap.set(cv::CAP_PROP_POS_FRAMES, frameNum);
		cap >> src[i];
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_original_" + ".png", src[i]);

		cv::Mat graySrc = src[i];
		cv::cvtColor(graySrc, graySrc, cv::COLOR_BGR2GRAY);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_gray_" + ".png", graySrc);

		cv::Mat binarySrc;
		cv::threshold(graySrc, binarySrc, binary, 255, binaryFlag);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_binary_" + ".png", binarySrc);

		cv::Mat morghologySrc;
		cv::morphologyEx(binarySrc, morghologySrc, morphFlag, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morghology, morghology)));
		cv::morphologyEx(morghologySrc, morghologySrc, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10)));
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_morghology_" + ".png", morghologySrc);

		cv::Mat srcOut(morghologySrc.size(), CV_32S);;
		cv::Mat stats, centroids;
		int nLabels = cv::connectedComponentsWithStats(morghologySrc, srcOut, stats, centroids);
		
		cv::Mat outconnectedComponents(morghologySrc.size(), CV_8UC3);
		int labelMax = 1;
		int max = 0;
		for (int j = 1; j < nLabels; j++) {
			if (max > stats.at<int>(j, cv::CC_STAT_AREA)) {
				max = stats.at<int>(j, cv::CC_STAT_AREA);
				labelMax = j;
			}
		}
		std::vector<cv::Vec3b> colors(nLabels);
		for (int j = 1; j < nLabels; j++) {
			colors[j] = cv::Vec3b(0, 0, 0);
		}
		colors[labelMax] = cv::Vec3b(255, 255, 255);
		for (int j = 0; j < srcOut.rows; j++) {
			for (int l = 0; l < srcOut.cols; l++)
			{
				int label = srcOut.at<int>(j, l);
				outconnectedComponents.at<cv::Vec3b>(j, l) = colors[label];
			}
		}
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_connectedComponents_" + ".png", outconnectedComponents);

		cv::waitKey(0);
	}
}

int main() {
	labProcess("video_money", 187, 140);
	labProcess("1kRub", 50, 50);
	labProcess("50Rub", 100, 30);
	labProcess("200Rub", 135, 90);
	labProcess("100Rub", 35, 460, cv::THRESH_BINARY_INV);
	labProcess("100Usd", 87, 215, cv::THRESH_BINARY_INV);
}
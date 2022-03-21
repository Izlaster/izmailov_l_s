#include <opencv2/opencv.hpp>

void labProcess(std::string videoName, std::string maskName) {
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

		int whiteCol = 0;
		for (int j = 0; j < graySrc.rows; j++) {
			for (int l = 0; l < graySrc.cols; l++) {
				if (graySrc.at<uint8_t>(j, l) >= 95) {
					whiteCol++;
				}
			}
		}
		int binaryFlag;
		if (whiteCol > (graySrc.rows * graySrc.cols - (graySrc.rows * graySrc.cols / 5)))
			binaryFlag = cv::THRESH_BINARY_INV;
		else
			binaryFlag = cv::THRESH_BINARY;
		cv::Mat binarySrc;
		cv::threshold(graySrc, binarySrc, 100, 255, binaryFlag);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_binary_" + ".png", binarySrc);

		cv::Mat morghologySrc;
		cv::morphologyEx(binarySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(80, 80)));
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

		cv::Mat mask = cv::imread("../../../data/lab_04_masks/" + maskName + std::to_string(i + 1) + ".png");
		double percent = 0;
		for (int j = 0; j < outconnectedComponents.rows; j++) {
			for (int l = 0; l < outconnectedComponents.cols; l++) {
				if (outconnectedComponents.at<uint8_t>(j, l) == mask.at<uint8_t>(j, l)) {
					percent++;
				}
			}
		}
		std::cout << std::endl;
		std::cout << percent / (outconnectedComponents.rows * outconnectedComponents.cols) * 100 << "%" << std::endl;

		cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);
		cv::cvtColor(outconnectedComponents, outconnectedComponents, cv::COLOR_BGR2GRAY);
		cv::Mat vizualErrors(mask.size(), CV_8UC3);
		vizualErrors = 0;
		for (int j = 0; j < mask.rows; j++)
			for (int l = 0; l < mask.cols; l++) {
				if (mask.at<uint8_t>(j, l) == outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 0) {
					vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(0, 0, 0);
				}
				else if (mask.at<uint8_t>(j, l) == outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 255) {
					vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(255, 255, 255);
				}
				else if (mask.at<uint8_t>(j, l) != outconnectedComponents.at<uint8_t>(j, l) && outconnectedComponents.at<uint8_t>(j, l) == 0) {
					vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(255, 0, 0);
				}
				else if (mask.at<uint8_t>(j, l) != outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 0) {
					vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(0, 0, 255);
				}
			}
		cv::Mat vizual;
		addWeighted(src[i], 0.5, vizualErrors, 0.5, 0.0, vizual);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_visual_" + ".png", vizual);

		cv::waitKey(0);
	}
}

int main() {
	//labProcess("1kRub", "1kRub_mask_");
	//labProcess("50Rub", "50Rub_mask_");
	//labProcess("100Rub", "100Rub_mask_");
	//labProcess("100Usd", "100Usd_mask_");
	labProcess("200Rub", "200Rub_mask_");
}
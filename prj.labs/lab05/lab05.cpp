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

		cv::Mat graySrc;
		cv::resize(src[i], graySrc, cv::Size(), 0.25, 0.25);

		cv::cvtColor(graySrc, graySrc, cv::COLOR_BGR2GRAY);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_gray_" + ".png", graySrc);

		cv::Mat gaussSrc;
		cv::GaussianBlur(graySrc, gaussSrc, cv::Size(7, 11), 9, 3);
		cv::imshow("frames/" + videoName + "_" + std::to_string(i + 1) + "_gaussSrc_" + ".png", gaussSrc);

		cv::Mat cannySrc;
		cv::Canny(gaussSrc, cannySrc, 30, 105);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_cannySrc_" + ".png", cannySrc);

		cv::Mat morghologySrc;
		cv::morphologyEx(cannySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5)));
		cv::morphologyEx(morghologySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 1)));

		std::vector<cv::Vec4i> findContoursSrc;
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(morghologySrc, contours, findContoursSrc, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

		//Õ≈ œ–» ŒÀ
		std::cout << std::endl;
		for (size_t y = 0; y < contours.size(); y++)
		{
			for (size_t z = 0; z < 4; z++)
			{
				std::cout << findContoursSrc[y][z] << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << contours.size() << "  <-  LOLOLOLOLLOLOLOLOLOLOLLOLOOLOLOLOLOL" << std::endl;

		cv::Mat rez = morghologySrc.clone();
		rez = 0;
		if (contours.size() == 1) {
			for (int y = 0; y < contours[0].size(); y++) {
				cv::line(rez, contours[0][y], contours[0][y], cv::Scalar(255, 255, 255));
			}
		}
		else
		{
			int maxS = 0, maxI = 0;
			for (int y = 0; y < contours.size(); y++) {
				if (contours[y].size() > maxS) {
					maxS = contours[y].size();
					maxI = y;
				}
			}
			for (int y = 0; y < contours[maxI].size(); y++) {
				cv::line(rez, contours[maxI][y], contours[maxI][y], cv::Scalar(255, 255, 255));
			}
		}

		cv::resize(rez, rez, cv::Size(), 2, 2);
		cv::imshow("frames/" + videoName + "_" + std::to_string(i + 1) + "_rez_" + ".png", rez);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_rez_" + ".png", rez);

		//cv::Mat srcOut(morghologySrc.size(), CV_32S);;
		//cv::Mat stats, centroids;
		//int nLabels = cv::connectedComponentsWithStats(morghologySrc, srcOut, stats, centroids);

		//cv::Mat outconnectedComponents(morghologySrc.size(), CV_8UC3);
		//int labelMax = 1;
		//int max = 0;
		//for (int j = 1; j < nLabels; j++) {
		//	if (max > stats.at<int>(j, cv::CC_STAT_AREA)) {
		//		max = stats.at<int>(j, cv::CC_STAT_AREA);
		//		labelMax = j;
		//	}
		//}
		//std::vector<cv::Vec3b> colors(nLabels);
		//for (int j = 1; j < nLabels; j++) {
		//	colors[j] = cv::Vec3b(0, 0, 0);
		//}
		//colors[labelMax] = cv::Vec3b(255, 255, 255);
		//for (int j = 0; j < srcOut.rows; j++) {
		//	for (int l = 0; l < srcOut.cols; l++)
		//	{
		//		int label = srcOut.at<int>(j, l);
		//		outconnectedComponents.at<cv::Vec3b>(j, l) = colors[label];
		//	}
		//}
		//cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_connectedComponents_" + ".png", outconnectedComponents);

		//cv::Mat mask = cv::imread("../../../data/lab_04_masks/" + maskName + std::to_string(i + 1) + ".png");
		//double percent = 0;
		//for (int j = 0; j < outconnectedComponents.rows; j++) {
		//	for (int l = 0; l < outconnectedComponents.cols; l++) {
		//		if (outconnectedComponents.at<uint8_t>(j, l) == mask.at<uint8_t>(j, l)) {
		//			percent++;
		//		}
		//	}
		//}
		//std::cout << std::endl;
		//std::cout << percent / (outconnectedComponents.rows * outconnectedComponents.cols) * 100 << "%" << std::endl;

		//cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY);
		//cv::cvtColor(outconnectedComponents, outconnectedComponents, cv::COLOR_BGR2GRAY);
		//cv::Mat vizualErrors(mask.size(), CV_8UC3);
		//vizualErrors = 0;
		//for (int j = 0; j < mask.rows; j++)
		//	for (int l = 0; l < mask.cols; l++) {
		//		if (mask.at<uint8_t>(j, l) == outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 0) {
		//			vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(0, 0, 0);
		//		}
		//		else if (mask.at<uint8_t>(j, l) == outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 255) {
		//			vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(255, 255, 255);
		//		}
		//		else if (mask.at<uint8_t>(j, l) != outconnectedComponents.at<uint8_t>(j, l) && outconnectedComponents.at<uint8_t>(j, l) == 0) {
		//			vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(255, 0, 0);
		//		}
		//		else if (mask.at<uint8_t>(j, l) != outconnectedComponents.at<uint8_t>(j, l) && mask.at<uint8_t>(j, l) == 0) {
		//			vizualErrors.at<cv::Vec3b>(j, l) += cv::Vec3b(0, 0, 255);
		//		}
		//	}
		//cv::Mat vizual;
		//addWeighted(src[i], 0.5, vizualErrors, 0.5, 0.0, vizual);
		//cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_visual_" + ".png", vizual);

		cv::waitKey(0);
	}
}

int main() {
	labProcess("1kRub", "1kRub_mask_");
	labProcess("50Rub", "50Rub_mask_");
	labProcess("100Rub", "100Rub_mask_");
	labProcess("100Usd", "100Usd_mask_");
	labProcess("200Rub", "200Rub_mask_");
}


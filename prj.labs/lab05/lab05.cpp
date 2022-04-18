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
		cv::GaussianBlur(graySrc, gaussSrc, cv::Size(3, 3), 9, 3);
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

		cv::Mat afterContours = morghologySrc.clone();
		afterContours = 0;

		std::vector<std::vector<cv::Point>> approx;
		std::vector<cv::Point> approxTmp;
		for (int y = 0; y < contours.size(); y++) {
			approxPolyDP(contours[y], approxTmp, arcLength(contours[y], true) * 0.01, true);
			approx.push_back(approxTmp);
		}

		int idx = 0;
		for (; idx >= 0; idx = findContoursSrc[idx][0]) {
			cv::drawContours(afterContours, approx, idx, cv::Scalar(255, 255, 255));
		}

		cv::resize(afterContours, afterContours, cv::Size(), 2, 2);
		cv::imshow("frames/" + videoName + "_" + std::to_string(i + 1) + "_cornerSub_" + ".png", afterContours);

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


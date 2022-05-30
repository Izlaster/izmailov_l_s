#include <opencv2/opencv.hpp>

long GetFileSize(std::string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}


void labProcess(std::string videoName, std::string maskName) {
	cv::Mat src[3];
	cv::VideoCapture cap("../../../data/lab_04_videos/" + videoName + ".mp4");
	int frameAll = cap.get(cv::CAP_PROP_FRAME_COUNT);

	cv::FileStorage fs("../../../data/lab_05_coords.json", cv::FileStorage::READ);

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
		//cv::imshow("frames/" + videoName + "_" + std::to_string(i + 1) + "_gaussSrc_" + ".png", gaussSrc);

		cv::Mat cannySrc;
		cv::Canny(gaussSrc, cannySrc, 30, 115);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_cannySrc_" + ".png", cannySrc);

		cv::Mat morghologySrc;
		cv::morphologyEx(cannySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 5)));
		cv::morphologyEx(morghologySrc, morghologySrc, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 1)));

		std::vector<cv::Vec4i> findContoursSrc;
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(morghologySrc, contours, findContoursSrc, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

		cv::Mat afterContours = morghologySrc.clone();
		afterContours = 0;

		std::vector<std::vector<cv::Point>> approx;
		std::vector<cv::Point> approxTmp;
		for (int y = 0; y < contours.size(); y++) {
			approxPolyDP(contours[y], approxTmp, arcLength(contours[y], true) * 0.01, true);
			approx.push_back(approxTmp);
		}

		int maxS = 0, maxI = 0;
		if (contours.size() == 1) {
			for (int y = 0; y < 4; y++) {
				cv::line(afterContours, approx[0][y], approx[0][(y + 1) % 4], cv::Scalar(255, 255, 255));
			}
		}
		else {
			for (int y = 0; y < contours.size(); y++) {
				if (contours[y].size() > maxS) {
					maxS = contours[y].size();
					maxI = y;
				}
			}
			for (int y = 0; y < 4; y++) {
				cv::line(afterContours, approx[maxI][y], approx[maxI][(y + 1) % 4], cv::Scalar(255, 255, 255));
			}
		}

		cv::resize(afterContours, afterContours, cv::Size(), 4, 4);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_afterContours_" + ".png", afterContours);

		cv::FileNode points_x, points_y;
		std::vector<cv::Point> points_xy;
		std::string root = videoName + "_" + std::to_string(i + 1) + "_original_" + ".png" +
			std::to_string(GetFileSize("frames/" + videoName + "_" + std::to_string(i + 1) + "_original_" + ".png"));
		points_x = fs[root]["regions"][0]["shape_attributes"]["all_points_x"];
		points_y = fs[root]["regions"][0]["shape_attributes"]["all_points_y"];
		for (int j = 0; j < 4; j++) {
			points_xy.push_back(cv::Point(points_x[j], points_y[j]));
		}

		cv::Mat bestContours = afterContours.clone();
		bestContours = 0;
		for (int j = 0; j < 4; j++) {
			cv::line(bestContours, points_xy[j], points_xy[(j + 1) % 4], cv::Scalar(255, 255, 255), 4);
		}
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_bestContours_" + ".png", bestContours);

		std::vector<std::vector<cv::Point>> bestContoursContours;
		std::vector<cv::Vec4i> name;
		cv::findContours(bestContours, bestContoursContours, name, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
		double perimeter = cv::arcLength(bestContoursContours[0], true);
		double mErr = 0, Err;
		for (int j = 0; j < 4; j++) {
			double distance = cv::norm(approx[maxI][j] - (points_xy[j] / 4));
			Err = distance / perimeter;
			if (Err > mErr) {
				mErr = Err;
			}
		}
		std::cout << std::endl;
		std::cout << videoName + "_" + std::to_string(i + 1) + ".png" << std::endl;
		std::cout.precision(2);
		std::cout << mErr * 100 << "%" << std::endl;
		std::cout << std::endl;
		
		cv::Mat vizualErrors(afterContours.size(), CV_8UC3);
		vizualErrors = 0;
		cv::threshold(afterContours, afterContours, 1, 255, cv::THRESH_BINARY);
		for (int j = 0; j < afterContours.rows; j++) {
			for (int l = 0; l < afterContours.cols; l++) {
				if (bestContours.at<uint8_t>(j, l) == 255) {
					vizualErrors.at<cv::Vec3b>(j, l) = cv::Vec3b(255, 255, 255);
				}
				if (afterContours.at< uint8_t>(j, l) == afterContours.at<uint8_t>(j, l) && bestContours.at<uint8_t>(j, l) == 0) {
					vizualErrors.at<cv::Vec3b>(j, l) = cv::Vec3b(0, 0, 0);
				}
				if (bestContours.at<uint8_t>(j, l) == afterContours.at<uint8_t>(j, l) && bestContours.at<uint8_t>(j, l) == 255) {
					vizualErrors.at<cv::Vec3b>(j, l) = cv::Vec3b(0, 255, 0);
				}
				else if (bestContours.at<uint8_t>(j, l) != afterContours.at<uint8_t>(j, l) && afterContours.at<uint8_t>(j, l) == 255) {
					vizualErrors.at<cv::Vec3b>(j, l) = cv::Vec3b(0, 0, 255);
				}
			}
		}

		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_vizualErrors_" + ".png", vizualErrors);

		cv::Mat vizual;
		addWeighted(src[i], 0.5, vizualErrors, 0.5, 0.0, vizual);
		cv::imwrite("frames/" + videoName + "_" + std::to_string(i + 1) + "_vizual_" + ".png", vizual);

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

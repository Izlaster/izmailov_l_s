#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat I_1(60, 768, CV_8UC1);
    cv::TickMeter tm1, tm2;

    I_1 = 0;
    for (int i = 0; i < I_1.rows; i++)
        for (int j = 0; j < I_1.cols; j++)
            I_1.at<uint8_t>(i, j) = j / 3;

    cv::Mat G_1;
    G_1 = I_1.clone();
    tm1.start();
    I_1.convertTo(G_1, CV_32FC1, 1.0 / 255.0);
    cv::pow(G_1, 2.4, G_1);
    G_1.convertTo(G_1, CV_8UC1, 255.0 / 1.0);
    tm1.stop();

    cv::Mat G_2;
    G_2 = I_1.clone();
    tm2.start();
    for (int i = 0; i < G_2.rows; i++)
        for (int j = 0; j < G_2.cols; j++)
            G_2.at<uint8_t>(i, j) = std::pow(G_2.at<uint8_t>(i, j) / 255.0, 2.4) * 255;
    tm2.stop();

    cv::Mat img;
    img = 0;
    cv::Mat mArr[] = { I_1, G_1, G_2 };
    cv::vconcat(mArr, 3, img);

    std::cout << "G_1 Total time: " << tm1.getAvgTimeSec() << std::endl;
    std::cout << "G_2 Total time: " << tm2.getAvgTimeSec() << std::endl;

    cv::imwrite("lab01.png", img);
    cv::waitKey(0);
}
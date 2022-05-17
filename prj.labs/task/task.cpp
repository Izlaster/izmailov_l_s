#include <opencv2/opencv.hpp>

int main() {
    cv::Mat screen(300, 450, CV_32FC1);
    std::vector <std::vector<float>> color = { {0, 127, 255}, {255, 0, 127} };
    int radius = 75;
    int width = 150;
    cv::Rect2d rect(0, 0, 150, 150);

    for (int i = 0; i < 2; i++) {
        rect.y = i * width;
        for (int j = 0; j < 3; j++) {
            rect.x = j * width;
            cv::Mat tmp = screen(rect);
            tmp = color[i][j] / 255;
            cv::circle(screen, cv::Point(rect.x + radius, rect.y + radius), 55, color[1 - i][j] / 255, -1);
        }
    }

    cv::imshow("screen", screen);
    cv::imwrite("screen.png", screen * 255);

    cv::Mat I1(2, 2, CV_32FC1);
    I1 = 0;
    I1.at<float>(0, 1) = 1;
    I1.at<float>(1, 0) = -1;
    cv::Mat filtImg1;
    cv::filter2D(screen, filtImg1, -1, I1, cv::Point(0, 0));

    cv::Mat I2(2, 2, CV_32FC1);
    I2 = 0;
    I2.at<float>(1, 0) = 1;
    I2.at<float>(0, 1) = -1;
    cv::Mat filtImg2;
    cv::filter2D(screen, filtImg2, -1, I2, cv::Point(0, 0));

    cv::Mat middle(300, 450, CV_32FC1);
    for (int i = 0; i < middle.rows; i++) {
        for (int j = 0; j < middle.cols; j++) {
            middle.at<float>(i, j) = filtImg1.at<float>(i, j) * filtImg1.at<float>(i, j) + filtImg2.at<float>(i, j) * filtImg2.at<float>(i, j);
        }
    }

    for (int i = 0; i < middle.rows; i++) {
        for (int j = 0; j < middle.cols; j++) {
            middle.at<float>(i, j) = std::sqrt(middle.at<float>(i, j));
        }
    }

    filtImg1 = (filtImg1 + 1) / 2;
    cv::imshow("filtImg1", filtImg1);
    cv::imwrite("filtImg1.png", filtImg1 * 255);

    filtImg2 = (filtImg2 + 1) / 2;
    cv::imshow("filtImg2", filtImg2);
    cv::imwrite("filtImg2.png", filtImg2 * 255);

    cv::imshow("middle", middle);
    cv::imwrite("middle.png", middle * 255);

    cv::waitKey(0);
}

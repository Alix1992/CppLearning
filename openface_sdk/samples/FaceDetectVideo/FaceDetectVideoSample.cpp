#include <opencv2/opencv.hpp>

#ifdef _MS_VER
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#else
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#endif
#endif

using namespace std;
using namespace cv;

#include "firefly_faceverify.h"

int CheckImageType(const unsigned char *image_data,
	const rr_image_type image_type,
	const int image_width,
	const int image_height)
{
	int cv_type = (int)rr_image_type::RR_IMAGE_MAX;

	switch (image_type)
	{
		case rr_image_type::RR_IMAGE_BGR8UC3:
		{
			cv_type = CV_8UC3;
		}
		break;

		case rr_image_type::RR_IMAGE_BGRA8UC4:
		{
			cv_type = CV_8UC4;
		}
		break;

		case rr_image_type::RR_IMAGE_GRAY8UC1:
		{
			cv_type = CV_8UC1;
		}
		break;

		default:
			return RR_E_ILLEGAL_ARGUMENT;
	}

	//
	cv::Mat img;
	try
	{
		img = cv::Mat(cv::Size(image_width, image_height), cv_type, (void*)image_data).clone();
	}
	catch (cv::Exception& e)
	{
		if (e.code == CV_StsNoMem)
		{
			return RR_E_NO_MEMORY;
		}

		return RR_E_FAIL;
	}
	if (img.empty() == true)
	{
		return RR_E_FAIL;
	}

	cv::namedWindow("CheckImageType");
	cv::imshow("CheckImageType", img);
	cv::waitKey(0);

	return RR_OK;
}

int main()
{
	printf("**************************************************\n");
	printf("FaceDetectVideoSample\n");
	printf("www.openface.com.cn\n");
	printf("\n");
	printf("SDK VERSION: rr_facedetect.%d.%d.%d\n", RR_GET_MAJOR_VERSION(firefly_fd_get_version()), RR_GET_MINOR_VERSION(firefly_fd_get_version()), RR_GET_PATCH_VERSION(firefly_fd_get_version()));
	printf("**************************************************\n");
	printf("\n");

	string video_file = "test.mp4";

	string model_dir; // Ä£ÐÍÎÄ¼þ¼Ð
	model_dir = "../../models";
	//model_dir = "models";

	int ret = 0;

	rr_handle_t hDetector = NULL;
	hDetector = firefly_fd_init_detector(model_dir.c_str(), RR_FD_VERSION);
	if (hDetector == NULL)
	{
		printf("firefly_fd_init_detector() failed\n");
		return -1;
	}

	//
	cv::VideoCapture capture;
	capture.open(video_file);
	if (capture.isOpened() == false)
	{
		printf("capture.open %s failed!\n", video_file.c_str());
		return -1;
	}

	int nFrameCount = capture.get(CV_CAP_PROP_FRAME_COUNT);

	int iFrame = 0;
	while (iFrame < nFrameCount)
	{
		// frame
		cv::Mat frame;
		ret = capture.read(frame);
		if (ret == false)
		{
			break;
		}
		iFrame++;

		rr_face_t* faces_array = NULL;
		int faces_count = 0;

		// you could test image_data, image_type and show image, when you met image_type problem.
		//if (CheckImageType(img.data, rr_image_type::RR_IMAGE_BGR8UC3, img.cols, img.rows) != RR_OK) printf("img data or type ERROR!\n");
		ret = firefly_fd_detect_image(hDetector, frame.data, rr_image_type::RR_IMAGE_BGR8UC3, frame.cols, frame.rows, &faces_array, &faces_count);
		if (ret != RR_OK)
		{
			printf("firefly_fd_detect_image() failed: %d\n", ret);
			printf("press ENTER to continue");
			getchar();
			continue;
		}

		///
		// frame #
		string info = "# " + to_string(iFrame);
		cv::Point ptText(100, 30);
		putText(frame, info.c_str(), ptText, CV_FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0, 0, 255), 1);

		// draw faces rect and id
		for (int i = 0; i<faces_count; i++)
		{
			float x = faces_array[i].rect.left;
			float y = faces_array[i].rect.top;
			float h = faces_array[i].rect.bottom - faces_array[i].rect.top + 1;
			float w = faces_array[i].rect.right - faces_array[i].rect.left + 1;
			cv::rectangle(frame, cv::Rect(x, y, w, h), cv::Scalar(255, 0, 0), 2);

			//for (int j = 0; j < 5; j++)
			//{
			//	cv::circle(frame, cv::Point(faces_array[i].landmarks[j].x, faces_array[i].landmarks[j].y), 1, cv::Scalar(255, 255, 0), 2);
			//}

			cv::Point pt(faces_array[i].rect.left, faces_array[i].rect.top);
			string strText = to_string(faces_array[i].id) + " " + to_string(faces_array[i].confidence);
			putText(frame, strText.c_str(), pt, CV_FONT_HERSHEY_PLAIN, 1.2, cv::Scalar(0, 0, 255), 1);
		}

		firefly_fd_free_detect_result(faces_array);

		cv::namedWindow("test");
		cv::imshow("test", frame);
		cv::waitKey(1);
	}

	//
	capture.release();
	firefly_fd_deinit_detector(hDetector);

	return 0;
}

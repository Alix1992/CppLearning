#include <opencv2/opencv.hpp>

#ifdef _MSC_VER
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

int FaceVerify(const rr_handle_t hDetector, const rr_handle_t hVerifier, const string img_file1, const string img_file2, float* p_score)
{
	int ret = 0;

#define FEATURE_LEN 256
	float feature1[FEATURE_LEN] = { 0 };
	float feature2[FEATURE_LEN] = { 0 };
	int feature_len = FEATURE_LEN;

	///
	cv::Mat img;
	rr_face_t* faces_array = NULL;
	int faces_count = 0;

	/// feature1
	img = cv::imread(img_file1);

	ret = firefly_fd_detect_image(hDetector, img.data, RR_IMAGE_BGR8UC3, img.cols, img.rows, &faces_array, &faces_count);
	if (ret != RR_OK)
	{
		printf("firefly_fd_detect_image() failed: %d\n", ret);
		return -1;
	}

	if (faces_count == 0)
		return -1;

	ret = firefly_fv_extract_feature(hVerifier, img.data, RR_IMAGE_BGR8UC3, img.cols, img.rows, faces_array[0].landmarks, 5, feature1, FEATURE_LEN);
	if (ret != RR_OK)
	{
		printf("firefly_fv_extract_feature() failed: %d\n", ret);
		return -1;
	}

	firefly_fd_free_detect_result(faces_array);

	/// feature2
	img = cv::imread(img_file2);

	ret = firefly_fd_detect_image(hDetector, img.data, RR_IMAGE_BGR8UC3, img.cols, img.rows, &faces_array, &faces_count);
	if (ret != RR_OK)
	{
		printf("firefly_fd_detect_image() failed: %d\n", ret);
		return -1;
	}

	if (faces_count == 0)
		return -1;

	ret = firefly_fv_extract_feature(hVerifier, img.data, RR_IMAGE_BGR8UC3, img.cols, img.rows, faces_array[0].landmarks, 5, feature2, FEATURE_LEN);
	if (ret != RR_OK)
	{
		printf("firefly_fv_extract_feature() failed: %d\n", ret);
		return -1;
	}

	firefly_fd_free_detect_result(faces_array);

	///
	float score = firefly_fv_compare_feature(feature1, feature2, feature_len);
	*p_score = score;

	return 0;
}

int main()
{
	printf("**************************************************\n");
	printf("Face Verify Sample\n");
	printf("**************************************************\n");
	printf("\n");

	printf("loading...\n\n");

	int ret = 0;

	string detect_model;
	detect_model = "../../models";

	string verify_model;
	verify_model = "../../models";

	///
	rr_handle_t hDetector = NULL;
	hDetector = firefly_fd_init_detector(detect_model.c_str(), RR_FD_VERSION);
	if (hDetector == NULL)
	{
		printf("firefly_fd_init_detector() failed\n");
		return -1;
	}

	rr_handle_t hVerifier = firefly_fv_init_verifier(verify_model.c_str());
	if (hVerifier == NULL)
	{
		printf("firefly_fv_init_verifier() failed!\n");
		return -1;
	}

	///
	float fScore = 0.0;
	string img_file1;
	string img_file2;

	///
	img_file1 = "test-images/Jennifer1.jpg";
	img_file2 = "test-images/Jennifer2.jpg";

	ret = FaceVerify(hDetector, hVerifier, img_file1, img_file2, &fScore);
	if (ret == 0)
	{
		printf("verify: %s vs %s\n", img_file1.c_str(), img_file2.c_str());
		printf("score: %f\n", fScore);
		printf("\n");
	}

	///
	img_file1 = "test-images/Aaron1.jpg";
	img_file2 = "test-images/Aaron2.jpg";

	ret = FaceVerify(hDetector, hVerifier, img_file1, img_file2, &fScore);
	if (ret == 0)
	{
		printf("verify: %s vs %s\n", img_file1.c_str(), img_file2.c_str());
		printf("score: %f\n", fScore);
		printf("\n");
	}

	///
	img_file1 = "test-images/Jennifer1.jpg";
	img_file2 = "test-images/Aaron1.jpg";

	ret = FaceVerify(hDetector, hVerifier, img_file1, img_file2, &fScore);
	if (ret == 0)
	{
		printf("verify: %s vs %s\n", img_file1.c_str(), img_file2.c_str());
		printf("score: %f\n", fScore);
		printf("\n");
	}

	///
	firefly_fd_deinit_detector(hDetector);
	firefly_fv_deinit_verifier(hVerifier);

	///
	printf("press ENTER to exit");
	getchar();

	return 0;
}

#ifndef __FIREFLY_FACEVERIFY_H__
#define __FIREFLY_FACEVERIFY_H__

#define RR_FD_VERSION_MAJOR    3
#define RR_FD_VERSION_MINOR    0
#define RR_FD_VERSION_PATCH    0

#define RR_FD_VERSION (RR_FD_VERSION_MAJOR << 16 | RR_FD_VERSION_MINOR << 8 | RR_FD_VERSION_PATCH)

#define RR_GET_MAJOR_VERSION(__V) (__V&0x00ff0000)>>16
#define RR_GET_MINOR_VERSION(__V) (__V&0x0000ff00)>>8
#define RR_GET_PATCH_VERSION(__V) (__V&0x000000ff)

#define RR_STR_EXP(__A)			#__A
#define RR_STR(__A)				RR_STR_EXP(__A)
#define RR_FD_VERSION_STRING	RR_STR(RR_FD_VERSION_MAJOR) "." RR_STR(RR_FD_VERSION_MINOR) "." RR_STR(RR_FD_VERSION_PATCH)

#ifdef __cplusplus
extern "C" {
#endif
	//---------------------------------------------------------------------------------------------------------------------------------
#ifdef _MSC_VER
#	ifdef __cplusplus
#		ifdef RR_STATIC_LIB
#			define RR_SDK_API  extern "C"
#		else
#			ifdef RR_SDK_EXPORTS
#				define RR_SDK_API extern "C" __declspec(dllexport)
#			else
#				define RR_SDK_API extern "C" __declspec(dllimport)
#			endif
#		endif
#	else
#		ifdef RR_STATIC_LIB
#			define RR_SDK_API
#		else
#			ifdef RR_SDK_EXPORTS
#				define RR_SDK_API __declspec(dllexport)
#			else
#				define RR_SDK_API __declspec(dllimport)
#			endif
#		endif
#	endif
#else /* _MSC_VER */
#	ifdef __cplusplus
#		ifdef RR_SDK_EXPORTS
#			define RR_SDK_API  extern "C" __attribute__((visibility ("default")))
#		else
#			define RR_SDK_API extern "C"
#		endif
#	else
#		ifdef RR_SDK_EXPORTS
#			define RR_SDK_API __attribute__((visibility ("default")))
#		else
#			define RR_SDK_API
#		endif
#	endif
#endif

#ifndef __FIREFLY_COMMON_H__
#define __FIREFLY_COMMON_H__
/// rr handle
typedef void *rr_handle_t;

#define RR_OK						 0	///< 函数返回成功
#define RR_E_ILLEGAL_ARGUMENT		-1	///< 非法参数
#define RR_E_INVALID_HANDLE			-2	///< 无效句柄
#define RR_E_NO_MEMORY				-3	///< 内存不足
#define RR_E_FAIL					-4	///< 内部错误
#define RR_E_INCOMPATIBLE_VERSION	-5  ///< 版本不兼容
#define RR_E_BAD_CONFIG_FILE		-6  ///< 配置文件格式错误
#define RR_E_FILE_NOT_FOUND			-7  ///< 找不到文件

/// rr rectangle
typedef struct rr_rect_t {
	int left;	///< 矩形左边的坐标
	int top;	///< 矩形顶边的坐标
	int right;	///< 矩形右边的坐标
	int bottom;	///< 矩形底边的坐标
} rr_rect_t;

/// rr point, float type
typedef struct rr_pointf_t {
	float x;	///< 点的x坐标，float类型
	float y;	///< 点的y坐标，float类型
} rr_pointf_t;

/// rr point, int type
typedef struct rr_pointi_t {
	int x;		///< 点的x坐标，int类型
	int y;		///< 点的y坐标，int类型
} rr_pointi_t;

/// rr image type
typedef enum {
	RR_IMAGE_BGR8UC3 = 0,	///< BGR  8:8:8   24bpp ( 3通道24bit BGR 像素 )
	RR_IMAGE_BGRA8UC4 = 1,	///< BGRA 8:8:8:8 32bpp ( 4通道32bit BGRA 像素 )
	RR_IMAGE_GRAY8UC1 = 2,	///< Y    1        8bpp ( 单通道8bit灰度像素 )
	RR_IMAGE_MAX
} rr_image_type;
#endif // end of __FIREFLY_COMMON_H__

/// @brief 人脸信息结构体
typedef struct rr_face_t {
	rr_rect_t rect;				///< 人脸的矩形区域，在图片中的位置。
	float confidence;			///< 本矩形区域是人脸的置信度。

	rr_pointf_t landmarks[5];	///< 人脸5关键点的数组。依次为 左眼中心，右眼中心，鼻尖，左嘴角，右嘴角。

	int yaw;			///< 水平转角的度数。正值代表脸向右看。
	int pitch;			///< 俯仰角的度数。正值代表脸向上看。
	int roll;			///< 旋转角的度数。正值代表脸向右肩倾斜。

	int id;				///< 不同帧中的同一个人的id
	float quality;		///< 人脸质量。本版本不提供。
} rr_face_t;

/// @brief 创建人脸检测句柄
/// @param model_path 模型文件夹的绝对路径或相对路径
/// @param sdk_version 程序编译时使用的sdk版本，必须传入RR_FD_VERSION
/// @return 成功返回人脸检测句柄，失败返回NULL
RR_SDK_API
rr_handle_t
firefly_fd_init_detector
(
	const char *model_path,
	const int sdk_version
);

/// @brief 销毁人脸检测句柄
/// @param detector_handle 人脸检测句柄
RR_SDK_API
void firefly_fd_deinit_detector(
	const rr_handle_t detector_handle
);

/// @brief 人脸检测
/// @param detector_handle 人脸检测句柄
/// @param image_data 用于检测的图像数据
/// @param image_type 用于检测的图像数据的格式
/// @param image_width 用于检测的图像的宽度(以像素为单位)
/// @param image_height 用于检测的图像的高度(以像素为单位)
/// @param p_faces_array 检测到的人脸信息数组。api内部分配内存，需要调用firefly_release_detect_result函数释放。
/// @param p_faces_count 检测到的人脸数量。api内部分配内存，需要调用firefly_release_detect_result函数释放。
/// @return 成功返回RR_OK，否则返回错误代码
RR_SDK_API
int
firefly_fd_detect_image(
	const rr_handle_t detector_handle,
	const unsigned char *image_data,
	const rr_image_type image_type,
	const int image_width,
	const int image_height,
	rr_face_t **p_faces_array,
	int *p_faces_count
);

/// @brief 释放人脸检测返回结果时分配的内存
/// @param faces_array 检测到的人脸信息数组
RR_SDK_API
void
rr_fd_release_detect_result(
	const rr_face_t *faces_array
);

#define firefly_fd_free_detect_result(a)  rr_fd_release_detect_result(a)

/// @brief 获得当前使用SDK的版本
/// @return 返回当前使用SDK的版本信息。可以对返回结果使用宏RR_GET_MAJOR_VERSION()，RR_GET_MINOR_VERSION()，RR_GET_PATCH_VERSION()获得更具体的版本信息。
RR_SDK_API
int firefly_fd_get_version();

RR_SDK_API
rr_handle_t
firefly_fv_init_verifier(
	const char *model_path
	);

RR_SDK_API
void firefly_fv_deinit_verifier(
	const rr_handle_t verifier_handle
	);

RR_SDK_API
int
firefly_fv_extract_feature(
	const rr_handle_t verifier_handle,
	const unsigned char *image_data,
	const rr_image_type image_type,
	const int image_width,
	const int image_height,
	rr_pointf_t* landmarks,	///< 人脸5关键点的数组。依次为 左眼中心，右眼中心，鼻尖，左嘴角，右嘴角。
	const int landmarks_num,
	float* p_feature,
	const int feature_len
	);

RR_SDK_API
float 
firefly_fv_compare_feature(
	const float* feature1,
	const float* feature2,
	const int feature_len
	);

#ifdef __cplusplus
}
#endif

//---------------------------------------------------------------------------------------------------------------------------------

#endif // end of ___FIREFLY_FACEVERIFY_H__

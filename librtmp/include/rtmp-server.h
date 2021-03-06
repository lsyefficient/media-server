#ifndef _rtmp_server_h_
#define _rtmp_server_h_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rtmp_server_handler_t
{
	/// network implemention
	/// @return >0-sent bytes, <0-error
	int (*send)(void* param, const void* data, size_t bytes);

	/// @return 0-ok, other-error
	//int (*oncreate_stream)(void* param, uint32_t* stream_id);
	//int (*ondelete_stream)(void* param, uint32_t stream_id);

	void (*onerror)(void* param, int code, const char* msg);

	// pull(server -> client)
	int (*onplay)(void* param, const char* app, const char* stream, double start, double duration, uint8_t reset);
	int (*onpause)(void* param, int pause, uint32_t ms);
	int (*onseek)(void* param, uint32_t ms);

	// push(client -> server)
	///@param[in] type: live/record/append
	int (*onpublish)(void* param, const char* app, const char* stream, const char* type);
	///@param[in] data FLV VideoTagHeader + AVCVIDEOPACKET: AVCDecoderConfigurationRecord(ISO 14496-15) / One or more NALUs(four-bytes length + NALU)
	int (*onvideo)(void* param, const void* data, size_t bytes, uint32_t timestamp);
	///@param[in] data FLV AudioTagHeader + AACAUDIODATA: AudioSpecificConfig(14496-3) / Raw AAC frame data in UI8
	int (*onaudio)(void* param, const void* data, size_t bytes, uint32_t timestamp);
};

void* rtmp_server_create(void* param, const struct rtmp_server_handler_t* handler);

void rtmp_server_destroy(void* rtmp);

int rtmp_server_getstate(void* rtmp);

/// @param[in] rtmp rtmp_server_create instance
/// @param[in] data rtmp chunk stream data
/// @param[in] bytes data length
/// @return 0-ok, other-error
int rtmp_server_input(void* rtmp, const uint8_t* data, size_t bytes);

/// send audio/video data(VOD only)
/// @param[in] rtmp rtmp_server_create instance
int rtmp_server_send_audio(void* rtmp, const void* data, size_t bytes, uint32_t timestamp);
int rtmp_server_send_video(void* rtmp, const void* data, size_t bytes, uint32_t timestamp);
int rtmp_server_send_metadata(void* rtmp, const void* data, size_t bytes);

#ifdef __cplusplus
}
#endif
#endif /* !_rtmp_server_h_ */

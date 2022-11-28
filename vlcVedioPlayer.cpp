// vlcVedioPlayer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "vlc.h"
//将unicode转换为utf-8的编码
std::string Unicode2Utf8(const std::wstring& strIn)
{
	std::string str;
	//获取转义以后的长度的length
	int length = ::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), NULL, 0, NULL, NULL);
	//设置str的length
	str.resize(length + 1);
	::WideCharToMultiByte(CP_UTF8, 0, strIn.c_str(), strIn.size(), (LPSTR)str.c_str(), length, NULL, NULL);
	return str;
}
int main()
{
	int argc = 1;
	char* argv[2];
	//忽略配置
	argv[0] =(char*)"--ignore-config";
	//拿到vlc实例
	libvlc_instance_t*vlc_ins = libvlc_new(argc,argv);
	//拿到media实例
	//std::wstring str = L"foreman.mp4";//获取道unicode编码的string
	libvlc_media_t*media = libvlc_media_new_path(vlc_ins,"vlcTest.mp4");
	//media = libvlc_media_new_location(vlc_ins, "files:///E:\\Windows_MyselfProject\\vlcVedioPlayer\\foreman.mp4");
	//获得player
	libvlc_media_player_t* player = libvlc_media_player_new_from_media(media);

	do {
		int ret = libvlc_media_player_play(player);
		if (ret == -1)
		{
			std::cout << "error player" << std::endl;
			break;
		}
		//预防音量获取失败
		int vol = -1;
		while (vol == -1)
		{
			Sleep(200);
			vol = libvlc_audio_get_volume(player);
		}
		Sleep(500);//休眠300毫秒，必须media解析加载完成才会有下面这些参数
		//获取音量
		
		printf("volume:%d\n", vol);
		//设置音量
		//10表示的是10%比例的正常，100表示正常，0表示静音
		system("pause");
		//缩小音量
		libvlc_audio_set_volume(player, 10);
		system("pause");
		//放大音量（做好心理准备）
		libvlc_audio_set_volume(player, 100);
		//拿长度（其实是视频的毫秒数）
		libvlc_time_t timel = libvlc_media_player_get_length(player);
		printf("%02d:%02d:%02d:%03d\r\n", int(timel / 3600000), int((timel / 60000) % 60), int((timel / 1000) % 60),int(timel % 1000));
		int width = libvlc_video_get_width(player);
		int height = libvlc_video_get_height(player);
		printf("width=%d,height=%d\r\n", width, height);
		//包含头文件#include <conio.h>
		while (!_kbhit()) {//输入不为空
			//获取视频播放进度
			printf("%f%%\r", 100.0*libvlc_media_player_get_position(player));//"\r"“回车”，告诉打字机把打印头定位在左边界；
			Sleep(500);
		}
		//把刚刚退出来的控制符吃掉
		getchar();
		//暂停player（暂停播放视频）
		libvlc_media_player_pause(player);
		system("pause");
		//继续播放视频
		libvlc_media_player_play(player);
		system("pause");
		//不播放视频
		libvlc_media_player_stop(player);
	} while (0);

	//释放播放器
	libvlc_media_player_release(player);
	//释放媒体
	libvlc_media_release(media);
	//释放实例
	libvlc_release(vlc_ins);
	return 0;
}


#pragma once

#ifndef MATXDLL_H
#define MATXDLL_H

#ifdef MATXDLL_EXPORTS
#define MATXDLL_API __declspec(dllexport)
#else
#define MATXDLL_API __declspec(dllimport)
#endif

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/unordered_map.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
	namespace matx {
		using namespace std;
		using boost::asio::ip::udp;

		enum MATXDLL_API config_props {
			inet_mode = 0,
			conn_mode = 1
		};

		enum MATXDLL_API sensor_conn_mode {
			Station = 10,
			AP = 11
		};

		enum MATXDLL_API sensor_inet_mode {
			Cable = 20,
			WiFi = 21
		};

		enum MATXDLL_API sensor_status {
			Unknown = 0,
			Error = 1,
			Ready = 2,
			Connected = 3,
			Started = 4,
			Stopped = 5,
			Disconnected = 6,
			Info = 7
		};

		struct MATXDLL_API MatInfo {
			unsigned int short_num;
			unsigned char addr[2];
			string serial_num;
			unsigned char mac[6];
			int software_version;
			int firmware_version;
			string ip;
			unsigned short port = 1002;
			unsigned short sampling;
			unsigned short adc;
			string model;
		};
		class MATXDLL_API MatDataHandler
		{
		public:
			virtual void handle_data_callback_42(unsigned int short_num, int raw_data[][42]) = 0;
			virtual void handle_data_callback_48(unsigned int short_num, int raw_data[][48]) = 0;
			virtual void handle_data_callback(unsigned int short_num, int raw_data[32][32]) = 0;
			virtual void handle_query_callback(boost::shared_ptr<MatInfo> matInfo) = 0;
			virtual void handle_status_notified(unsigned int short_num, sensor_status status) = 0;
			virtual ~MatDataHandler() {

			}
		};

		void MATXDLL_API init_mat_service(boost::shared_ptr<MatDataHandler> handler,
			boost::unordered_map<config_props, UINT> config);

		void MATXDLL_API start_service();
		void MATXDLL_API stop_service();

		void MATXDLL_API connect_by_num(UINT short_num);
		void MATXDLL_API connect_by_ip(std::string remote_point);
		void MATXDLL_API get_device_info(UINT short_num);
		void MATXDLL_API start_sampling(UINT short_num);
		void MATXDLL_API stop_sampling(UINT short_num);
		void MATXDLL_API update_rate(UINT short_num,UINT rate);
	}
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //MATXDLL_H
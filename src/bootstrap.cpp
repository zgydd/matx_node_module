#pragma warning(disable:4996)
#include <node.h>
#include <math.h>
#include <thread>
#include "MatXDll.h"
#include <Windows.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
namespace matxBridge {
	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Null;
	using v8::Object;
	using v8::Value;
	using v8::Persistent;
	using v8::String;
	using namespace::std;
	using namespace boost::unordered;
	boost::asio::io_service io;
	int _SOURCE_B_3232_[32][32];
	int _SOURCE_B_2842_[28][42];
	int _SOURCE_G_3248_[32][48];
	double extended_data_b_2842[28][42];
	double extended_data_b_3232[32][64];
	double extended_data_g_3248[32][96];
	int _UNIT_ = 0;
	int _STATUS_ = -1;
	int _REFRESHED_ = 0;
	int _SHORTNUMBER_ = -1;
	int _MULTIPLE_ = 2;
	thread *pBackTh;
	string _CONNECTIP_ = "192.168.0.57";
	class MatxHandler :public matx::MatDataHandler
	{
	public:
		void handle_data_callback(unsigned int short_num, int raw_data[32][32]) {
			//cout << "--" << raw_data[10][20] << endl;
			if (_SHORTNUMBER_ <= 0) _SHORTNUMBER_ = short_num;
			_STATUS_ = 66;
			_REFRESHED_ = 1;
			for (int i = 0; i < 32; i++) {
				for (int j = 0; j < 32; j++) {
					_SOURCE_B_3232_[j][i] = raw_data[i][j];
				}
			}
		};
		void handle_data_callback_48(unsigned int short_num, int raw_data[][48]) {
			if (_SHORTNUMBER_ <= 0) _SHORTNUMBER_ = short_num;
			_STATUS_ = 67;
			_REFRESHED_ = 1;
			for (int i = 0; i < 32; i++) {
				for (int j = 0; j < 48; j++) {
					_SOURCE_G_3248_[i][j] = raw_data[i][j];
				}
			}
		};
		void handle_data_callback_42(unsigned int short_num, int raw_data[][42]) {
			if (_SHORTNUMBER_ <= 0) _SHORTNUMBER_ = short_num;
			_STATUS_ = 68;
			_REFRESHED_ = 1;
			for (int i = 0; i < 28; i++) {
				for (int j = 0; j < 42; j++) {
					_SOURCE_B_2842_[i][j] = raw_data[i][j];
				}
			}
		};
		/*
		void handle_update_callback(unsigned int short_num, unsigned int result) {
			if (_SHORTNUMBER_ <= 0) _SHORTNUMBER_ = short_num;
			//cout << "updated:" << result << endl;
		};
		*/
		void handle_query_callback(boost::shared_ptr<matx::MatInfo> matInfo) {
			//cout << "serial_num:" << matInfo->adc;
		};
		void handle_status_notified(unsigned int short_num, matx::sensor_status status) {
			//cout << "num:" << short_num << ",status:" << status << endl;
			_STATUS_ = status;
			_SHORTNUMBER_ = short_num;
			if (status == matx::Ready) {
				matx::connect_by_ip(_CONNECTIP_);
			}
			else if (status == matx::Connected) {
				matx::start_sampling(short_num);
			}
			else if (status == matx::Stopped) {
				matx::get_device_info(short_num);
			}
		};
	};

	void extended_B_3232_data() {
		switch(_MULTIPLE_) {
			case 1:
				for (int i = 0; i < 32; i++) {
					int srcIdx = 0;
					for (int j = 0; j < 32; j++) {
						if (_SOURCE_B_3232_[srcIdx][i] > 18) {
							double pressure_number;
							switch (_UNIT_)
							{
							case 0:
								pressure_number =_SOURCE_B_3232_[srcIdx][i];
								break;
							case 1:
								pressure_number = round(((double)(_SOURCE_B_3232_[srcIdx][i] / 2.343)) * 100) / 100;
								break;
							default:
								pressure_number = round(((double)(4 * _SOURCE_B_3232_[srcIdx][i] / 70.29)) * 100) / 100;
								break;
							}
							extended_data_b_3232[i][j] = pressure_number;
							//extended_data_b_3232[i][j + 1] = pressure_number;
						}
						else {
							extended_data_b_3232[i][j] = 0;
							//extended_data_b_3232[i][j + 1] = 0;
						}
						srcIdx++;
					}
				}
				break;
			case 2:
			default:
				for (int i = 0; i < 32; i++) {
					int srcIdx = 0;
					for (int j = 0; j < 64; j += 2) {
						if (_SOURCE_B_3232_[srcIdx][i] > 18) {
							double pressure_number;
							switch (_UNIT_)
							{
							case 0:
								pressure_number =_SOURCE_B_3232_[srcIdx][i];
								break;
							case 1:
								pressure_number = round(((double)(_SOURCE_B_3232_[srcIdx][i] / 2.343)) * 100) / 100;
								break;
							default:
								pressure_number = round(((double)(4 * _SOURCE_B_3232_[srcIdx][i] / 70.29)) * 100) / 100;
								break;
							}
							extended_data_b_3232[i][j] = pressure_number;
							extended_data_b_3232[i][j + 1] = pressure_number;
						}
						else {
							extended_data_b_3232[i][j] = 0;
							extended_data_b_3232[i][j + 1] = 0;
						}
						srcIdx++;
					}
				}
				break;
		}
	}
	void extended_G_3248_data() {
		for (int i = 0; i < 32; i++) {
			int srcIdx = 0;
			for (int j = 0; j < 96; j += 2) {
				if (_SOURCE_G_3248_[i][srcIdx] > 18) {
					double pressure_number;
					switch (_UNIT_)
					{
					case 0:
						pressure_number =_SOURCE_G_3248_[i][srcIdx];
						break;
					case 1:
						pressure_number = round(((double)(_SOURCE_G_3248_[i][srcIdx] / 2.343)) * 100) / 100;
						break;
					default:
						pressure_number = round(((double)(4 * _SOURCE_G_3248_[i][srcIdx] / 70.29)) * 100) / 100;
						break;
					}
					extended_data_g_3248[i][j] = pressure_number;
					extended_data_g_3248[i][j + 1] = pressure_number;
				}
				else {
					extended_data_g_3248[i][j] = 0;
					extended_data_g_3248[i][j + 1] = 0;
				}
				srcIdx++;
			}
		}		
	}
	void extended_B_2842_data() {
		for (int i = 0; i < 28; i++) {
			for (int j = 0; j < 42; j++) {
				if (_SOURCE_B_2842_[i][j] > 18) {
					double pressure_number;
					switch (_UNIT_)
					{
					case 0:
						pressure_number =_SOURCE_B_2842_[i][j];
						break;
					case 1:
						pressure_number = round(((double)(_SOURCE_B_2842_[i][j] / 2.343)) * 100) / 100;
						break;
					default:
						pressure_number = round(((double)(4 * _SOURCE_B_2842_[i][j] / 70.29)) * 100) / 100;
						break;
					}
					extended_data_b_2842[i][j] = pressure_number;
				}
				else {
					extended_data_b_2842[i][j] = 0;
				}
			}
		}		
	}
	void start_thread() {
		//cout << "Test MatX here......";

		boost::unordered_map<matx::config_props, UINT> my_config;
		my_config.insert(make_pair(matx::inet_mode, 20));//20 means Cable
		my_config.insert(make_pair(matx::conn_mode, 10));//10 means Station
		boost::shared_ptr<MatxHandler> handler(new MatxHandler());

		matx::init_mat_service(handler, my_config);
		matx::start_service();
	}
	void _set_ip(const FunctionCallbackInfo<Value>& args) {
		if(args.Length() < 1 || !args[0]->IsString()) return;
		String::Utf8Value utf8_value(args[0]);
		_CONNECTIP_ = string(*utf8_value);
	}
	void _set_multiple(const FunctionCallbackInfo<Value>& args) {
		if(args.Length() < 1 ||!args[0]->IsNumber()) return;
		else _MULTIPLE_ = args[0]->NumberValue();		
	}
	void _start_mattress_service(const FunctionCallbackInfo<Value>& args) {
		pBackTh = new thread(start_thread);
		pBackTh->detach();
	}
	void _stop_mattress_service(const FunctionCallbackInfo<Value>& args) {
		_UNIT_ = 0;
		_STATUS_ = -1;
		_REFRESHED_ = 0;
		_SHORTNUMBER_ = -1;
		_CONNECTIP_ = "192.168.0.57";
		matx::stop_service();
		free(pBackTh);
	}
	void _get_mattress_status(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set(_STATUS_);
	}
	void _check_mattress_refreshed(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set(_REFRESHED_);
	}
	void _get_mattress_number(const FunctionCallbackInfo<Value>& args) {
		args.GetReturnValue().Set(_SHORTNUMBER_);
	}
	void _reconnect_mattress(const FunctionCallbackInfo<Value>& args) {
		matx::connect_by_ip(_CONNECTIP_);
	}
	void _start_sampling(const FunctionCallbackInfo<Value>& args) {
		if(args.Length() < 1 ||!args[0]->IsNumber()) matx::start_sampling(_SHORTNUMBER_);
		else matx::start_sampling(args[0]->NumberValue());
	}
	void _get_matrix_B_2842(const FunctionCallbackInfo<Value>& args) {
		extended_B_2842_data();
		Isolate* isolate = args.GetIsolate();
		v8::HandleScope handleScope(isolate);

		Local<v8::Array> outputData = v8::Array::New(isolate, 28);
		if (outputData.IsEmpty()) return;

		for (int i = 0; i < 28; i++) {
			Local<v8::Array> row = v8::Array::New(isolate, 42);
			for (int j = 0; j < 42; j++) {
				Local<v8::Integer> value = v8::Integer::New(isolate, extended_data_b_2842[i][j]);
				row->Set(j, value);
			}
			outputData->Set(i, row);
		}
		args.GetReturnValue().Set(outputData);
		_REFRESHED_ = 0;
	}
	void _get_matrix_B_3232(const FunctionCallbackInfo<Value>& args) {
		extended_B_3232_data();
		Isolate* isolate = args.GetIsolate();
		v8::HandleScope handleScope(isolate);

		Local<v8::Array> outputData = v8::Array::New(isolate, 32);
		if (outputData.IsEmpty()) return;

		for (int i = 0; i < 32; i++) {
			Local<v8::Array> row = v8::Array::New(isolate, 64);
			for (int j = 0; j < 64; j++) {
				Local<v8::Integer> value = v8::Integer::New(isolate, extended_data_b_3232[i][j]);
				row->Set(j, value);
			}
			outputData->Set(i, row);
		}
		args.GetReturnValue().Set(outputData);
		_REFRESHED_ = 0;
	}
	void _get_matrix_G_3248(const FunctionCallbackInfo<Value>& args) {
		extended_G_3248_data();
		Isolate* isolate = args.GetIsolate();
		v8::HandleScope handleScope(isolate);

		Local<v8::Array> outputData = v8::Array::New(isolate, 32);
		if (outputData.IsEmpty()) return;

		for (int i = 0; i < 32; i++) {
			Local<v8::Array> row = v8::Array::New(isolate, 96);
			for (int j = 0; j < 96; j++) {
				Local<v8::Integer> value = v8::Integer::New(isolate, extended_data_g_3248[i][j]);
				row->Set(j, value);
			}
			outputData->Set(i, row);
		}
		args.GetReturnValue().Set(outputData);
		_REFRESHED_ = 0;
	}
	void Init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "set_ip", _set_ip);
		NODE_SET_METHOD(exports, "set_multiple", _set_multiple);
		NODE_SET_METHOD(exports, "start_mattress_service", _start_mattress_service);
		NODE_SET_METHOD(exports, "stop_mattress_service", _stop_mattress_service);
		NODE_SET_METHOD(exports, "get_mattress_number", _get_mattress_number);
		NODE_SET_METHOD(exports, "get_mattress_status", _get_mattress_status);
		NODE_SET_METHOD(exports, "reconnect_mattress", _reconnect_mattress);
		NODE_SET_METHOD(exports, "start_sampling", _start_sampling);
		NODE_SET_METHOD(exports, "check_mattress_refreshed", _check_mattress_refreshed);
		NODE_SET_METHOD(exports, "get_matrix_B_2842", _get_matrix_B_2842);		
		NODE_SET_METHOD(exports, "get_matrix_B_3232", _get_matrix_B_3232);
		NODE_SET_METHOD(exports, "get_matrix_G_3248", _get_matrix_G_3248);
	}
	NODE_MODULE(matxBridge, Init)
}  // namespace matxBridge
#ifdef __cplusplus
}
#endif /* __cplusplus */
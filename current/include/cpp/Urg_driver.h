#ifndef QRK_URG_DRIVER_H
#define QRK_URG_DRIVER_H

/*!
  \file
  \~japanese
  \brief URG ドライバ
  \~english
  \brief URG driver
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include <memory>
#include <string>
#include <cstdint>
#include "Lidar.h"

namespace qrk
{
    //! \~japanese URG ドライバ  \~english URG driver
    class Urg_driver : public Lidar
    {
    public:
        enum
        {
            Default_baudrate = 115200,
            Default_port = 10940,
            Infinity_times = -1,
        };

        //! \~japanese URG ドライバを構築する  \~english Constructs the URG driver
        Urg_driver(void);
        //! \~japanese URG ドライバを破棄する  \~english Destroys the URG driver
        virtual ~Urg_driver(void);

        //! \~japanese 接続可能なポートを検索する  \~english Finds available ports
        static std::vector<std::string> find_ports(void);
        //! \~japanese URG センサを検索し、判定結果を返す  \~english Finds sensors and returns the detection results
        static std::vector<std::string> find_ports(std::vector<int32_t> &
                                                       is_urg_ports);
        //! \~japanese 最後に発生したエラーを示す文字列を返す  \~english Returns the last error message
        const char *what(void) const;

        //! \~japanese 指定したデバイスに接続する  \~english Connects to the specified device
        bool open(const char *device_name, int32_t baudrate = Default_baudrate,
                  connection_type_t type = Serial);
        //! \~japanese センサとの接続を切断する  \~english Disconnects from the sensor
        void close(void);
        //! \~japanese 接続状態を返す  \~english Returns whether the sensor is open
        bool is_open(void) const;

        //! \~japanese 通信のタイムアウト時間を設定する [msec]  \~english Sets the communication timeout [msec]
        void set_timeout_msec(int32_t msec);

        //! \~japanese レーザを点灯する  \~english Turns the laser on
        bool laser_on(void);
        //! \~japanese レーザを消灯する  \~english Turns the laser off
        bool laser_off(void);

        //! \~japanese センサを再起動する  \~english Reboots the sensor
        bool reboot(void);

        //! \~japanese センサをスリープ状態にする  \~english Puts the sensor to sleep
        void sleep(void);
        //! \~japanese センサを復帰させる  \~english Wakes the sensor
        void wakeup(void);
        //! \~japanese センサが安定状態か返す  \~english Returns whether the sensor is stable
        bool is_stable(void);

        //! \~japanese データ取得の開始  \~english Starts data measurement process
        bool start_measurement(measurement_type_t type = Distance,
                               int32_t scan_times = Infinity_times,
                               int32_t skip_scan = 0);

        //! \~japanese チェックサムエラーを無視するか設定する  \~english Sets whether checksum errors are ignored
        void set_ignore_checkSumError(bool ignore = true);

        //! \~japanese 受信データの受け取り  \~english Receives measurement data
        //! \~japanese 距離データを取得する  \~english Gets distance data
        bool get_distance(std::vector<int32_t> &data, int32_t *time_stamp = NULL);
        //! \~japanese 距離と強度データを取得する  \~english Gets distance and intensity data
        bool get_distance_intensity(std::vector<int32_t> &data,
                                    std::vector<unsigned short> &intensity,
                                    int32_t *time_stamp = NULL);

        //! \~japanese 距離と IO データを取得する  \~english Gets distance and IO data
        bool get_distance_io(std::vector<int32_t> &data, std::vector<int32_t> &io, int32_t *time_stamp = NULL);

        //! \~japanese 距離、強度、IO データを取得する  \~english Gets distance, intensity and IO data
        bool get_distance_intensity_io(std::vector<int32_t> &data,
                                       std::vector<unsigned short> &intensity,
                                       std::vector<int32_t> &io,
                                       int32_t *time_stamp = NULL);

        //! \~japanese マルチエコー距離データを取得する  \~english Gets multiecho distance data
        bool get_multiecho(std::vector<int32_t> &data_multi,
                           int32_t *time_stamp = NULL);

        //! \~japanese マルチエコー距離と強度データを取得する  \~english Gets multiecho distance and intensity data
        bool get_multiecho_intensity(std::vector<int32_t> &data_multiecho,
                                     std::vector<unsigned short> &
                                         intensity_multiecho,
                                     int32_t *time_stamp = NULL);

        //! \~japanese 計測範囲とスキップ幅を設定する  \~english Sets the measurement range and skip step
        bool set_scanning_parameter(int32_t first_step, int32_t last_step,
                                    int32_t skip_step = 1);

        //! \~japanese データ取得の中断  \~english Stops data measurement process
        void stop_measurement(void);

        //! \~japanese タイムスタンプの同期  \~english Synchronization of timestamps
        //! \~japanese タイムスタンプモードを開始する  \~english Starts timestamp mode
        bool start_time_stamp_mode(void);
        //! \~japanese タイムスタンプモードを停止する  \~english Stops timestamp mode
        bool stop_time_stamp_mode(void);
        //! \~japanese センサのタイムスタンプを設定する  \~english Sets the sensor timestamp
        bool set_sensor_time_stamp(int32_t time_stamp);
        //! \~japanese センサのタイムスタンプを取得する  \~english Gets the sensor timestamp
        int32_t get_sensor_time_stamp(void);

        //! \~japanese 角度変換  \~english Angle conversion functions
        //! \~japanese インデックスを角度(radian)に変換する  \~english Converts an index to an angle in radians
        double index2rad(int32_t index) const;
        //! \~japanese インデックスを角度(degree)に変換する  \~english Converts an index to an angle in degrees
        double index2deg(int32_t index) const;
        //! \~japanese 角度(radian)をインデックスに変換する  \~english Converts an angle in radians to an index
        int32_t rad2index(double radian) const;
        //! \~japanese 角度(radian)をインデックスに変換する  \~english Converts an angle in radians to an index
        int32_t deg2index(double degree) const;
        //! \~japanese 角度(degree)をインデックスに変換する  \~english Converts an angle in degrees to an index
        int32_t rad2step(double radian) const;
        //! \~japanese 角度(radian)を step に変換する  \~english Converts an angle in radians to a step
        int32_t deg2step(double degree) const;
        //! \~japanese 角度(degree)を step に変換する  \~english Converts an angle in degrees to a step
        double step2rad(int32_t step) const;
        //! \~japanese step を角度(radian)に変換する  \~english Converts a step to an angle in radians
        double step2deg(int32_t step) const;
        //! \~japanese step を角度(degree)に変換する  \~english Converts a step to an angle in degrees
        int32_t step2index(int32_t step) const;
        //! \~japanese step をインデックスに変換する  \~english Converts a step to an index

        //! \~japanese 計測可能な最小 step を返す  \~english Returns the minimum measurable step
        int32_t min_step(void) const;
        //! \~japanese 計測可能な最大 step を返す  \~english Returns the maximum measurable step
        int32_t max_step(void) const;
        //! \~japanese 距離の最小値を返す [mm]  \~english Returns the minimum distance [mm]
        int32_t min_distance(void) const;
        //! \~japanese 距離の最大値を返す [mm]  \~english Returns the maximum distance [mm]
        int32_t max_distance(void) const;
        //! \~japanese 1スキャンの時間を返す [usec]  \~english Returns the time for one scan [usec]
        int32_t scan_usec(void) const;
        //! \~japanese 取得可能なデータ数の最大値を返す  \~english Returns the maximum data size
        int32_t max_data_size(void) const;
        //! \~japanese マルチエコー数の最大値を返す  \~english Returns the maximum echo size
        int32_t max_echo_size(void) const;
        //! \~japanese IO データ数の最大値を返す  \~english Returns the maximum IO size
        int32_t max_io_size(void) const;

        //! \~japanese センサの製品名を返す  \~english Returns the product type
        const char *product_type(void) const;
        //! \~japanese ファームウェアバージョンを返す  \~english Returns the firmware version
        const char *firmware_version(void) const;
        //! \~japanese センサのシリアル ID を返す  \~english Returns the serial ID
        const char *serial_id(void) const;
        //! \~japanese センサの状態を返す  \~english Returns the sensor status
        const char *status(void) const;
        //! \~japanese センサの動作状態を返す  \~english Returns the sensor state
        const char *state(void) const;

        //! \~japanese 生データを送信する  \~english Writes raw data
        int32_t raw_write(const char *data, size_t data_size);
        //! \~japanese 生データを受信する  \~english Reads raw data
        int32_t raw_read(char *data, size_t max_data_size, int32_t timeout);
        //! \~japanese 改行まで生データを受信する  \~english Reads raw data until end-of-line
        int32_t raw_readline(char *data, size_t max_data_size, int32_t timeout);
        //! \~japanese C API の URG 管理構造体を取得する  \~english Returns the C API URG structure
        void *raw_urg(void);
        //! \~japanese 計測タイプを設定する  \~english Sets the measurement type
        void set_measurement_type(measurement_type_t type);

    private:
        Urg_driver(const Urg_driver &rhs);
        Urg_driver &operator=(const Urg_driver &rhs);

        struct pImpl;
        std::unique_ptr<pImpl> pimpl;
    };
}

#endif /* !QRK_URG_DRIVER_H */

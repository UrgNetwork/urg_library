#ifndef QRK_LIDAR_H
#define QRK_LIDAR_H

/*!
  \file
  \~japanese
  \brief Lidar インターフェース
  \~english
  \brief Lidar interface
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#include <vector>
#include <cstddef>
#include <cstdint>

namespace qrk
{
    //! \~japanese Lidar インターフェース  \~english Lidar interface
    class Lidar
    {
    public:
        typedef enum {
            Distance,            //!< \~japanese 距離  \~english Range
            Distance_intensity,  //!< \~japanese 距離 + 強度  \~english Distance (range) and intensity (strength)
            Multiecho,           //!< \~japanese マルチエコーの距離  \~english Multiecho distance
            Multiecho_intensity, //!< \~japanese マルチエコーの(距離 + 強度)  \~english Multiecho distance and intensity
            Distance_io,           //!< \~japanese 距離 + IO  \~english Distance (range) and io(input/output)
            Distance_intensity_io, //!< \~japanese 距離 + 強度 + IO  \~english Distance (range), intensity and io(input/output)
        } measurement_type_t;

        //! \~japanese 接続タイプ  \~english Connection types
        typedef enum
        {
            Serial,   //!< \~japanese シリアル、USB 接続  \~english Serial/USB connection
            Ethernet, //!< \~japanese イーサーネット接続  \~english Ethernet connection
        } connection_type_t;

        //! \~japanese 仮想デストラクタ  \~english Virtual destructor
        virtual ~Lidar(void)
        {
        }

        //! \~japanese 最後に発生したエラーを示す文字列を返す  \~english Returns the last error message
        virtual const char *what(void) const = 0;

        //! \~japanese センサに接続する  \~english Connects to the sensor
        virtual bool open(const char *device_name, int32_t baudrate,
                          connection_type_t type) = 0;
        //! \~japanese センサとの接続を切断する  \~english Disconnects from the sensor
        virtual void close(void) = 0;
        //! \~japanese 接続状態を返す  \~english Returns whether the sensor is open
        virtual bool is_open(void) const = 0;

        //! \~japanese 通信のタイムアウト時間を設定する [msec]  \~english Sets the communication timeout [msec]
        virtual void set_timeout_msec(int32_t msec) = 0;

        //! \~japanese レーザを点灯する  \~english Turns the laser on
        virtual bool laser_on(void) = 0;
        //! \~japanese レーザを消灯する  \~english Turns the laser off
        virtual bool laser_off(void) = 0;

        //! \~japanese センサを再起動する  \~english Reboots the sensor
        virtual bool reboot(void) = 0;
        //! \~japanese センサをスリープ状態にする  \~english Puts the sensor to sleep

        virtual void sleep(void) = 0;
        //! \~japanese センサを復帰させる  \~english Wakes the sensor
        virtual void wakeup(void) = 0;
        //! \~japanese センサが安定状態か返す  \~english Returns whether the sensor is stable
        virtual bool is_stable(void) = 0;

        //! \~japanese データ取得の開始  \~english Starts data measurement process
        virtual bool start_measurement(measurement_type_t type,
                                       int scan_times, int skip_scan) = 0;

        //! \~japanese 受信データの受け取り  \~english Receives measurement data
        //! \~japanese 距離データを取得する  \~english Gets distance data
        virtual bool get_distance(std::vector<long>& data,
                                  long *time_stamp) = 0;
        virtual bool get_distance_intensity(std::vector<long>& data,
                                            std::vector<unsigned short>&
                                  int32_t *time_stamp) = 0;
        //! \~japanese 距離と強度データを取得する  \~english Gets distance and intensity data
        virtual bool get_distance_intensity(std::vector<int32_t> &data,
                                            std::vector<unsigned short> &
                                            intensity,
                                            long *time_stamp) = 0;

        //! \~japanese マルチエコー距離データを取得する  \~english Gets multiecho distance data
        virtual bool get_multiecho(std::vector<int32_t> &data_multi,
                                   int32_t *time_stamp) = 0;

        //! \~japanese マルチエコー距離と強度データを取得する  \~english Gets multiecho distance and intensity data
        virtual bool get_multiecho_intensity(std::vector<int32_t> &data_multiecho,
                                             std::vector<unsigned short> &
                                             intensity_multiecho,
                                             long* time_stamp) = 0;

        //! \~japanese 計測範囲とスキップ幅を設定する  \~english Sets the measurement range and skip step
        virtual bool set_scanning_parameter(int32_t first_step, int32_t last_step,
                                            int32_t skip_step) = 0;

        //! \~japanese データ取得の中断  \~english Stops data measurement process
        virtual void stop_measurement(void) = 0;

        //! \~japanese タイムスタンプの同期  \~english Synchronization of timestamps
        //! \~japanese センサのタイムスタンプを設定する  \~english Sets the sensor timestamp
        virtual bool set_sensor_time_stamp(int32_t time_stamp) = 0;

        //! \~japanese 角度変換  \~english Angle conversion functions
        //! \~japanese インデックスを角度(radian)に変換する  \~english Converts an index to an angle in radians
        virtual double index2rad(int32_t index) const = 0;
        //! \~japanese インデックスを角度(degree)に変換する  \~english Converts an index to an angle in degrees
        virtual double index2deg(int32_t index) const = 0;
        //! \~japanese 角度(radian)をインデックスに変換する  \~english Converts an angle in radians to an index
        virtual int32_t rad2index(double radian) const = 0;
        //! \~japanese 角度(degree)をインデックスに変換する  \~english Converts an angle in degrees to an index
        virtual int32_t deg2index(double degree) const = 0;
        //! \~japanese 角度(radian)を step に変換する  \~english Converts an angle in radians to a step
        virtual int32_t rad2step(double radian) const = 0;
        //! \~japanese 角度(degree)を step に変換する  \~english Converts an angle in degrees to a step
        virtual int32_t deg2step(double degree) const = 0;
        //! \~japanese step を角度(radian)に変換する  \~english Converts a step to an angle in radians
        virtual double step2rad(int32_t step) const = 0;
        //! \~japanese step を角度(degree)に変換する  \~english Converts a step to an angle in degrees
        virtual double step2deg(int32_t step) const = 0;
        //! \~japanese step をインデックスに変換する  \~english Converts a step to an index
        virtual int32_t step2index(int32_t step) const = 0;

        //! \~japanese 計測可能な最小 step を返す  \~english Returns the minimum measurable step
        virtual int32_t min_step(void) const = 0;
        //! \~japanese 計測可能な最大 step を返す  \~english Returns the maximum measurable step
        virtual int32_t max_step(void) const = 0;
        //! \~japanese 距離の最小値を返す [mm]  \~english Returns the minimum distance [mm]
        virtual int32_t min_distance(void) const = 0;
        //! \~japanese 距離の最大値を返す [mm]  \~english Returns the maximum distance [mm]
        virtual int32_t max_distance(void) const = 0;
        //! \~japanese 1スキャンの時間を返す [usec]  \~english Returns the time for one scan [usec]
        virtual int32_t scan_usec(void) const = 0;
        //! \~japanese 取得可能なデータ数の最大値を返す  \~english Returns the maximum data size
        virtual int32_t max_data_size(void) const = 0;
        //! \~japanese マルチエコー数の最大値を返す  \~english Returns the maximum echo size
        virtual int32_t max_echo_size(void) const = 0;

        //! \~japanese センサの製品名を返す  \~english Returns the product type
        virtual const char *product_type(void) const = 0;
        //! \~japanese ファームウェアバージョンを返す  \~english Returns the firmware version
        virtual const char *firmware_version(void) const = 0;
        //! \~japanese センサのシリアル ID を返す  \~english Returns the serial ID
        virtual const char *serial_id(void) const = 0;
        //! \~japanese センサの状態を返す  \~english Returns the sensor status
        virtual const char *status(void) const = 0;
        //! \~japanese センサの動作状態を返す  \~english Returns the sensor state
        virtual const char *state(void) const = 0;

        //! \~japanese 生データを送信する  \~english Writes raw data
        virtual int32_t raw_write(const char *data, size_t data_size) = 0;
        //! \~japanese 生データを受信する  \~english Reads raw data
        virtual int32_t raw_read(char *data, size_t max_data_size, int32_t timeout) = 0;
        //! \~japanese 改行まで生データを受信する  \~english Reads raw data until end-of-line
        virtual int32_t raw_readline(char *data, size_t max_data_size,
                                     int32_t timeout) = 0;
    };
}

#endif /* !QRK_LIDAR_H */

#ifndef QRK_LIDAR_H
#define QRK_LIDAR_H

/*!
  \file
  \brief Lidar インターフェース

  \author Satofumi KAMIMURA

  $Id: lidar.h 1937 2010-10-25 01:12:49Z satofumi $
*/

#include <vector>
#include <cstddef>


namespace qrk
{
    //! Lidar インターフェース
    class Lidar
    {
    public:
        typedef enum {
            Distance,            /*!< 距離 */
            Distance_intensity,  /*!< 距離 + 強度 */
            Multiecho,           /*!< マルチエコーの距離 */
            Multiecho_intensity, /*!< マルチエコーの(距離 + 強度) */
        } measurement_type_t;

        typedef enum {
            Serial,
            Ethernet,
        } connection_type_t;

        virtual ~Lidar(void)
        {
        }

        virtual const char* what(void) const = 0;

        virtual bool open(const char* device_name, long baudrate,
                          connection_type_t type) = 0;
        virtual void close(void) = 0;
        virtual bool is_open(void) const = 0;

        virtual void set_timeout_msec(int msec) = 0;

        virtual bool laser_on(void) = 0;
        virtual bool laser_off(void) = 0;

        virtual void reboot(void) = 0;

        virtual void sleep(void) = 0;
        virtual void wakeup(void) = 0;
        virtual bool is_stable(void) = 0;

        // !!! データ取得の開始
        virtual bool start_measurement(measurement_type_t type,
                                       int scan_times, int skip_scan) = 0;

        // !!! 受信データの受け取り
        virtual bool get_distance(std::vector<long>& data,
                                  long *time_stamp) = 0;
        virtual bool get_distance_intensity(std::vector<long>& data,
                                            std::vector<unsigned short>&
                                            intensity,
                                            long *time_stamp) = 0;

        virtual bool get_multiecho(std::vector<long>& data_multi,
                                   long* time_stamp) = 0;

        virtual bool get_multiecho_intensity(std::vector<long>& data_multiecho,
                                             std::vector<unsigned short>&
                                             intensity_multiecho,
                                             long* time_stamp) = 0;

        virtual bool set_scanning_parameter(int first_step, int last_step,
                                            int skip_step) = 0;

        // !!! データ取得の中断
        virtual void stop_measurement(void) = 0;

        // !!! タイムスタンプの同期
        virtual bool set_sensor_time_stamp(long time_stamp) = 0;

        // !!! 角度変換
        virtual double index2rad(int index) const = 0;
        virtual double index2deg(int index) const = 0;
        virtual int rad2index(double radian) const = 0;
        virtual int deg2index(double degree) const = 0;
        virtual int rad2step(double radian) const = 0;
        virtual int deg2step(double degree) const = 0;
        virtual double step2rad(int step) const = 0;
        virtual double step2deg(int step) const = 0;
        virtual int step2index(int step) const = 0;

        virtual int min_step(void) const = 0;
        virtual int max_step(void) const = 0;
        virtual long min_distance(void) const = 0;
        virtual long max_distance(void) const = 0;
        virtual long scan_usec(void) const = 0;
        virtual int max_data_size(void) const = 0;
        virtual int max_echo_size(void) const = 0;

        virtual const char* product_type(void) const = 0;
        virtual const char* firmware_version(void) const = 0;
        virtual const char* serial_id(void) const = 0;
        virtual const char* status(void) const = 0;
        virtual const char* state(void) const = 0;

        virtual int raw_write(const char* data, size_t data_size) = 0;
        virtual int raw_read(char* data, size_t max_data_size, int timeout) = 0;
        virtual int raw_readline(char* data, size_t max_data_size,
                                 int timeout) = 0;
    };
}

#endif /* !QRK_LIDAR_H */

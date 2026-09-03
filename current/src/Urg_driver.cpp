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

#include "Urg_driver.h"
#include "ticks.h"
extern "C"
{
#include "urg_sensor.h"
#include "urg_utils.h"
#include "urg_serial_utils.h"
#include "urg_errno.h"
#include "urg_debug.h"
}

using namespace qrk;
using namespace std;

struct Urg_driver::pImpl
{
    urg_t urg_;
    measurement_type_t last_measure_type_;
    int32_t time_stamp_offset_;

    string product_type_;
    string firmware_version_;
    string serial_id_;
    string status_;
    string state_;

    pImpl(void)
        : last_measure_type_(Distance), time_stamp_offset_(0)
    {
        urg_t_initialize(&urg_);
    }

    void adjust_time_stamp(int32_t *time_stamp)
    {
        if (time_stamp)
        {
            *time_stamp += time_stamp_offset_;
        }
    }
};

Urg_driver::Urg_driver(void) : pimpl(new pImpl)
{
}

Urg_driver::~Urg_driver(void)
{
    close();
}

std::vector<std::string> Urg_driver::find_ports(void)
{
    vector<int32_t> dummy_is_urg_port;
    return find_ports(dummy_is_urg_port);
}

std::vector<std::string> Urg_driver::find_ports(std::vector<int32_t> &is_urg_ports)
{
    vector<string> found_ports;

    is_urg_ports.clear();
    int32_t n = urg_serial_find_port();
    for (int32_t i = 0; i < n; ++i)
    {
        found_ports.push_back(urg_serial_port_name(i));
        is_urg_ports.push_back(urg_serial_is_urg_port(i));
    }
    return found_ports;
}

const char *Urg_driver::what(void) const
{
    return urg_error(&pimpl->urg_);
}

bool Urg_driver::open(const char *device_name, int32_t baudrate,
                      connection_type_t type)
{
    close();
    pimpl->product_type_.clear();
    pimpl->firmware_version_.clear();
    pimpl->serial_id_.clear();

    urg_connection_type_t connection_type =
        (type == Ethernet) ? URG_ETHERNET : URG_SERIAL;
    int ret = urg_open(&pimpl->urg_, connection_type, device_name, baudrate);
    if (ret < 0)
    {
        return false;
    }

    return true;
}

void Urg_driver::close(void)
{
    if (is_open())
    {
        urg_close(&pimpl->urg_);
    }
}

bool Urg_driver::is_open(void) const
{
    return pimpl->urg_.is_active;
}

void Urg_driver::set_timeout_msec(int32_t msec)
{
    urg_set_timeout_msec(&pimpl->urg_, msec);
}

bool Urg_driver::laser_on(void)
{
    int ret = urg_laser_on(&pimpl->urg_);
    return (ret < 0) ? false : true;
}

bool Urg_driver::laser_off(void)
{
    int ret = urg_laser_off(&pimpl->urg_);
    return (ret < 0) ? false : true;
}

bool Urg_driver::reboot(void)
{
    return urg_reboot(&pimpl->urg_) == URG_NO_ERROR;
}

void Urg_driver::sleep(void)
{
    urg_sleep(&pimpl->urg_);
}

void Urg_driver::wakeup(void)
{
    urg_wakeup(&pimpl->urg_);
}

bool Urg_driver::is_stable(void)
{
    return urg_is_stable(&pimpl->urg_) ? true : false;
}

bool Urg_driver::start_measurement(measurement_type_t type,
                                   int32_t scan_times, int32_t skip_scan)
{
    typedef struct
    {
        urg_measurement_type_t c_type;
        measurement_type_t type;
    } type_table_t;

    type_table_t type_table[] = {
        {URG_DISTANCE, Distance},
        {URG_DISTANCE_INTENSITY, Distance_intensity},
        {URG_MULTIECHO, Multiecho},
        {URG_MULTIECHO_INTENSITY, Multiecho_intensity},
        {URG_DISTANCE_IO, Distance_io},
        {URG_DISTANCE_INTENSITY_IO, Distance_intensity_io},
    };

    size_t n = sizeof(type_table) / sizeof(type_table[0]);
    for (size_t i = 0; i < n; ++i)
    {
        const type_table_t *p = &type_table[i];
        if (type == p->type)
        {
            int ret =
                urg_start_measurement(&pimpl->urg_, p->c_type, scan_times, skip_scan, pimpl->urg_.ignore_checkSumError);
            if (ret == URG_NO_ERROR)
            {
                pimpl->last_measure_type_ = type;
            }
            return (ret == URG_NO_ERROR) ? true : false;
        }
    }

    return false;
}

void Urg_driver::set_ignore_checkSumError(bool ignore) { pimpl->urg_.ignore_checkSumError = ignore ? 1 : 0; }

bool Urg_driver::get_distance(std::vector<int32_t> &data, int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Distance)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // \~japanese 最大サイズを確保し、そこにデータを格納する
    // \~english Allocates memory for the maximum size and stores data there
    data.resize(max_data_size());
    int ret = urg_get_distance(&pimpl->urg_, &data[0], time_stamp);
    if (ret > 0)
    {
        data.resize(ret);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::get_distance_intensity(std::vector<int32_t> &data,
                                        std::vector<unsigned short> &intensity,
                                        int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Distance_intensity)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // \~japanese 最大サイズを確保し、そこにデータを格納する
    // \~english Allocates memory for the maximum size and stores data there
    size_t data_size = max_data_size();
    data.resize(data_size);
    intensity.resize(data_size);
    int ret = urg_get_distance_intensity(&pimpl->urg_,
                                         &data[0], &intensity[0], time_stamp);
    if (ret > 0)
    {
        data.resize(ret);
        intensity.resize(ret);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::get_distance_io(std::vector<int32_t> &data, std::vector<int32_t> &io, int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Distance_io)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // 最大サイズを確保し、そこにデータを格納する
    data.resize(max_data_size());
    io.resize(max_io_size());
    int ret = urg_get_distance_io(&pimpl->urg_, &data[0], &io[0], time_stamp);
    if (ret > 0)
    {
        data.resize(ret);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::get_distance_intensity_io(std::vector<int32_t> &data,
                                           std::vector<unsigned short> &intensity,
                                           std::vector<int32_t> &io,
                                           int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Distance_intensity_io)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // 最大サイズを確保し、そこにデータを格納する
    size_t data_size = max_data_size();
    data.resize(data_size);
    intensity.resize(data_size);
    io.resize(max_io_size());
    int ret = urg_get_distance_intensity_io(&pimpl->urg_, &data[0], &intensity[0], &io[0], time_stamp);
    if (ret > 0)
    {
        data.resize(ret);
        intensity.resize(ret);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::get_multiecho(std::vector<int32_t> &data_multiecho,
                               int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Multiecho)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // \~japanese 最大サイズを確保し、そこにデータを格納する
    // \~english Allocates memory for the maximum size and stores data there
    size_t echo_size = max_echo_size();
    size_t data_size = max_data_size() * echo_size;
    data_multiecho.resize(data_size);
    int ret = urg_get_multiecho(&pimpl->urg_, &data_multiecho[0], time_stamp);
    if (ret > 0)
    {
        data_multiecho.resize(ret * echo_size);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::get_multiecho_intensity(std::vector<int32_t> &data_multiecho,
                                         std::vector<unsigned short> &
                                             intensity_multiecho,
                                         int32_t *time_stamp)
{
    if (pimpl->last_measure_type_ != Multiecho_intensity)
    {
        pimpl->urg_.last_errno = URG_MEASUREMENT_TYPE_MISMATCH;
        return false;
    }

    // \~japanese 最大サイズを確保し、そこにデータを格納する
    // \~english Allocates memory for the maximum size and stores data there
    size_t echo_size = max_echo_size();
    size_t data_size = max_data_size() * echo_size;
    data_multiecho.resize(data_size);
    intensity_multiecho.resize(data_size);
    int ret = urg_get_multiecho_intensity(&pimpl->urg_,
                                          &data_multiecho[0],
                                          &intensity_multiecho[0],
                                          time_stamp);
    if (ret > 0)
    {
        data_multiecho.resize(ret * echo_size);
        intensity_multiecho.resize(ret * echo_size);
        pimpl->adjust_time_stamp(time_stamp);
    }
    return (ret < 0) ? false : true;
}

bool Urg_driver::set_scanning_parameter(int32_t first_step, int32_t last_step,
                                        int32_t skip_step)
{
    int ret = urg_set_scanning_parameter(&pimpl->urg_,
                                         first_step, last_step, skip_step);
    return (ret < 0) ? false : true;
}

void Urg_driver::stop_measurement(void)
{
    urg_stop_measurement(&pimpl->urg_);
}

bool Urg_driver::start_time_stamp_mode(void)
{
    int ret = urg_start_time_stamp_mode(&pimpl->urg_);
    return (ret < 0) ? false : true;
}

bool Urg_driver::stop_time_stamp_mode(void)
{
    int ret = urg_stop_time_stamp_mode(&pimpl->urg_);
    return (ret < 0) ? false : true;
}

int32_t Urg_driver::get_sensor_time_stamp(void)
{
    int32_t time_stamp = urg_time_stamp(&pimpl->urg_);
    if (time_stamp < 0)
    {
        return time_stamp; // error code
    }

    pimpl->adjust_time_stamp(&time_stamp);
    return time_stamp;
}

bool Urg_driver::set_sensor_time_stamp(int32_t time_stamp)
{
    // \~japanese この時点での PC のタイムスタンプを取得
    // \~english Gets the PC's current timestamp
    int32_t function_first_ticks = ticks();

    // \~japanese PC とセンサのタイムスタンプの差を計算から推定し、
    // \~japanese 最後に指定された time_stamp になるような補正値を足し込む
    // \~english Estimates the difference between the PC's and the sensor timestamps
    // \~english and then adds the correction offset indicated by the time_stamp argument
    enum
    {
        Average_times = 10,
    };

    int32_t sum_of_pc_and_sensor_diff = 0;
    int ret = urg_start_time_stamp_mode(&pimpl->urg_);
    if (ret != 0)
    {
        return false;
    }
    for (int i = 0; i < Average_times; ++i)
    {
        int32_t before_ticks = ticks();
        int32_t sensor_ticks = urg_time_stamp(&pimpl->urg_);
        int32_t after_ticks = ticks();
        int32_t estimated_communication_msec = (after_ticks - before_ticks) / 2;
        int32_t pc_ticks = before_ticks + estimated_communication_msec;
        int32_t pc_and_sensor_diff = pc_ticks - sensor_ticks;
        sum_of_pc_and_sensor_diff += pc_and_sensor_diff;
    }
    ret = urg_stop_time_stamp_mode(&pimpl->urg_);
    if (ret != 0)
    {
        return false;
    }

    pimpl->time_stamp_offset_ =
        (sum_of_pc_and_sensor_diff / Average_times) +
        (time_stamp - function_first_ticks);

    return true;
}

double Urg_driver::index2rad(int32_t index) const
{
    return urg_index2rad(&pimpl->urg_, index);
}

double Urg_driver::index2deg(int32_t index) const
{
    return urg_index2deg(&pimpl->urg_, index);
}

int32_t Urg_driver::rad2index(double radian) const
{
    return urg_rad2index(&pimpl->urg_, radian);
}

int32_t Urg_driver::deg2index(double degree) const
{
    return urg_deg2index(&pimpl->urg_, degree);
}

int32_t Urg_driver::rad2step(double radian) const
{
    return urg_rad2step(&pimpl->urg_, radian);
}

int32_t Urg_driver::deg2step(double degree) const
{
    return urg_deg2step(&pimpl->urg_, degree);
}

double Urg_driver::step2rad(int32_t step) const
{
    return urg_step2rad(&pimpl->urg_, step);
}

double Urg_driver::step2deg(int32_t step) const
{
    return urg_step2deg(&pimpl->urg_, step);
}

int32_t Urg_driver::step2index(int32_t step) const
{
    return urg_step2index(&pimpl->urg_, step);
}

int32_t Urg_driver::min_step(void) const
{
    int32_t min_step;
    int32_t max_step;

    urg_step_min_max(&pimpl->urg_, &min_step, &max_step);

    return min_step;
}

int32_t Urg_driver::max_step(void) const
{
    int32_t min_step;
    int32_t max_step;

    urg_step_min_max(&pimpl->urg_, &min_step, &max_step);

    return max_step;
}

int32_t Urg_driver::min_distance(void) const
{
    int32_t min_distance;
    int32_t max_distance;

    urg_distance_min_max(&pimpl->urg_, &min_distance, &max_distance);

    return min_distance;
}

int32_t Urg_driver::max_distance(void) const
{
    int32_t min_distance;
    int32_t max_distance;

    urg_distance_min_max(&pimpl->urg_, &min_distance, &max_distance);

    return max_distance;
}

int32_t Urg_driver::scan_usec(void) const
{
    return urg_scan_usec(&pimpl->urg_);
}

int32_t Urg_driver::max_data_size(void) const
{
    return urg_max_data_size(&pimpl->urg_);
}

int32_t Urg_driver::max_echo_size(void) const
{
    return URG_MAX_ECHO;
}

int32_t Urg_driver::max_io_size(void) const
{
    return URG_MAX_IO;
}

const char *Urg_driver::product_type(void) const
{
    if (pimpl->product_type_.empty())
    {
        pimpl->product_type_ = urg_sensor_product_type(&pimpl->urg_);
    }
    return pimpl->product_type_.c_str();
}

const char *Urg_driver::firmware_version(void) const
{
    if (pimpl->firmware_version_.empty())
    {
        pimpl->firmware_version_ = urg_sensor_firmware_version(&pimpl->urg_);
    }
    return pimpl->firmware_version_.c_str();
}

const char *Urg_driver::serial_id(void) const
{
    if (pimpl->serial_id_.empty())
    {
        pimpl->serial_id_ = urg_sensor_serial_id(&pimpl->urg_);
    }
    return pimpl->serial_id_.c_str();
}

const char *Urg_driver::status(void) const
{
    pimpl->status_ = urg_sensor_status(&pimpl->urg_);
    return pimpl->status_.c_str();
}

const char *Urg_driver::state(void) const
{
    pimpl->state_ = urg_sensor_state(&pimpl->urg_);
    return pimpl->state_.c_str();
}

int32_t Urg_driver::raw_write(const char *data, size_t data_size)
{
    return urg_raw_write(&pimpl->urg_, data, data_size);
}

int32_t Urg_driver::raw_read(char *data, size_t max_data_size, int32_t timeout)
{
    return urg_raw_read(&pimpl->urg_, data, max_data_size, timeout);
}

int32_t Urg_driver::raw_readline(char *data, size_t max_data_size, int32_t timeout)
{
    return urg_raw_readline(&pimpl->urg_, data, max_data_size, timeout);
}

void *Urg_driver::raw_urg(void)
{
    return (void *)&pimpl->urg_;
}

void Urg_driver::set_measurement_type(measurement_type_t type)
{
    pimpl->last_measure_type_ = type;
}

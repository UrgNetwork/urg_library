#include "urg_sensor.h"
#include "urg_utils.h"

int32_t main(void)
{
    const char connect_device[] = "/dev/ttyACM0";
    const int32_t connect_baudrate = 115200;
    urg_t urg;
    int32_t first_step;
    int32_t last_step;
    int32_t skip_step;
    int32_t scan_times;
    int32_t skip_scan;
    int32_t ret;
    // \~japanese �v���p�����[�^�̐ݒ�
    // \~english Configures measurement parameters

    // \~japanese �Z���T�ɑ΂��Đڑ����s���B
    // \~japanese �ڑ����s���ƁA�v���p�����[�^�̐ݒ�͏����������
    // \~english Connects to the sensor
    // \~english Upon connection, measurement parameters are initialized (default values)
    ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
    // \todo check error code

    // \~japanese �v���͈͂��w�肷��
    // \~japanese �Z���T���ʕ����� 90 [deg] �͈͂̃f�[�^�擾���s���A�X�e�b�v�Ԉ������s��Ȃ���
    // \~english Defines the measurement scope (start, end steps)
    // \~english Defines a measurement scope of 90 [deg] at the front of the sensor, and no step grouping in this example
    first_step = urg_rad2step(&urg, -45);
    last_step = urg_rad2step(&urg, +45);
    skip_step = 0;
    ret = urg_set_scanning_parameter(&urg, first_step, last_step, skip_step);
    // \todo check error code

    // \~japanese �v���񐔂ƌv���̊Ԉ������w�肵�āA�v�����J�n����
    // \~japanese 123 ��̌v�����w�����A�X�L�����̊Ԉ������s��Ȃ���
    // \~english Defines the number of scans
    // \~english 123 scans are requested, and no scan skipping in this example
    scan_times = 123;
    skip_scan = 0;
    ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, skip_scan, 0);
    // \todo check error code
    return 0;
}

#include "urg_sensor.h"

int32_t main(void)
{
    urg_t urg;
    int32_t ret;
    // \~japanese �C�[�T�[�l�b�g�ڑ��ł̃Z���T�Ƃ̐ڑ��Ƌ����f�[�^�̎擾
    // \~english Connects to the sensor via Ethernet and receives range data

    const char connect_address[] = "192.168.0.10";
    const int32_t connect_port = 10940;

    // \~japanese �Z���T�ɑ΂��Đڑ����s���B
    // \~english Connects to the sensor
    ret = urg_open(&urg, URG_ETHERNET, connect_address, connect_port);
    // \todo check error code
    return 0;
}

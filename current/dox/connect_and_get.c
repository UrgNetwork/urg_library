// \~japanese �V���A���ڑ��ł̃Z���T�Ƃ̐ڑ��Ƌ����f�[�^�̎擾
// \~english Connects to the sensor via serial interface and gets range data

#include "urg_sensor.h"
#include "urg_utils.h"
#include <stdlib.h>

int32_t main(void)
{
    urg_t urg;
    int32_t ret;
    int32_t *length_data;
    int32_t length_data_size;

    // \~japanese "COM1" �́A�Z���T���F������Ă���f�o�C�X���ɂ���K�v������
    // \~english "COM1" is, in this case, the device name detected for the sensor
    const char connect_device[] = "COM1";
    const int32_t connect_baudrate = 115200;

    // \~japanese �Z���T�ɑ΂��Đڑ����s���B
    // \~english Connects to the sensor
    ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
    // \todo check error code

    // \~japanese �f�[�^��M�̂��߂̗̈���m�ۂ���
    // \~english Allocates memory to hold received measurement data
    length_data = (int32_t *)malloc(sizeof(int32_t) * urg_max_data_size(&urg));
    // \todo check length_data is not NULL

    // \~japanese �����f�[�^�̌v���J�n�B
    // \~english Starts range data measurement
    ret = urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
    // \todo check error code

    // \~japanese �Z���T���狗���f�[�^���擾����B
    // \~english Receives the measurement data
    length_data_size = urg_get_distance(&urg, length_data, NULL);
    // \todo process length_data array

    // \~japanese �Z���T�Ƃ̐ڑ������B
    // \~english Disconnects from the sensor
    urg_close(&urg);

    return 0;
}

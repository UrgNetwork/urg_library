#include "urg_sensor.h"
#include <inttypes.h>
#include <stdio.h>

int32_t main(void)
{
    urg_t urg;
    int32_t *length_data = NULL;
    int32_t ret;
    // \~japanese �^�C���X�^���v�̎擾
    // \~english Gets timestamp values

    // \~japanese urg_get_distance() �֐��ɕϐ���^���A�^�C���X�^���v���擾����B
    // \~english Uses the urg_get_distance() function and returns the timestamp values for each scan

    const int32_t scan_times = 123;
    int32_t length_data_size;
    int32_t timestamp;
    int32_t i;

    // \~japanese �Z���T���狗���f�[�^���擾����B
    // \~english Starts range data measurement
    ret = urg_start_measurement(&urg, URG_DISTANCE, scan_times, 0);
    // \todo check error code

    for (i = 0; i < scan_times; ++i)
    {
        length_data_size = urg_get_distance(&urg, length_data, &timestamp);
        // \todo process length_data array

        // \~japanese �擾�����^�C���X�^���v���o�͂���
        // \~english Outputs the received timestamp value
        printf("%" PRId32 "\n", timestamp);
    }
    return 0;
}

#include "urg_sensor.h"
#include "urg_utils.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h>

int32_t main(void)
{
    urg_t urg;
    int32_t *length_data = NULL;
    int32_t length_data_size;
    int32_t i;
    // \~japanese �����f�[�^�� X-Y ���W�n�ɕϊ����ĕ\������
    // \~english Converts data to X-Y coordinates and displays it

    length_data_size = urg_get_distance(&urg, length_data, NULL);
    for (i = 0; i < length_data_size; ++i)
    {
        // \~japanese ���̋����f�[�^�̃��W�A���p�x�����߁AX, Y �̍��W�l���v�Z����
        // \~english Gets the angle in radians for range data, and convert to X-Y coordinates
        double radian;
        int32_t length;
        int32_t x;
        int32_t y;

        radian = urg_index2rad(&urg, i);
        length = length_data[i];
        // \todo check length is valid

        x = (int32_t)(length * cos(radian));
        y = (int32_t)(length * sin(radian));
        printf("(%" PRId32 ", %" PRId32 "), ", x, y);
    }
    printf("\n");
    return 0;
}

/*!
  \file
  \brief Simple viewer (SDL)

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_sensor.h"
#include "urg_utils.h"
#include "urg_connection.h"
#include "plotter_sdl.h"
#include <SDL.h>
#include <math.h>

#if defined(URG_WINDOWS_OS)
static const char *default_serial_device = "COM3";
#else
static const char *default_serial_device = "/dev/ttyACM0";
#endif
static const char *default_ip_address = "192.168.0.10";
// static const char *default_ip_address = "localhost";

typedef struct
{
    urg_connection_type_t connection_type;
    const char *device;
    int32_t baudrate_or_port;
    urg_measurement_type_t measurement_type;
    bool is_intensity;
    bool is_multiecho;
} scan_mode_t;

static void help_exit(const char *program_name)
{
    printf("URG simple data viewer\n"
           "usage:\n"
           "    %s [options]\n"
           "\n"
           "options:\n"
           "  -h, --help    display this help and exit\n"
           "  -s [device name],   serial connection mode\n"
           "  -e [ip address],    ethernet connection mode\n"
           "  -i,           intensity mode\n"
           "  -m,           multiecho mode\n"
           "\n",
           program_name);
}

static void parse_args(scan_mode_t *mode, int32_t argc, char *argv[])
{
    int32_t i;

    mode->connection_type = URG_SERIAL;
    mode->device = default_serial_device;
    mode->baudrate_or_port = 115200;
    mode->is_multiecho = false;
    mode->is_intensity = false;

    for (i = 1; i < argc; ++i)
    {
        const char *token = argv[i];

        if (!strcmp(token, "-h") || !strcmp(token, "--help"))
        {
            help_exit(argv[0]);
        }
        else if (!strcmp(token, "-s"))
        {
            mode->connection_type = URG_SERIAL;
            mode->device = default_serial_device;
            if (argc > i + 1 && argv[i + 1][0] != '-')
            {
                mode->device = argv[++i];
            }
            mode->baudrate_or_port = 115200;
        }
        else if (!strcmp(token, "-e"))
        {
            mode->connection_type = URG_ETHERNET;
            mode->device = default_ip_address;
            if (argc > i + 1 && argv[i + 1][0] != '-')
            {
                mode->device = argv[++i];
            }
            mode->baudrate_or_port = 10940;
        }
        else if (!strcmp(token, "-m"))
        {
            mode->is_multiecho = true;
        }
        else if (!strcmp(token, "-i"))
        {
            mode->is_intensity = true;
        }
    }

    if (mode->is_multiecho)
    {
        mode->measurement_type =
            (mode->is_intensity) ? URG_MULTIECHO_INTENSITY : URG_MULTIECHO;
    }
    else
    {
        mode->measurement_type =
            (mode->is_intensity) ? URG_DISTANCE_INTENSITY : URG_DISTANCE;
    }
}

static void plot_data_point(urg_t *urg, int32_t data[], unsigned short intensity[],
                            int32_t data_n, bool is_multiecho, int32_t offset)
{
    int32_t min_distance;
    int32_t max_distance;
    const double radian_offset = M_PI / 2.0;
    int32_t step = (is_multiecho) ? 3 : 1;
    int32_t i;

    urg_distance_min_max(urg, &min_distance, &max_distance);

    for (i = 0; i < data_n; ++i)
    {
        int32_t index = (step * i) + offset;
        int32_t l = (data) ? data[index] : intensity[index];
        double rad;
        float x;
        float y;

        if ((l <= min_distance) || (l >= max_distance))
        {
            continue;
        }

        rad = urg_index2rad(urg, i) + radian_offset;
        x = l * cos(rad);
        y = l * sin(rad);
        plotter_plot(x, y);
    }
}

static void plot_data(urg_t *urg,
                      int32_t data[], unsigned short intensity[], int32_t data_n,
                      bool is_multiecho)
{
    plotter_clear();

    // \~japanese ����
    plotter_set_color(0x00, 0xff, 0xff);
    plot_data_point(urg, data, NULL, data_n, is_multiecho, 0);

    if (is_multiecho)
    {
        plotter_set_color(0xff, 0x00, 0xff);
        plot_data_point(urg, data, NULL, data_n, is_multiecho, 1);

        plotter_set_color(0x00, 0x00, 0xff);
        plot_data_point(urg, data, NULL, data_n, is_multiecho, 2);
    }

    if (intensity)
    {
        // \~japanese  ���x
        plotter_set_color(0xff, 0xff, 0x00);
        plot_data_point(urg, NULL, intensity, data_n, is_multiecho, 0);

        if (is_multiecho)
        {
            plotter_set_color(0xff, 0x00, 0x00);
            plot_data_point(urg, NULL, intensity, data_n, is_multiecho, 1);

            plotter_set_color(0x00, 0xff, 0x00);
            plot_data_point(urg, NULL, intensity, data_n, is_multiecho, 2);
        }
    }

    plotter_swap();
}

int32_t main(int32_t argc, char *argv[])
{
    scan_mode_t mode;
    urg_t urg;
    int32_t *data = NULL;
    unsigned short *intensity = NULL;
    int32_t timestamp;
    int32_t data_size;

    // \~japanese  �����̉��
    // \~english Analyzes the arguments
    parse_args(&mode, argc, argv);

    // \~japanese  URG �ɐڑ�
    // \~english Connects to the URG
    if (urg_open(&urg, mode.connection_type,
                 mode.device, mode.baudrate_or_port))
    {
        printf("urg_open: %s\n", urg_error(&urg));
        return 1;
    }

    // \~japanese  �f�[�^�擾�̏���
    // \~english Prepares for measuremment data reading
    data_size = urg_max_data_size(&urg);
    if (mode.is_multiecho)
    {
        data_size *= 3;
    }
    data = malloc(data_size * sizeof(data[0]));
    if (mode.is_intensity)
    {
        intensity = malloc(data_size * sizeof(intensity[0]));
    }

    // \~japanese  ��ʂ̍쐬
    // \~english Perpares the plot screen
    if (!plotter_initialize(data_size * ((mode.is_intensity) ? 2 : 1)))
    {
        return 1;
    }

    // \~japanese  �f�[�^�̎擾�ƕ`��
    // \~english Gets and displays measurement data
    urg_start_measurement(&urg, mode.measurement_type, URG_SCAN_INFINITY, 0);
    while (1)
    {
        int32_t n;
        // urg_start_measurement(&urg, mode.measurement_type, 1, 0);
        switch (mode.measurement_type)
        {
        case URG_DISTANCE:
            n = urg_get_distance(&urg, data, &timestamp);
            break;

        case URG_DISTANCE_INTENSITY:
            n = urg_get_distance_intensity(&urg, data, intensity, &timestamp);
            break;

        case URG_MULTIECHO:
            n = urg_get_multiecho(&urg, data, &timestamp);
            break;

        case URG_MULTIECHO_INTENSITY:
            n = urg_get_multiecho_intensity(&urg, data, intensity, &timestamp);
            break;

        default:
            n = 0;
            break;
        }

        if (n <= 0)
        {
            printf("urg_get_function: %s\n", urg_error(&urg));
            break;
        }

        // fprintf(stderr, "%ld, ", timestamp - previous_timestamp);
        // previous_timestamp = timestamp;

        plot_data(&urg, data, intensity, n, mode.is_multiecho);
        if (plotter_is_quit())
        {
            break;
        }
    }

    // \~japanese  ���\�[�X�̉��
    // \~english Release resources
    plotter_terminate();
    free(intensity);
    free(data);
    urg_close(&urg);

    return 0;
}

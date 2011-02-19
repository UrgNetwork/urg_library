// 計測パラメータの設定

// センサに対して接続を行う。
// 接続を行うと、計測パラメータの設定は初期化される
ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
// \todo check error code

// 計測範囲を指定する
// センサ正面方向の 90 [deg] 範囲のデータ取得を行い、ステップ間引きを行わない例
first_step = urg_rad2step(&urg, -45);
last_step = urg_rad2step(&urg, +45);
skip_step = 0;
ret = urg_set_scanning_parameter(&urg, first_step, last_step, skip_step);
// \todo check error code

// 計測回数と計測の間引きを指定して、計測を開始する
// 123 回の計測を指示し、スキャンの間引きを行わない例
scan_times = 123;
skip_scan = 0;
extern int urg_start_measurement(&urg, URG_DISTANCE, scan_times, skip_scan);
// \todo check error code

#ifndef URG_RING_BUFFER_H
#define URG_RING_BUFFER_H

/*!
  \file
  \brief リングバッファ

  \author Satofumi KAMIMURA

  $Id$
*/


//! リングバッファの管理情報
typedef struct
{
    char *buffer;                 //!< バッファへのポインタ
    int buffer_size;              //!< バッファサイズ
    int first;                    //!< バッファの先頭位置
    int last;                     //!< バッファの最終位置
} ring_buffer_t;


/*!
  \brief 初期化

  \param[in] ring リングバッファの構造体
  \param[in] buffer 割り当てるバッファ
  \param[in] shift_length バッファサイズの 2 の乗数
*/
void ring_initialize(ring_buffer_t *ring,
                            char *buffer, const int shift_length);


/*!
  \brief リングバッファのクリア

  \param[in] ring リングバッファの構造体
*/
void ring_clear(ring_buffer_t *ring);


/*!
  \brief 格納データ数を返す

  \param[in] ring リングバッファの構造体
*/
int ring_size(const ring_buffer_t *ring);


/*!
  \brief 最大の格納データ数を返す

  \param[in] ring リングバッファの構造体
*/
int ring_capacity(const ring_buffer_t *ring);


/*!
  \brief データの格納

  \param[in] ring リングバッファの構造体
  \param[in] data データ
  \param[in] size データサイズ

  \return 格納したデータ数
*/
int ring_write(ring_buffer_t *ring, const char *data, int size);


/*!
  \brief データの取り出し

  \param[in] ring リングバッファの構造体
  \param[out] buffer データ
  \param[in] size 最大のデータサイズ

  \return 取り出したデータ数
*/
int ring_read(ring_buffer_t *ring, char *buffer, int size);

int ring_readPtr(ring_buffer_t *ring, char **ptr, int size);

#endif /* ! RING_BUFFER_H */

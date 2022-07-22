#include <iostream>
#include <sstream>
#include <stdexcept>

/**
 * @brief srcからnum個の連続するT型の値をkeyの値でソートし
 *        destから始まる連続する領域に格納する
 *
 * @tparam T ソートする配列の型
 * @tparam KR_MAX keyrangeの上限値
 * @param dest 格納先の先頭アドレス
 *             [dest, dest+num) の範囲が書き込み可能であることが
 *             保証されている必要がある
 * @param src ソートする配列の先頭アドレス
 *            [src, src+num) の範囲が読み取り可能であることが
 *            保証されている必要がある
 * @param num ソートする配列の要素数
 * @param keyrange [src, src+num) の範囲に関する
 *                 keyの返り値について [0, keyrange) となる値
 * @param key ソートのキーを [0, keyrange) の範囲で返す関数
 * @attention [dest, dest+num) と [src, src+num) の範囲が重なる場合の
 *            動作は保証されない
 */
template <typename T, const size_t KR_MAX = 4096>
void bucket_sort(T dest[], T src[], size_t num, size_t keyrange,
                 size_t (*key)(T *)) {
  static size_t index_of[KR_MAX + 1];
  // 不正なkeyrangeで例外を投げる
  if (keyrange < 0 || KR_MAX < keyrange) {
    std::stringstream ss;
    ss << keyrange << " of keyrange is out of range (0, " << KR_MAX << "].";
    throw std::out_of_range(ss.str());
  }

  for (size_t i = 0; i < num; i++) {
    size_t k = key(src + i);
    index_of[k + 1]++;
  }

  for (size_t i = 0; i < keyrange; i++) {
    index_of[i + 1] += index_of[i];
  }

  for (size_t i = 0; i < num; i++) {
    size_t k = key(src + i);
    dest[index_of[k]++] = src[i];
  }
}

int main() {
  int src[] = {0, 7, 2, 4, 1, 1, 1, 7, 3, 3, 5, 1, 2};
  int dest[20] = {0};

  bucket_sort<int>(dest, src, 13, 8, [](int *x) { return (size_t)*x; });

  for (size_t i = 0; i < 20; i++) {
    std::cout << dest[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

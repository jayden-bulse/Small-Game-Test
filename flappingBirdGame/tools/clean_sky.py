# -*- coding: utf-8 -*-
"""
清理天空区域里极淡的污迹 / 水印 / JPEG 残留。

原理：
  天空是一片近似纯色的区域，而那些污迹只是"和天空色非常接近、但略有偏差"的像素。
  对天空区域的每一行：
    1. 找出这一行里"属于天空色"的像素（与参考天空色差距很小）；
    2. 用这些像素的中值作为该行的天空颜色；
    3. 把这些天空像素统一替换成该行颜色。
  云朵、建筑、树木与天空色差距很大，不会被改动；
  由于是逐行处理，天空本身若有轻微的上下渐变也会被保留。

用法：
  python clean_sky.py 输入图 输出图 [天空区域下边比例] [天空色判定阈值]

  天空区域下边比例默认 0.60（图片高度的 60% 以下就进入云朵区域，不再处理）
  判定阈值默认 60（RGB 三通道差的绝对值之和；越小只清理越接近天空色的像素）
"""

import sys
import numpy as np
from PIL import Image


def clean_sky(src_path: str, dst_path: str, sky_bottom: float = 0.60, tol: int = 60) -> None:
    im = Image.open(src_path).convert("RGB")
    a = np.asarray(im).astype(np.int16)
    H, W, _ = a.shape

    # 用顶部一小块（一定属于天空）作为参考天空色
    ref = a[0:max(4, H // 40), :, :].reshape(-1, 3)
    sky_ref = np.median(ref, axis=0)

    y_end = int(H * sky_bottom)
    changed = 0

    for y in range(y_end):
        row = a[y]
        d = np.abs(row - sky_ref).sum(axis=1)
        sky_mask = d < tol
        n = int(sky_mask.sum())
        if n < 20:           # 这一行几乎全是云，跳过
            continue
        row_med = np.median(row[sky_mask], axis=0)
        row[sky_mask] = row_med
        changed += n

    out = Image.fromarray(a.astype(np.uint8))
    out.save(dst_path)
    print(f"OK {dst_path}  size={out.size}  sky_ref={sky_ref.astype(int).tolist()}  cleaned_px={changed}")


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)
    src, dst = sys.argv[1], sys.argv[2]
    bottom = float(sys.argv[3]) if len(sys.argv) > 3 else 0.60
    tol = int(sys.argv[4]) if len(sys.argv) > 4 else 60
    clean_sky(src, dst, bottom, tol)


if __name__ == "__main__":
    main()

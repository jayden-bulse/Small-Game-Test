# -*- coding: utf-8 -*-
"""
把一张背景图变成「水平方向可无缝拼接」的图片。

原理（右边缘 → 左边缘 的镜像融合 / mirror-blend）：
  1. 取图片最右侧的一条竖条（宽度 = blend_ratio * 图宽）；
  2. 把它水平镜像，镜像后它的第 0 列正好等于原图最右一列；
  3. 用一条从「完全不透明」渐变到「完全透明」的遮罩，把它叠到图片最左侧；
  4. 结果：新图的第 0 列 == 原图最右列，两张图左右相接时像素连续，看不出接缝。

优点：不缩放、不拉伸，云朵/树木的比例大小完全不变。

用法：
  python make_background_tileable.py 输入图 输出图 [融合宽度比例]
  python make_background_tileable.py a.png b.png 0.10

  融合宽度比例默认 0.10（占图宽的 10%）。数值越大过渡越柔和，
  但左边缘出现的「镜像反射感」也越强；一般 0.08 ~ 0.15 之间效果最好。
"""

import sys
from PIL import Image


def smoothstep(t: float) -> float:
    """平滑插值：两端变化慢、中间变化快，比线性过渡更自然。"""
    return t * t * (3.0 - 2.0 * t)


def make_tileable(src: Image.Image, blend_ratio: float = 0.10, hold: float = 0.5) -> Image.Image:
    img = src.convert("RGBA")
    W, H = img.size

    w = max(8, int(round(W * blend_ratio)))
    w = min(w, W - 1)

    left = img.crop((0, 0, w, H))                 # 原图左边条
    right = img.crop((W - w, 0, W, H))            # 原图右边条
    mirrored = right.transpose(Image.Transpose.FLIP_LEFT_RIGHT)

    # 渐变遮罩：
    #   x < hold*w      -> 255（完全使用镜像内容，画面清晰无重影）
    #   hold*w .. w     -> 255 平滑降到 0（在这里过渡回原图左边的内容）
    # hold=0 时就是普通的全程线性融合（重影更明显）
    hold = min(max(hold, 0.0), 0.95)
    x0 = int(round(w * hold))
    span = max(1, w - x0)
    mask = Image.new("L", (w, 1))
    px = mask.load()
    for x in range(w):
        if x < x0:
            px[x, 0] = 255
        else:
            t = (x - x0) / span
            px[x, 0] = int(round((1.0 - smoothstep(t)) * 255))
    mask = mask.resize((w, H), Image.Resampling.NEAREST)

    blended = Image.composite(mirrored, left, mask)

    out = img.copy()
    out.paste(blended, (0, 0))
    return out


def make_preview(img: Image.Image, tiles: int = 3) -> Image.Image:
    """把图水平重复拼几次，方便肉眼检查接缝。"""
    W, H = img.size
    canvas = Image.new("RGBA", (W * tiles, H))
    for i in range(tiles):
        canvas.paste(img, (i * W, 0))
    # 用细红线标出接缝位置，便于检查
    from PIL import ImageDraw
    d = ImageDraw.Draw(canvas)
    for i in range(1, tiles):
        d.line([(i * W, 0), (i * W, H)], fill=(255, 0, 0, 255), width=1)
    return canvas


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)

    src_path, dst_path = sys.argv[1], sys.argv[2]
    ratio = float(sys.argv[3]) if len(sys.argv) > 3 else 0.10
    hold = float(sys.argv[4]) if len(sys.argv) > 4 else 0.5

    src = Image.open(src_path)
    out = make_tileable(src, ratio, hold)
    out.save(dst_path)
    print(f"OK  {dst_path}  size={out.size}  blend_width={max(8, int(round(out.size[0] * ratio)))} px  hold={hold}")

    # 预览图：3 张横向拼接，红线为接缝
    prev_path = dst_path.rsplit(".", 1)[0] + "_preview.png"
    make_preview(out, 3).save(prev_path)
    print(f"OK  {prev_path}")


if __name__ == "__main__":
    main()

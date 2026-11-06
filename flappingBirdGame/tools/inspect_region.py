# -*- coding: utf-8 -*-
"""放大指定区域并做对比度增强，用来检查图片里极淡的伪影/水印。"""

import sys
from PIL import Image, ImageEnhance, ImageDraw


def main():
    if len(sys.argv) < 6:
        print("usage: inspect_region.py out.png in.png x0frac y0frac x1frac y1frac [contrast]")
        sys.exit(1)

    out = sys.argv[1]
    src = sys.argv[2]
    x0f, y0f, x1f, y1f = map(float, sys.argv[3:7])
    contrast = float(sys.argv[7]) if len(sys.argv) > 7 else 4.0

    im = Image.open(src).convert("RGB")
    W, H = im.size
    box = (int(W * x0f), int(H * y0f), int(W * x1f), int(H * y1f))
    crop = im.crop(box)
    crop = crop.resize((crop.width * 3, crop.height * 3), Image.Resampling.LANCZOS)

    # 对比度增强版：把极淡的差异放大出来
    boost = ImageEnhance.Contrast(crop).enhance(contrast)

    canvas = Image.new("RGB", (crop.width, crop.height * 2 + 24), (0, 0, 0))
    canvas.paste(crop, (0, 0))
    canvas.paste(boost, (0, crop.height + 24))
    ImageDraw.Draw(canvas).text((6, crop.height + 6),
                                f"top=original   bottom=contrast x{contrast}",
                                fill=(255, 255, 255))
    canvas.save(out)
    print(f"OK {out} size={canvas.size} crop={box}")


if __name__ == "__main__":
    main()

# -*- coding: utf-8 -*-
"""放大对比多张图的接缝质量：每张图先横向拼两份，再裁出接缝附近区域放大。"""

import sys
from PIL import Image, ImageDraw


def seam_crop(img: Image.Image, half: int = 100, y0: int = 540, y1: int = 820, zoom: int = 3) -> Image.Image:
    W, H = img.size
    two = Image.new("RGBA", (W * 2, H))
    two.paste(img, (0, 0))
    two.paste(img, (W, 0))

    x0 = W - half
    x1 = W + half
    crop = two.crop((x0, y0, x1, y1))
    crop = crop.resize((crop.width * zoom, crop.height * zoom), Image.Resampling.NEAREST)

    d = ImageDraw.Draw(crop)
    cx = half * zoom
    d.line([(cx, 0), (cx, crop.height)], fill=(255, 0, 0, 255), width=2)
    return crop


def main():
    if len(sys.argv) < 3:
        print("usage: seam_check.py out.png in1.png [in2.png ...] [--y0 N --y1 N]")
        sys.exit(1)

    args = sys.argv[1:]
    y0, y1 = 540, 820
    if "--y0" in args:
        i = args.index("--y0")
        y0 = int(args[i + 1])
        del args[i:i + 2]
    if "--y1" in args:
        i = args.index("--y1")
        y1 = int(args[i + 1])
        del args[i:i + 2]

    out_path = args[0]
    paths = args[1:]

    crops = []
    for p in paths:
        im = Image.open(p).convert("RGBA")
        c = seam_crop(im, y0=y0, y1=y1)
        lab = Image.new("RGBA", (c.width, 24), (0, 0, 0, 255))
        ImageDraw.Draw(lab).text((6, 6), p.split("/")[-1].split("\\")[-1], fill=(255, 255, 255, 255))
        crops.append(lab)
        crops.append(c)

    total_h = sum(c.height for c in crops)
    W = max(c.width for c in crops)
    canvas = Image.new("RGBA", (W, total_h), (255, 255, 255, 255))
    y = 0
    for c in crops:
        canvas.paste(c, (0, y))
        y += c.height
    canvas.save(out_path)
    print(f"OK {out_path} size={canvas.size}")


if __name__ == "__main__":
    main()

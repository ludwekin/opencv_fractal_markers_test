#!/usr/bin/env python3
"""
Robust PNG->PDF for printing with exact physical size.
Usage:
    python3 tools/png_to_pdf_print.py input.png output.pdf --size-mm 50 --border-mm 5
"""
import argparse
import os
import sys
from reportlab.lib.units import mm
from reportlab.pdfgen import canvas
from PIL import Image

def png_to_pdf(input_png, output_pdf, size_mm, border_mm=5):
    if not os.path.exists(input_png):
        raise FileNotFoundError(f"Input PNG not found: {input_png}")
    # basic checks
    try:
        img = Image.open(input_png)
        img.verify()  # will raise if file is broken
    except Exception as e:
        raise RuntimeError(f"Cannot open/verify image {input_png}: {e}")
    size_pts = size_mm * mm
    border_pts = border_mm * mm
    page_w = size_pts + 2 * border_pts
    # create pdf
    c = canvas.Canvas(output_pdf, pagesize=(page_w, page_w))
    # Draw image centered at border
    c.drawImage(input_png, border_pts, border_pts, width=size_pts, height=size_pts, preserveAspectRatio=True, mask='auto')
    c.showPage()
    c.save()

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input_png")
    parser.add_argument("output_pdf")
    parser.add_argument("--size-mm", type=float, required=True, help="Physical side length in millimeters")
    parser.add_argument("--border-mm", type=float, default=5, help="Border in millimeters")
    args = parser.parse_args()

    try:
        print(f"Input: {args.input_png}")
        print(f"Output: {args.output_pdf}")
        print(f"Size (mm): {args.size_mm}, Border (mm): {args.border_mm}")
        png_to_pdf(args.input_png, args.output_pdf, args.size_mm, args.border_mm)
        print("PDF created successfully.")
    except Exception as e:
        print("ERROR:", e)
        sys.exit(1)

if __name__ == "__main__":
    main()
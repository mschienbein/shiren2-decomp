.PHONY: all trial setup split test

all:
	uv run --frozen python tools/build.py --promote

trial:
	uv run --frozen python tools/build.py --no-promote

setup:
	uv sync --frozen
	uv run --frozen python tools/setup.py

split:
	uv run --frozen python tools/rom.py verify
	uv run --frozen python -m splat split config/shiren2.jp.yaml --disassemble-all

test:
	uv run --frozen python -m unittest discover -s tests -v

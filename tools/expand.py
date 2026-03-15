#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*$')


class Expander:
    def __init__(self, root: Path) -> None:
        self.root = root.resolve()
        self.include_roots = [self.root, self.root / "lib"]
        self.expanded_files: set[Path] = set()

    def expand(self, path: Path) -> str:
        return self._expand_file(path.resolve())

    def _expand_file(self, path: Path) -> str:
        if path in self.expanded_files:
            return ""
        self.expanded_files.add(path)

        lines: list[str] = []
        for raw_line in path.read_text().splitlines():
            if raw_line.strip() == "#pragma once":
                continue

            match = INCLUDE_RE.match(raw_line)
            if match is None:
                lines.append(raw_line)
                continue

            delimiter, include_path = match.groups()
            if include_path == "debug/all.hpp":
                lines.append(raw_line)
                continue
            if include_path == "atcoder/all":
                lines.append(raw_line)
                continue
            resolved = self._resolve_include(path.parent, include_path, delimiter == '"')
            if resolved is None:
                lines.append(raw_line)
                continue

            lines.append(self._expand_file(resolved))
        return "\n".join(lines)

    def _resolve_include(self, base_dir: Path, include_path: str, quoted: bool) -> Path | None:
        candidates: list[tuple[Path, bool]] = []
        if quoted:
            candidates.append((base_dir / include_path, False))
        for include_root in self.include_roots:
            candidates.append((include_root / include_path, True))

        for candidate, from_include_root in candidates:
            if not candidate.exists():
                continue
            resolved = candidate.resolve()
            if self.root == resolved or self.root in resolved.parents:
                return resolved
            if from_include_root and self._is_under_include_root(candidate):
                return resolved
        return None

    def _is_under_include_root(self, path: Path) -> bool:
        return any(include_root == path or include_root in path.parents for include_root in self.include_roots)

def main() -> int:
    parser = argparse.ArgumentParser(description="Expand local #include files into a single submission file.")
    parser.add_argument("input", type=Path, help="Input source file")
    parser.add_argument("-o", "--output", type=Path, help="Output path. Defaults to stdout.")
    args = parser.parse_args()

    root = Path(__file__).resolve().parent.parent
    input_path = args.input
    if not input_path.is_absolute():
        input_path = (Path.cwd() / input_path).resolve()

    expander = Expander(root)
    expanded = expander.expand(input_path) + "\n"

    if args.output is None:
        sys.stdout.write(expanded)
    else:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(expanded)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

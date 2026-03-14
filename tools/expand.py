#!/usr/bin/env python3

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path


INCLUDE_RE = re.compile(r'^\s*#\s*include\s*([<"])([^>"]+)[>"]\s*$')
EXPAND_ACL_RE = re.compile(r"^\s*//\s*expand-acl:\s*(on|off)\s*$")


class Expander:
    def __init__(self, root: Path) -> None:
        self.root = root.resolve()
        self.include_roots = [self.root, self.root / "lib"]
        self.expanded_files: set[Path] = set()
        self.expand_acl = True

    def expand(self, path: Path) -> str:
        self.expand_acl = self._read_expand_acl_policy(path.resolve())
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
            if not self.expand_acl and include_path.startswith("atcoder/"):
                lines.append(raw_line)
                continue
            resolved = self._resolve_include(path.parent, include_path, delimiter == '"')
            if resolved is None:
                lines.append(raw_line)
                continue

            lines.append(self._expand_file(resolved))
        return "\n".join(lines)

    def _resolve_include(self, base_dir: Path, include_path: str, quoted: bool) -> Path | None:
        candidates: list[Path] = []
        if quoted:
            candidates.append(base_dir / include_path)
        for include_root in self.include_roots:
            candidates.append(include_root / include_path)

        for candidate in candidates:
            if not candidate.exists():
                continue
            resolved = candidate.resolve()
            if self.root == resolved or self.root in resolved.parents:
                return resolved
        return None

    def _read_expand_acl_policy(self, path: Path) -> bool:
        try:
            for raw_line in path.read_text().splitlines()[:20]:
                match = EXPAND_ACL_RE.match(raw_line)
                if match is not None:
                    return match.group(1) == "on"
        except OSError:
            pass
        return True


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

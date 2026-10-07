#!/usr/bin/env python3
"""Install pinned project-local MIPS binutils, assemblers and historical GCC compilers."""
from __future__ import annotations

import hashlib
import json
import os
import platform
import shutil
import subprocess
import tarfile
from pathlib import Path
from urllib.request import urlopen

PROJECT = Path(__file__).resolve().parents[1]
CACHE = PROJECT / ".cache"
# Host-only compatibility edits for building GNU binutils 2.9.1 on arm64 Darwin.
# Each replacement must apply exactly once; the lock pins before/after hashes.
GAS291_HOST_PATCHES = {
    "config.sub": [
        ("| arc-* | arm-* | c[123]*", "| arc-* | arm-* | aarch64-* | c[123]*"),
        ("-gnu* | -bsd* | -mach* |", "-gnu* | -bsd* | -mach* | -darwin* |"),
    ],
    "libiberty/strerror.c": [
        ("extern int sys_nerr;\n", "#ifdef __APPLE__\nextern const int sys_nerr;\n#else\nextern int sys_nerr;\n#endif\n"),
    ],
}


def sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def download(spec: dict, name: str) -> Path:
    """Fetch a pinned archive from its URL, then any listed mirrors; the hash decides."""
    archive = CACHE / name
    if not archive.exists():
        errors = []
        for url in [spec["url"], *spec.get("mirrors", [])]:
            print(f"Downloading {url}", flush=True)
            try:
                with urlopen(url, timeout=60) as response, archive.open("wb") as output:
                    shutil.copyfileobj(response, output)
                break
            except OSError as error:
                archive.unlink(missing_ok=True)
                errors.append(f"{url}: {error}")
        else:
            raise ValueError("Download failed: " + "; ".join(errors))
    actual = sha256(archive)
    if actual != spec["sha256"]:
        raise ValueError(f"Archive hash mismatch: {archive.name}")
    return archive


def install_prebuilt_compiler(spec: dict, archive_name: str) -> Path:
    """Extract a pinned prebuilt GCC archive once, then verify its driver/cpp/cc1 pins."""
    directory = PROJECT / spec["install_directory"]
    if not (directory / "gcc").exists():
        archive = download(spec, archive_name)
        directory.mkdir(parents=True, exist_ok=True)
        with tarfile.open(archive) as tar:
            tar.extractall(directory, filter="data")
    for name, expected in spec["binaries_sha256"].items():
        if not (directory / name).is_file() or sha256(directory / name) != expected:
            raise ValueError(f"Installed {spec['install_directory']}/{name} differs from the lock file; remove the directory and rerun setup")
    return directory


def install_gnu_gas(spec: dict, make: str) -> Path:
    """Build GNU gas 2.9.1 natively from the pinned release with recorded host-only patches."""
    directory = PROJECT / spec["install_directory"]
    assembler = directory / "as"
    if not assembler.is_file():
        if platform.machine() != "arm64":
            raise SystemExit("The binutils 2.9.1 host patches are recorded for arm64 Darwin only")
        archive = download(spec, f"binutils-{spec['version']}.tar.gz")
        source = directory / f"binutils-{spec['version']}"
        build = directory / "build"
        for stale in [source, build]:
            if stale.exists():
                shutil.rmtree(stale)
        directory.mkdir(parents=True, exist_ok=True)
        with tarfile.open(archive) as tar:
            tar.extractall(directory, filter="data")
        patches = {item["path"]: item for item in spec["host_patches"]}
        if set(patches) != set(GAS291_HOST_PATCHES):
            raise ValueError("Lock host patches disagree with setup's recorded edits")
        for relative, replacements in GAS291_HOST_PATCHES.items():
            path = source / relative
            if sha256(path) != patches[relative]["original_sha256"]:
                raise ValueError(f"Unexpected original source: {relative}")
            text = path.read_text(encoding="latin-1")
            for old, new in replacements:
                if text.count(old) != 1:
                    raise ValueError(f"Host patch anchor is not unique in {relative}: {old!r}")
                text = text.replace(old, new)
            path.write_text(text, encoding="latin-1")
            if sha256(path) != patches[relative]["modified_sha256"]:
                raise ValueError(f"Patched source differs from the lock: {relative}")
        build.mkdir()
        env = {key: value for key, value in os.environ.items() if key not in {"CFLAGS", "CPPFLAGS", "LDFLAGS", "CPATH", "C_INCLUDE_PATH", "LIBRARY_PATH", "GCC_EXEC_PREFIX", "COMPILER_PATH", "VR4300MUL", "N64ALIGN"}}
        env.update(CC="/usr/bin/clang", CXX="/usr/bin/clang++", AR="/usr/bin/ar", RANLIB="/usr/bin/ranlib", CFLAGS=spec["cflags"], LC_ALL="C", MAKE=make)
        host = f"aarch64-apple-darwin{platform.release()}"
        # A relative srcdir keeps checkout paths out of the assert() __FILE__ strings.
        configure = ["/bin/sh", f"../{source.name}/configure", f"--target={spec['target']}", f"--host={host}", f"--build={host}", *spec["configure_extra"]]
        with (directory / "build.log").open("w") as log:
            for command in [configure, [make, "-j8", spec["make_target"], f"CFLAGS={spec['cflags']}", "MAKEINFO=true"]]:
                print("Running", " ".join(command), flush=True)
                subprocess.run(command, cwd=build, env=env, stdout=log, stderr=subprocess.STDOUT, check=True)
        built = build / "gas/as-new"
        actual = sha256(built)
        if actual != spec["assembler_sha256"]:
            raise ValueError(f"Built gas {spec['version']} ({actual}) differs from the lock; the host compiler differs from the recorded recipe")
        shutil.copy2(built, assembler)
    if sha256(assembler) != spec["assembler_sha256"]:
        raise ValueError(f"Installed {spec['install_directory']}/as differs from the lock file; remove it and rerun setup")
    return assembler


def main() -> None:
    if platform.system() != "Darwin":
        raise SystemExit("This bootstrap is for macOS; use the lock file to provision another host")
    make = shutil.which("gmake")
    if make is None:
        raise SystemExit("GNU make is required: brew install make")
    CACHE.mkdir(exist_ok=True)
    lock = json.loads((PROJECT / "toolchain.lock.json").read_text())
    prefix = CACHE / "binutils"
    if not (prefix / "bin/mips64-elf-as").exists():
        archive = download(lock["binutils"], "binutils-2.37.tar.xz")
        with tarfile.open(archive) as tar:
            tar.extractall(CACHE, filter="data")
        build = CACHE / "binutils-build"
        build.mkdir(exist_ok=True)
        env = os.environ.copy()
        env.update(CC="clang", CXX="clang++", CFLAGS=lock["binutils"]["cflags"], CXXFLAGS="-O2", MAKE=make)
        configure = ["../binutils-2.37/configure", "--target=mips64-elf", "--with-arch=vr4300", "--enable-64-bit-bfd", "--with-system-zlib", "--disable-nls", "--disable-werror", "--disable-gdb", "--disable-sim", "--disable-multilib", "--disable-shared", f"--prefix={prefix}"]
        with (CACHE / "binutils-build.log").open("w") as log:
            for command in [configure, [make, "-j8"], [make, "install"]]:
                print("Running", " ".join(command), flush=True)
                subprocess.run(command, cwd=build, env=env, stdout=log, stderr=subprocess.STDOUT, check=True)
    gcc_dir = install_prebuilt_compiler(lock["gcc_candidate"], "gcc-2.7.2-mac.tar.gz")
    pm281_dir = install_prebuilt_compiler(lock["gcc_pm281"], "gcc-papermario-mac.tar.gz")
    kmc_dir = PROJECT / lock["binutils_kmc"]["install_directory"]
    if not (kmc_dir / "as").is_file():
        archive = download(lock["binutils_kmc"], "binutils-kmc-2.6-mac.tar.gz")
        kmc_dir.mkdir(exist_ok=True)
        with tarfile.open(archive) as tar:
            tar.extractall(kmc_dir, filter="data")
    if sha256(kmc_dir / "as") != lock["binutils_kmc"]["assembler_sha256"]:
        raise ValueError("Existing KMC assembler differs from the lock file")
    gas291 = install_gnu_gas(lock["binutils_gnu291"], make)
    for tool in [prefix / "bin/mips64-elf-as", prefix / "bin/mips64-elf-ld", gcc_dir / "gcc", pm281_dir / "gcc", kmc_dir / "as", gas291]:
        print(subprocess.check_output([str(tool), "--version"], text=True, stderr=subprocess.STDOUT).splitlines()[0])
    print("Ready. Python packages are pinned separately in uv.lock.")


if __name__ == "__main__":
    main()

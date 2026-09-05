import argparse
from pathlib import Path
from get_platform import get_platform
import requests
import stat

root_path = Path(__file__).parent.parent


platform = get_platform()
if platform is None:
    exit(1)

parser = argparse.ArgumentParser()
parser.add_argument("tool", choices=("dsd", "wibo", "objdiff"))
parser.add_argument("tag")
parser.add_argument("--path", type=Path, required=True)
args = parser.parse_args()


def dsd_url(tag: str) -> str:
    return f'https://github.com/AetiasHax/ds-decomp/releases/download/{tag}/dsd-{platform.system}-{platform.machine}{platform.exe}'

def wibo_url(tag: str) -> str:
    return f'https://github.com/decompals/wibo/releases/download/{tag}/wibo'

def objdiff_url(tag: str) -> str:
    return f'https://github.com/encounter/objdiff/releases/download/{tag}/objdiff-cli-{platform.system}-{platform.machine}{platform.exe}'

TOOLS = {
    "dsd": dsd_url,
    "wibo": wibo_url,
    "objdiff": objdiff_url,
}


download_url = TOOLS[args.tool](args.tag)
print(f'\nDownloading {args.tool} {args.tag}...')
response = requests.get(download_url)
response.raise_for_status()
out_path: Path = args.path
with out_path.open('wb') as f:
    f.write(response.content)
out_path.chmod(out_path.stat().st_mode | stat.S_IEXEC)

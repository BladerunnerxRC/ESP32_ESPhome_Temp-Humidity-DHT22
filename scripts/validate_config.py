"""Compile a disposable, non-deployable config without reading real secrets."""
import argparse
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

SECRETS = '''wifi_ssid: "validation-network"
wifi_password: "validation-password"
enviro_b2_api_key: "AQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQEBAQE="
enviro_b2_ota_pwd: "validation-password"
enviro_b2_ap_sid: "validation-recovery"
enviro_b2_ap_pwd: "validation-password"
'''

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--generate-only', action='store_true')
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    with tempfile.TemporaryDirectory(prefix='enviro-') as tmp:
        target = Path(tmp)
        for name in ('enviro-b2.yaml', 'wifi.png', 'Roboto-Regular.ttf', 'enviro_helpers.h'):
            source = root / name
            if source.exists():
                shutil.copy2(source, target / name)
        (target / 'secrets.yaml').write_text(SECRETS, encoding='utf-8')
        command = [sys.executable, '-m', 'esphome', 'compile', str(target / 'enviro-b2.yaml')]
        if args.generate_only:
            command.append('--only-generate')
        subprocess.run(command, check=True)

if __name__ == '__main__':
    main()

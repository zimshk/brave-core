import argparse
import os

from shutil import copy

"""
Command to approximate for brave-core on windows:

python tools/signature_generator.py --input_file "BraveAdsTest-win32-x64/BraveAdsTest.exe" --flag 1

On Mac the command is in an email from brian Bondy(I forwarded to devops team), something like:
'python tools/signature_generator.py --input_file "' + wvBundle + '" --output_file "' + wvBundleSig + '" --flag 1',

"""

# Verify all environment variables needed for Widevine 
# signing are set
if 'SIGN_WIDEVINE_PASSPHRASE' in os.environ:
    pass
if 'SIGN_WIDEVINE_CERT' in os.environ:
    pass
if 'SIGN_WIDEVINE_KEY' in os.environ:
    pass

# NOTE: For now copy from jenkins home dir but in future use
# the same process as browser-laptop where they wget this from
# the private brave/internal repository

copy('C:\Users\jenkins\signature_generator.py', 'C:\jenkins\win\src\brave\script')

# Verify Widevine script exists
assert(os.path.isfile('C:\jenkins\win\src\brave\script\signature_generator.py'))

# NOTE 2: The python cryptography module needs to be installed on the system
# for signature_generator.py, we should verify it exists and exit before
# running the script and describe the error in depth if not found. This module
# should be installed via `pip` as we install the Jenkins worker node.

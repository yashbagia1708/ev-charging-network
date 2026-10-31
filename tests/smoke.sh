#!/usr/bin/env bash
# Drive the menu with scripted input and check a few known answers.
# Run from the repo root (the program reads Locations.txt and Distance.txt
# from the current directory): ./tests/smoke.sh
set -euo pipefail

check() {  # check <name> <menu input> <expected text>
    out=$(printf '%b' "$2" | ./evcharging)
    if grep -qF -- "$3" <<<"$out"; then
        echo "ok    $1"
    else
        echo "FAIL  $1: expected '$3'"
        exit 1
    fi
}

check "loads 24 locations"            '1\nn\n'                 "Pymble"
check "power line MST total"          '4\nn\n'                 "Total length of the power line: 221.5"
check "cheapest station listed first" '5\nn\n'                 "Kingswood            yes      free of charge"
check "nearest station from Penrith"  '6\n0\nn\n'              "The distance to this station is: 3.7"
check "cheapest station incl. travel" '7\nPenrith\n10\nn\n'    "Total cost = \$0.59"
check "minimal-cost path"             '8\nPenrith\nCentral\n10\nn\n' "Travel path: Penrith, Blacktown, Central"
check "minimal-cost travel cost"      '8\nPenrith\nCentral\n10\nn\n' "Travel cost = \$6.576"
check "big charge uses a free stop"   '9\nPenrith\nCentral\n40\nn\n' "Charging cost = Free"
check "small charge costs the trip"   '9\nPenrith\nCentral\n10\nn\n' "Total cost = \$9.076"

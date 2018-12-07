# !/usr/bin/bash
VALUE=16

ipcs | grep $VALUE > my_test_abcdee
MY_ID="$(cat my_test_abcdee | grep -Eow '\w{10}')"
ipcrm -M $MY_ID
ipcrm -S $MY_ID


VALUE=128

ipcs | grep $VALUE > my_test_abcdee
MY_ID="$(cat my_test_abcdee | grep -Eow '\w{10}')"
ipcrm -M $MY_ID
ipcrm -S $MY_ID

